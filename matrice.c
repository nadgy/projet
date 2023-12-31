#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <ctype.h> // Inclure le fichier d'en-tête ctype.h

#define N1 4 // Nombre de lignes de B
#define M1 3 // Nombre de colonnes de B
#define N2 5 // Nombre de lignes de C
#define M2 6 // Nombre de colonnes de C
#define N (N1 * M1 * N2 * M2) // Taille du tableau T et de la matrice A

int B[N1][M1];
int C[N2][M2];
int A[N][N];
int T[N]; // Tableau pour stocker les données produites
pthread_mutex_t mutex; // Mutex pour la section critique
sem_t empty; // Sémaphore pour les cases vides
sem_t full; // Sémaphore pour les cases pleines

int produce() {
    return rand() % 10;
}

void insert_item(int item) {
    static int index = 0;
    pthread_mutex_lock(&mutex);
    T[index] = item;
    index = (index + 1) % N; // Utilisation du modulo pour un tampon circulaire
    pthread_mutex_unlock(&mutex);
}

int remove_item() {
    static int index = 0;
    int item;
    pthread_mutex_lock(&mutex);
    item = T[index];
    index = (index + 1) % N; // Utilisation du modulo pour un tampon circulaire
    pthread_mutex_unlock(&mutex);
    return item;
}

void *producer(void *arg) {
    int nb = *((int *)arg);
    for (int i = 0; i < nb; i++) {
        int item = produce();
        sem_wait(&empty); // Attendre qu'il y ait une case vide
        insert_item(item); // Insérer l'item dans le tableau
        sem_post(&full); // Signaler qu'il y a une case pleine
    }
    pthread_exit(NULL);
}

void *consumer(void *arg) {
    int nb = *((int *)arg);
    while (nb > 0) {
        sem_wait(&full); // Attendre qu'il y ait une case pleine
        int item = remove_item(); // Retirer l'item du tableau
        sem_post(&empty); // Signaler qu'il y a une case vide
        nb--;
    }
    pthread_exit(NULL);
}

int main() {
    sem_init(&empty, 0, N); // Initialiser le sémaphore empty à la taille maximale du tampon
    sem_init(&full, 0, 0); // Initialiser le sémaphore full à 0
    pthread_mutex_init(&mutex, NULL); // Initialiser le mutex
    srand(time(NULL));

    // Initialiser la matrice B aléatoirement
    for (int i = 0; i < N1; i++) {
        for (int j = 0; j < M1; j++) {
            B[i][j] = rand() % 10;
        }
    }

    // Initialiser la matrice C aléatoirement
    for (int i = 0; i < N2; i++) {
        for (int j = 0; j < M2; j++) {
            C[i][j] = rand() % 10;
        }
    }

    pthread_t threads_prod[N];
    pthread_t threads_cons[N];
    int nb = N; // Nombre d'itérations pour chaque thread
    int i; // Déclarer la variable i
    for (i = 0; i < N; i++) {
        if (pthread_create(&threads_prod[i], NULL, producer, &nb) != 0 ||
            pthread_create(&threads_cons[i], NULL, consumer, &nb) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i < N; i++) {
        if (pthread_join(threads_prod[i], NULL) != 0 ||
            pthread_join(threads_cons[i], NULL) != 0) {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }

    sem_destroy(&empty); // Détruire le sémaphore empty
    sem_destroy(&full); // Détruire le sémaphore full
    pthread_mutex_destroy(&mutex); // Détruire le mutex

    // Copier les éléments de T dans la matrice A
    for (i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = T[i * N + j];
        }
    }

    // Affichage des matrices B, C et A
    printf("Matrice B :\n");
    for (i = 0; i < N1; i++) {
        for (int j = 0; j < M1; j++) {
            printf("%d ", B[i][j]);
        }
        printf("\n");
    }

    printf("\nMatrice C :\n");
    for (i = 0; i < N2; i++) {
        for (int j = 0; j < M2; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }

    printf("\nMatrice A :\n");
    for (i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }

    return 0;
}
