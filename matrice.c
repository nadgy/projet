//section include..
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


//define
#define MAX_THREADS 4
#define N 5

//variable globales 
//les matrices
int B[N][N];
int C[N][N];
int A[N][N];
//le tampon
int T[N];
//pour la synchronisation
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


// Producteur
void* producer(void* arg) {
    int row = *(int*)arg;
    int result = 0;

    for (int j = 0; j < N; ++j) {
        result += B[row][j] * C[j][row];
    }

    pthread_mutex_lock(&mutex);
    T[row] = result;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}
// consumer
void* consumer(void* arg) {
    int col = *(int*)arg;
    int result = 0;

    for (int i = 0; i < N; ++i) {
        result += T[i] * B[col][i];
    }

    A[col / N][col % N] = result;

    pthread_exit(NULL);
}

int main() {
    // Initialiser les matrices B et C avec des valeurs aléatoires
    srand((unsigned int)time(NULL));

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            B[i][j] = rand() % 10;  // Valeurs aléatoires entre 0 et 9
            C[i][j] = rand() % 10;
        }
    }

    pthread_t producerThreads[N];
    pthread_t consumerThreads[N * N];

    for (int i = 0; i < N; ++i) {
        int* arg = malloc(sizeof(*arg));
        *arg = i;
        pthread_create(&producerThreads[i], NULL, producer, arg);
    }

    for (int i = 0; i < N; ++i) {
        pthread_join(producerThreads[i], NULL);
    }

    for (int i = 0; i < N * N; ++i) {
        int* arg = malloc(sizeof(*arg));
        *arg = i;
        pthread_create(&consumerThreads[i], NULL, consumer, arg);
    }

    for (int i = 0; i < N * N; ++i) {
        pthread_join(consumerThreads[i], NULL);
    }

    // Afficher la matrice résultante A
    printf("Matrice résultante A :\n");
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }

    return 0;
}
