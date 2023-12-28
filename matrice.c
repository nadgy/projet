//section include..
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 100 // Taille du buffer
#define MAX_THREADS 100 // Nombre maximal de threads

int n1, m1, n2, m2; // Dimensions des matrices
int B[100][100], C[100][100], A[100][100]; // Matrices
int T[N], in = 0, out = 0; // Buffer et indices
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex
pthread_cond_t empty = PTHREAD_COND_INITIALIZER; // Condition variable

void *producer(void *arg) {
    int i = *(int *)arg;
    int j, k, sum;
    for (j = 0; j < m2; j++) {
        sum = 0;
        for (k = 0; k < n1; k++) {
            sum += B[i][k] * C[k][j];
        }
        pthread_mutex_lock(&mutex);
        while ((in + 1) % N == out) {
            pthread_cond_wait(&empty, &mutex);
        }
        T[in] = sum;
        in = (in + 1) % N;
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *consumer(void *arg) {
    int i = *(int *)arg;
    int j, k;
    for (j = 0; j < m2; j++) {
        pthread_mutex_lock(&mutex);
        while (in == out) {
            pthread_cond_wait(&empty, &mutex);
        }
        A[i][j] = T[out];
        out = (out + 1) % N;
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    int i, j, k;
    pthread_t threads[MAX_THREADS];
    printf("Entrez les dimensions de la matrice B (n1 m1) : ");
    scanf("%d %d", &n1, &m1);
    printf("Entrez les dimensions de la matrice C (n2 m2) : ");
    scanf("%d %d", &n2, &m2);
    if (m1 != n2) {
        printf("Erreur : les dimensions des matrices ne sont pas compatibles\n");
        return 1;
    }
    printf("Matrice B :\n");
    for (i = 0; i < n1; i++) {
        for (j = 0; j < m1; j++) {
            B[i][j] = rand() % 10;
            printf("%d ", B[i][j]);
        }
        printf("\n");
    }
    printf("Matrice C :\n");
    for (i = 0; i < n2; i++) {
        for (j = 0; j < m2; j++) {
            C[i][j] = rand() % 10;
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }
    for (i = 0; i < n1; i++) {
        pthread_create(&threads[i], NULL, producer, &i);
    }
    for (i = 0; i < n1; i++) {
        pthread_join(threads[i], NULL);
    }
    for (j = 0; j < m2; j++) {
        pthread_create(&threads[j], NULL, consumer, &j);
    }
    for (j = 0; j < m2; j++) {
        pthread_join(threads[j], NULL);
    }
    printf("Matrice A :\n");
    for (i = 0; i < n1; i++) {
        for (j = 0; j < m2; j++) {
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }
    return 0;
}
