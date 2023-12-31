 #include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define MIN_SIZE 2
#define MAX_SIZE 10
#define MAX_VALUE 9

int BUFFER_SIZE;

sem_t full, empty, mutex;

int RowsMatrixA, ColumnsMatrixB, RowsA_ColumnsB;

int MatrixA[MAX_SIZE][MAX_SIZE];
int MatrixB[MAX_SIZE][MAX_SIZE];
int ResultMatrix[MAX_SIZE][MAX_SIZE];

int producerIndex = 0, consumerIndex = 0;
int* buffer;

void fillMatrices() {
    srand(time(NULL));

    RowsMatrixA = rand() % (MAX_SIZE - MIN_SIZE + 1) + MIN_SIZE;
    ColumnsMatrixB = rand() % (MAX_SIZE - MIN_SIZE + 1) + MIN_SIZE;
    RowsA_ColumnsB = rand() % (MAX_SIZE - MIN_SIZE + 1) + MIN_SIZE;

    BUFFER_SIZE = ColumnsMatrixB;

    for (int i = 0; i < RowsMatrixA; ++i) {
        for (int j = 0; j < RowsA_ColumnsB; ++j) {
            MatrixA[i][j] = rand() % (MAX_VALUE + 1);
        }
    }

    for (int i = 0; i < RowsA_ColumnsB; ++i) {
        for (int j = 0; j < ColumnsMatrixB; ++j) {
            MatrixB[i][j] = rand() % (MAX_VALUE + 1);
        }
    }
}

void *calculateElement(void *arg) {
    int* indices = (int *)arg;

    int row = indices[0];
    int col = indices[1];
    int element = 0;

    for (int i = 0; i < RowsA_ColumnsB; i++) {
        element += MatrixA[row][i] * MatrixB[i][col];
    }

    sem_wait(&empty);
    sem_wait(&mutex);

    buffer[producerIndex] = element;
    producerIndex = (producerIndex + 1) % BUFFER_SIZE;

    sem_post(&full);
    sem_post(&mutex);

    pthread_exit(NULL);
}

void *populateResultMatrix(void *arg) {
    sem_wait(&full);
    sem_wait(&mutex);

    int element = buffer[consumerIndex];
    ResultMatrix[consumerIndex / ColumnsMatrixB][consumerIndex % ColumnsMatrixB] = element;
    consumerIndex = (consumerIndex + 1) % BUFFER_SIZE;

    sem_post(&empty);
    sem_post(&mutex);

    pthread_exit(NULL);
}

void displayResultMatrix() {
    printf("Le résultat est :\n\n");
    for (int i = 0; i < RowsMatrixA; i++) {
        for (int j = 0; j < ColumnsMatrixB; j++) {
            printf("%d\t", ResultMatrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    fillMatrices();

    int resultSize = RowsMatrixA * ColumnsMatrixB;
    buffer = malloc(BUFFER_SIZE * sizeof(int));
    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer[i] = 0;  // or any default value
    }

    sem_init(&mutex, 0, 1);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUFFER_SIZE);

    pthread_t producers[resultSize];
    pthread_t consumers[resultSize];

    int rowMatrixA = 0;
    int colMatrixB = 0;

    int indices[resultSize][2];

    for (int i = 0; i < resultSize; i++) {
        indices[i][0] = rowMatrixA;
        indices[i][1] = colMatrixB;

        colMatrixB++;

        if (colMatrixB == ColumnsMatrixB) {
            colMatrixB = 0;
            rowMatrixA++;
        }
    }

    // Thread creation and execution
    for (int i = 0; i < resultSize; i++) {
        pthread_create(&producers[i], NULL, &calculateElement, (void *)indices[i]);
        pthread_create(&consumers[i], NULL, &populateResultMatrix, NULL);

        // Join the producer and consumer threads immediately
        pthread_join(producers[i], NULL);
        pthread_join(consumers[i], NULL);
    }

    displayResultMatrix();

    free(buffer);

    return 0;
}
