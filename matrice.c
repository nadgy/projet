#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define N 10 // Taille du tampon
#define n1 3  // Nombre de lignes dans B
#define m1 3  // Nombre de colonnes dans B
#define n2 3  // Nombre de lignes dans C
#define m2 3  // Nombre de colonnes dans C

// Variables globales
//les matrices
int B[n1][m1];
int C[n2][m2];
int A[n1][m2];  

//le tampon
  
int T[N];

// Synchronisation
pthread_mutex_t mutex;
sem_t empty;
sem_t full;

int produce(int row) {
   
    return row;
}

void insert_item(int item) {
   
    T[item] = item;
}


int remove_item() {
   
    return T[0];
}

// Producteur
void *producer(void *arg) {
    int row;
    for (row = 0; row < n1; row++) {
        int item = produce(row);
        sem_wait(&empty); // Attente d'une place libre
        pthread_mutex_lock(&mutex);
        // Section critique
        insert_item(item);
        pthread_mutex_unlock(&mutex);
        sem_post(&full); // Il y a une place remplie en plus
    }
    pthread_exit(NULL);
}

// Consommateur
void *consumer(void *arg) {
    while (1) {
        sem_wait(&full); // Attente d'une place remplie
        pthread_mutex_lock(&mutex);
        // Section critique
        int item = remove_item();
        pthread_mutex_unlock(&mutex);
        sem_post(&empty); // Il y a une place libre en plus
        // Insérer l'élément dans la matrice résultante A au bon emplacement
        // ... logique pour placer l'élément dans A
    }
    pthread_exit(NULL);
}

int main() {
   

    // Initialisation des synchronisations
    sem_init(&empty, 0, N);  // Buffer vide
    sem_init(&full, 0, 0);   // Buffer vide
    pthread_mutex_init(&mutex, NULL); // Exclusion mutuelle

    // Création des threads
    pthread_t producer_thread, consumer_thread;
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // Attente des threads
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // Destruction 
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}
