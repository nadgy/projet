/*
PROJET SE
Nom : Djaroum Samy
Groupe : 01
Matricule : 212133018982
Date : 30/12/2023
*/

//section include..
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

//define
#define N 10 // Nombre de places dans le tampon
#define M 10 // Nombre de colonnes dans la matrice B et de lignes dans la matrice C

// Déclaration des matrices globales
double **B, **C, **A;

// Déclaration du tampon
double T[N];

// Mutex pour protéger l'accès aux données
pthread_mutex_t mutex;

// Sémaphores pour gérer le tampon
sem_t empty; // Sémaphore pour les places vides dans le tampon
sem_t full; // Sémaphore pour les places occupées dans le tampon

// Fonction pour produire une ligne de la matrice résultante
double produce(int line) {
  double sum = 0;
  for (int i = 0; i < M; i++) {
      sum += B[line][i] * C[i][0]; // Calcul du produit de la ligne par la colonne
  }
  return sum;
}

// Fonction pour insérer un élément dans le tampon
void insert_item(double item) {
  for (int i = 0; i < N; i++) {
      if (T[i] == 0) {
          T[i] = item; // Insère l'élément à la première place libre
          break;
      }
  }
}

// Fonction pour retirer un élément du tampon
double remove_item() {
  double item = 0;
  for (int i = 0; i < N; i++) {
      if (T[i] != 0) {
          item = T[i]; // Retire le premier élément non nul rencontré
          T[i] = 0;
          break;
      }
  }
  return item;
}

// Fonction du producteur
void *producer(void *arg) {
  int line = *((int *) arg);
  double item = produce(line); // Produit un élément
  sem_wait(&empty); // Attend qu'une place soit libre dans le tampon
  pthread_mutex_lock(&mutex); // Verrouille le mutex pour protéger l'accès au tampon
  insert_item(item); // Insère l'élément dans le tampon
  printf("Producer inserted item: %f\n", item);
  pthread_mutex_unlock(&mutex); // Déverrouille le mutex
  sem_post(&full); // Signale qu'une place est occupée dans le tampon
  free(arg);
  return NULL;
}

// Fonction du consommateur
void *consumer(void *arg) {
  for (int i = 0; i < N; i++) {
      sem_wait(&full); // Attend qu'une place soit occupée dans le tampon
      pthread_mutex_lock(&mutex); // Verrouille le mutex pour protéger l'accès au tampon
      double item = remove_item(); // Retire un élément du tampon
      printf("Consumer removed item: %f\n", item);
      pthread_mutex_unlock(&mutex); // Déverrouille le mutex
      sem_post(&empty); // Signale qu'une place est libre dans le tampon

      // Place l'élément dans la matrice résultante A
      int line = *((int *) arg);
      A[line][0] = item;
  }
  return NULL;
}

int main() {
  pthread_t producers[N], consumers[N];

  // Initialisation des matrices B, C, et A
  B = malloc(N * sizeof(double *));
  C = malloc(M * sizeof(double *));
  A = malloc(N * sizeof(double *));
  for (int i = 0; i < N; i++) {
      B[i] = malloc(M * sizeof(double));
      A[i] = malloc(M * sizeof(double));
      for (int j = 0; j < M; j++) {
          B[i][j] = i + j; // Affectation de valeurs à B
          A[i][j] = 0;
      }
  }
  for (int i = 0; i < M; i++) {
      C[i] = malloc(sizeof(double));
      C[i][0] = i; //Affectation de valeurs à C
  }

  // Initialisation du mutex
  pthread_mutex_init(&mutex, NULL);

  // Initialisation des sémaphores
  sem_init(&empty, 0, N); // Initialisation du sémaphore pour le tampon vide
  sem_init(&full, 0, 0); // Initialisation du sémaphore pour le tampon plein

  // Création des threads producteurs
  for (int i = 0; i < N; i++) {
      int *arg = malloc(sizeof(*arg));
      *arg = i;
      pthread_create(&producers[i], NULL, producer, arg);
  }

  // Création des threads consommateurs
  for (int i = 0; i < N; i++) {
      int *arg = malloc(sizeof(*arg));
      *arg = i;
      pthread_create(&consumers[i], NULL, consumer, arg);
  }

  // Attente de la fin des threads
  for (int i = 0; i < N; i++) {
      pthread_join(producers[i], NULL);
      printf("Producer thread %d finished\n", i);
      pthread_join(consumers[i], NULL);
      printf("Consumer thread %d finished\n", i);
  }

  // Destruction du mutex
  pthread_mutex_destroy(&mutex);

  // Destruction des sémaphores
  sem_destroy(&empty); // Destruction du sémaphore pour le tampon vide
  sem_destroy(&full); // Destruction du sémaphore pour le tampon plein

  return 0;
}
