#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_SIZE 10
#define MAX_THREADS 10
#define N 100

int B[MAX_SIZE][MAX_SIZE], C[MAX_SIZE][MAX_SIZE];
int n1, m1, n2, m2;
int T[N];
int A[MAX_SIZE][MAX_SIZE];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *producer(void *arg) {
    int row = *((int *)arg);

    for (int j = 0; j < m2; j++) {
        T[row * m2 + j] = 0;
        for (int k = 0; k < m1; k++) {
            T[row * m2 + j] += B[row][k] * C[k][j];
        }
    }

    pthread_exit(NULL);
}

void *consumer(void *arg) {
    int index = *((int *)arg);
    int row = index / m2;
    int col = index % m2;

    pthread_mutex_lock(&mutex);
    A[row][col] = T[index];
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

void fill_matrix_random(int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = rand() % 10;
        }
    }
}

int main() {
    printf("*********MATRICE B***********\n");
    printf("Entrez le nombre de lignes : ");
    scanf("%d", &n1);
    printf("Entrez le nombre de colonnes : ");
    scanf("%d", &m1);
    system("clear");
    printf("*********MATRICE C***********\n");
    printf("Entrez le nombre de lignes : ");
    scanf("%d", &n2);
    printf("Entrez le nombre de colonnes : ");
    scanf("%d", &m2);
    system("clear");

    if (m1 != n2) {
        printf("Impossible de faire la multiplication entre ces deux matrices !\n");
        return 1;
    }

    fill_matrix_random(B, n1, m1);
    fill_matrix_random(C, n2, m2);
    pthread_t producer_threads[MAX_THREADS];
    pthread_t consumer_threads[N];
    int producer_arguments[MAX_THREADS];
    int consumer_arguments[N];

    for (int i = 0; i < n1; i++) {
        producer_arguments[i] = i;
        pthread_create(&producer_threads[i], NULL, producer, (void *)&producer_arguments[i]);
    }

    for (int i = 0; i < n1; i++) {
        pthread_join(producer_threads[i], NULL);
    }

    for (int i = 0; i < N; i++) {
        consumer_arguments[i] = i;
        pthread_create(&consumer_threads[i], NULL, consumer, (void *)&consumer_arguments[i]);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(consumer_threads[i], NULL);
    }

    printf("Matrice résultante A :\n");
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < m2; j++) {
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }

    return 0;
}
