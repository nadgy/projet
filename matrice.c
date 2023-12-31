#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define N1 3
#define M1 4
#define N2 4
#define M2 3
#define BUFFER_SIZE 5

int B[N1][M1];
int C[N2][M2];
int A[N1][M2];
int buffer[BUFFER_SIZE][M2];
int in = 0, out = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t buffer_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t buffer_empty = PTHREAD_COND_INITIALIZER;

void initialize_matrices() {
    
    srand(time(NULL));

    for (int i = 0; i < N1; ++i) {
        for (int j = 0; j < M1; ++j) {
            B[i][j] = rand() % 10;
        }
    }

    for (int i = 0; i < N2; ++i) {
        for (int j = 0; j < M2; ++j) {
            C[i][j] = rand() % 10;
        }
    }
}

void print_matrix(int rows, int cols, int matrix[rows][cols]) {
   
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void* producer(void* arg) {
    int thread_id = *(int*)arg;
    for (int i = 0; i < N1; ++i) {
        sleep(1);
        for (int j = 0; j < M2; ++j) {
            A[i][j] = 0;
            for (int k = 0; k < M1; ++k) {
                A[i][j] += B[i][k] * C[k][j];
            }
        }

        pthread_mutex_lock(&mutex);
        while ((in + 1) % BUFFER_SIZE == out) {
            printf("Producer %d: Buffer full, waiting...\n", thread_id);
            pthread_cond_wait(&buffer_empty, &mutex);
        }
  
        for (int j = 0; j < M2; ++j) {
            buffer[in][j] = A[i][j];
        }

        printf("Producer %d: Produced row %d\n", thread_id, i);
        in = (in + 1) % BUFFER_SIZE;
        pthread_cond_signal(&buffer_full);
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

void* consumer(void* arg) {
    int thread_id = *(int*)arg;
    for (int i = 0; i < N1; ++i) {
        pthread_mutex_lock(&mutex);
        while (in == out) {
            printf("Consumer %d: Buffer empty, waiting...\n", thread_id);
            pthread_cond_wait(&buffer_full, &mutex);
        }

        for (int j = 0; j < M2; ++j) {
            A[i][j] = buffer[out][j];
        }

        printf("Consumer %d: Consumed row %d\n", thread_id, i);
        out = (out + 1) % BUFFER_SIZE;
        pthread_cond_signal(&buffer_empty);
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

int main() {
    initialize_matrices();
   
    printf("Matrix B:\n");
    print_matrix(N1, M1, B);
    printf("Matrix C:\n");
    print_matrix(N2, M2, C);

    pthread_t producer_thread, consumer_thread;
    int producer_id = 1, consumer_id = 2; 

    if (pthread_create(&producer_thread, NULL, producer, &producer_id) != 0) {
        perror("Error creating producer thread");
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&consumer_thread, NULL, consumer, &consumer_id) != 0) {
        perror("Error creating consumer thread");
        exit(EXIT_FAILURE);
    }

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    printf("Resulting Matrix A:\n");
    print_matrix(N1, M2, A);

    return 0;
}
