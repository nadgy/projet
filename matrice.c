#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX 10

int B[MAX][MAX];
int C[MAX][MAX];
int A[MAX][MAX];
int buffer[MAX];
int buffer_index = 0;

int rows, cols; // dimensions of matrices

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full =PTHREAD_COND_INITIALIZER;;
pthread_cond_t empty =PTHREAD_COND_INITIALIZER; ;

// Function to initialize matrices B and C with random values
void fillMatrix() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            B[i][j] = rand() % 10;  
            C[i][j] = rand() % 10;
        }
    }
}

void *producer(void *arg) {
    int my_row = *((int *)arg);

    for (int j = 0; j < cols; j++) {
        int sum = 0;
        for (int k = 0; k < cols; k++) {
            sum += B[my_row][k] * C[k][j];
        }

        pthread_mutex_lock(&mutex);
        while (buffer_index == MAX) {
            pthread_cond_wait(&full, &mutex);
        }

        buffer[buffer_index++] = sum;

        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

void *consumer(void *arg) {
    for (int i = 0; i < rows; i++) {
        pthread_mutex_lock(&mutex);
        while (buffer_index == 0) {
            pthread_cond_wait(&empty, &mutex);
        }

        // Store the entire row in matrix A in the correct order
        for (int j = 0; j < cols; j++) {
            A[i][j] = buffer[j];
        }

        // Shift the remaining elements in the buffer to maintain order
        for (int k = 0; k < buffer_index - cols; k++) {
            buffer[k] = buffer[k + cols];
        }

        buffer_index -= cols;

        pthread_cond_signal(&full);
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}



int main() {
    // Set the dimensions of matrices
    rows = 3; 
    cols = 3; 
    //pthread_cond_init(&full, NULL);  // You can pass specific attributes as the second argument
//pthread_cond_init(&empty, NULL);

    // Initialize matrices B and C with random values
    fillMatrix();

    pthread_t producers[rows];
    pthread_t consumer_t;

    // Create producer threads
    for (int i = 0; i < rows; i++) {
        int *arg = malloc(sizeof(*arg));
        *arg = i;
        pthread_create(&producers[i], NULL, producer, arg);
    }

    // Create consumer thread
    pthread_create(&consumer_t, NULL, consumer, NULL);

    // Join producer threads
    for (int i = 0; i < rows; i++) {
        pthread_join(producers[i], NULL);
    }

    // Join consumer thread
    pthread_join(consumer_t, NULL);

    // Print matrices B, C, and A
    printf("B matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d\t", B[i][j]);
        }
        printf("\n");
    }

    printf("C matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d\t", C[i][j]);
        }
        printf("\n");
    }

    printf("Result matrix (A):\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d\t", A[i][j]);
        }
        printf("\n");
    }
        pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&full);
    pthread_cond_destroy(&empty);

    return 0;
}

