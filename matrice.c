//SETTI MOHAMED GROUP 3
// Includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

// Global variables
int matrixBRows, matrixBCols, matrixCRows, matrixCCols;
int **matrixB;
int **matrixC;
int **matrixResult;
int shouldExit = 0;

// Buffer
#define BUFFER_SIZE 5 // Max number of items in buffer
typedef struct Buffer
{
    int *data;
    int *posX;
    int *posY;
    int count;
    int in;
    int out;
    int produced;
    int consumed;
} Buffer;
Buffer sharedBuffer;

// Semaphores
pthread_mutex_t mutex;
sem_t empty;
sem_t full;

// Producer
void *producer(void *arg)
{
    int index = *(int *)arg;
    int item = 0;

    for (int i = 0; i < matrixCCols; i++)
    {
        // Calculate matrix element
        item = 0;
        for (int j = 0; j < matrixBCols; j++)
            item += matrixC[index][j] * matrixResult[j][i];

        // Produce item into the shared buffer
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        sharedBuffer.data[sharedBuffer.in] = item;
        sharedBuffer.posX[sharedBuffer.in] = index;
        sharedBuffer.posY[sharedBuffer.in] = i;
        sharedBuffer.in = (sharedBuffer.in + 1) % BUFFER_SIZE;
        sharedBuffer.count++;
        sharedBuffer.produced++;

        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }

    pthread_exit(NULL);
}

// Consumer
void *consumer(void *arg)
{
    int index = *(int *)arg;
    int item = 0;

    while (!shouldExit)
    {
        // Consume item from the shared buffer
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        if (shouldExit)
        {
            pthread_mutex_unlock(&mutex);
            break;
        }

        item = sharedBuffer.data[sharedBuffer.out];
        matrixB[sharedBuffer.posX[sharedBuffer.out]][sharedBuffer.posY[sharedBuffer.out]] = item;
        sharedBuffer.out = (sharedBuffer.out + 1) % BUFFER_SIZE;
        sharedBuffer.count--;
        sharedBuffer.consumed++;

        // Check exit condition
        shouldExit = sharedBuffer.consumed == matrixBRows * matrixCCols;
        if (shouldExit)
            for (int i = 0; i < BUFFER_SIZE; i++)
                sem_post(&full);

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }

    pthread_exit(NULL);
}

// Generate random matrices
void generateMatrices(void)
{
    // Generate matrix B
    matrixB = (int **)malloc(matrixBRows * sizeof(int *));
    for (int i = 0; i < matrixBRows; i++)
        matrixB[i] = (int *)malloc(matrixBCols * sizeof(int));

    // Generate matrix C
    matrixC = (int **)malloc(matrixCRows * sizeof(int *));
    for (int i = 0; i < matrixCRows; i++)
        matrixC[i] = (int *)malloc(matrixCCols * sizeof(int));

    // Fill matrix B with random numbers
    for (int i = 0; i < matrixBRows; i++)
        for (int j = 0; j < matrixBCols; j++)
            matrixB[i][j] = rand() % 10;

    // Fill matrix C with random numbers
    for (int i = 0; i < matrixCRows; i++)
        for (int j = 0; j < matrixCCols; j++)
            matrixC[i][j] = rand() % 10;

    // Pretty print both matrices
    printf("\n| Matrix B\n");
    for (int i = 0; i < matrixBRows; i++)
    {
        for (int j = 0; j < matrixBCols; j++)
            printf("%d  ", matrixB[i][j]);
        printf("\n");
    }
    printf("\n| Matrix C\n");
    for (int i = 0; i < matrixCRows; i++)
    {
        for (int j = 0; j < matrixCCols; j++)
            printf("%d  ", matrixC[i][j]);
        printf("\n");
    }
    printf("\n\n");
}

int main(int argc, char *argv[])
{
    srand(time(NULL));

    // Get matrix dimensions from user
    printf("Enter number of rows and cols in matrix B: ");
    scanf("%d %d", &matrixBRows, &matrixBCols);
    printf("Enter number of rows and cols in matrix C: ");
    scanf("%d %d", &matrixCRows, &matrixCCols);

    // Check if multiplication is possible
    if (matrixBCols != matrixCRows)
    {
        perror("Multiplication of B and C is not possible.\n");
        exit(EXIT_FAILURE);
    }

    // Generate matrices
    generateMatrices();

    // Initialize shared buffer
    sharedBuffer.data = (int *)malloc(BUFFER_SIZE * sizeof(int));
    sharedBuffer.posX = (int *)malloc(BUFFER_SIZE * sizeof(int));
    sharedBuffer.posY = (int *)malloc(BUFFER_SIZE * sizeof(int));
    sharedBuffer.count = 0;
    sharedBuffer.in = 0;
    sharedBuffer.out = 0;
    sharedBuffer.produced = 0;
    sharedBuffer.consumed = 0;

    // Initialize matrix for results
    matrixResult = (int **)malloc(matrixBRows * sizeof(int *));
    for (int i = 0; i < matrixBRows; i++)
    {
        matrixResult[i] = (int *)malloc(matrixCCols * sizeof(int));
    }
    if (matrixResult == NULL)
        perror("Memory allocation for matrix A failed.\n");

    // Initialize semaphores and mutex
    if (pthread_mutex_init(&mutex, NULL) != 0)
    {
        perror("Mutex init failed.\n");
        exit(EXIT_FAILURE);
    }
    if (sem_init(&empty, 0, BUFFER_SIZE) != 0)
    {
        perror("Semaphore [empty] init failed.\n");
        exit(EXIT_FAILURE);
    }
    if (sem_init(&full, 0, 0) != 0)
    {
        perror("Semaphore [full] init failed.\n");
        exit(EXIT_FAILURE);
    }

    // Declare threads
    pthread_t producersThread[matrixBRows];
    pthread_t consumersThread[BUFFER_SIZE];

    // Create producer threads
    for (int i = 0; i < matrixBRows; i++)
    {
        int *index = malloc(sizeof(int));
        *index = i;
        if (pthread_create(&producersThread[i], NULL, producer, index) != 0)
        {
            perror("Thread creation failed.\n");
            exit(EXIT_FAILURE);
        }
    }

    // Create consumer threads
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        int *index = malloc(sizeof(int));
        *index = i;
        if (pthread_create(&consumersThread[i], NULL, consumer, index) != 0)
        {
            perror("Thread creation failed.\n");
            exit(EXIT_FAILURE);
        }
    }

    // Join producer threads
    for (int i = 0; i < matrixBRows; i++)
        pthread_join(producersThread[i], NULL);

    // Join consumer threads
    for (int i = 0; i < BUFFER_SIZE; i++)
        pthread_join(consumersThread[i], NULL);

    // Pretty print matrix Result
    printf("\n| Matrix Result\n");
    for (int i = 0; i < matrixBRows; i++)
    {
        for (int j = 0; j < matrixCCols; j++)
            printf("%d  ", matrixResult[i][j]);
        printf("\n");
    }

    // Cleanup
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    for (int i = 0; i < matrixBRows; i++)
    {
        free(matrixResult[i]);
    }
    free(matrixResult);
    for (int i = 0; i < matrixBRows; i++)
    {
        free(matrixB[i]);
    }
    free(matrixB);
    for (int i = 0; i < matrixBRows; i++)
    {
        free(matrixC[i]);
    }
    free(matrixC);
    free(sharedBuffer.data);
    free(sharedBuffer.posX);
    free(sharedBuffer.posY);

    return 0;
}

