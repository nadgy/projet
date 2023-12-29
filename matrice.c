#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX_THREADS 4
#define N 5

int B[N][N];
int C[N][N];
int A[N][N];
int T[N];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void printMatrix(int matrix[N][N], char* name) {
    printf("Matrice %s :\n", name);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

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

void* consumer(void* arg) {
    int col = *(int*)arg;
    int result = 0;

    for (int i = 0; i < N; ++i) {
        result += T[i] * B[col][i];
    }

    pthread_mutex_lock(&mutex);
    A[col / N][col % N] = result;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main() {
    srand((unsigned int)time(NULL));

    // Initialisation des matrices B et C avec des valeurs aléatoires
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            B[i][j] = rand() % 10;  // Valeurs aléatoires entre 0 et 9
            C[i][j] = rand() % 10;
        }
    }

    // Affichage des matrices B et C
    printMatrix(B, "B");
    printMatrix(C, "C");

    // Threads producteurs
    pthread_t producerThreads[N];
    for (int i = 0; i < N; ++i) {
        int* arg = malloc(sizeof(*arg));
        *arg = i;
        pthread_create(&producerThreads[i], NULL, producer, arg);
    }

    // Attendre la fin des threads producteurs
    for (int i = 0; i < N; ++i) {
        pthread_join(producerThreads[i], NULL);
    }

    // Threads consommateurs
    pthread_t consumerThreads[N * N];
    for (int i = 0; i < N * N; ++i) {
        int* arg = malloc(sizeof(*arg));
        *arg = i;
        pthread_create(&consumerThreads[i], NULL, consumer, arg);
    }

    // Attendre la fin des threads consommateurs
    for (int i = 0; i < N * N; ++i) {
        pthread_join(consumerThreads[i], NULL);
    }

    // Affichage de la matrice résultante A
    printMatrix(A, "A");

    return 0;
}
