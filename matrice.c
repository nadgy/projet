/*
 * Author:   Youcef BADAOUI
 * Email:    youcefbadaoui@outlook.com
 * Date:     2023/12/16
 */

/* Required headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>

/* Helper Prototypes */
void DEBUG(const char *threadType, int threadIndex, const char *msg, int *item);
void GENERATE_MATRIX(int ***globalMatrix, int n, int m, const char *name);
void PRINT_MATRIX(int **matrix, int n, int m, const char *name);
void CLEAR_MATRIX(int ***matrix, int n);

/* Buffer Item */
typedef struct BufferItem
{
    int value; // Value of item (result of multiplication)
    int i;     // Row index in matrix A
    int j;     // Column index in matrix A
} BufferItem;

/* Buffer Queue (Circular) */
typedef struct BufferQueue
{
    BufferItem *items; // Array of items T[BUFFER_SIZE]
    int inIndex;       // Index of next item to insert
    int outIndex;      // Index of next item to remove
    int produced;      // Number of items produced
    int consumed;      // Number of items consumed
} BufferQueue;

/* Global variables */
int BUFFER_SIZE, NUM_CONSUMERS; // Buffer size and number of consumers threads
int n1, m1, n2, m2;             // Dimensions of matrices
int **A, **B, **C;              // Matrices
int shouldExit = 0;             // Flag to exit
BufferQueue buffer;             // Buffer queue

/* Semaphores */
pthread_mutex_t mutex; // Mutex for critical section
sem_t empty;           // Flag to indicate if buffer is empty
sem_t full;            // Flag to indicate if buffer is full

/* Insert Item Function */
void insertItem(int value, int i, int j)
{
    // Produce item
    BufferItem item;
    item.value = value;
    item.i = i;
    item.j = j;

    // Insert item in buffer
    buffer.items[buffer.inIndex] = item;
    buffer.inIndex = (buffer.inIndex + 1) % BUFFER_SIZE;
    buffer.produced++;
}

/* Remove Item Function */
BufferItem removeItem()
{
    // Consume item from buffer
    BufferItem item = buffer.items[buffer.outIndex];
    buffer.outIndex = (buffer.outIndex + 1) % BUFFER_SIZE;
    buffer.consumed++;

    return item;
}

/* Producer Thread */
void *producer(void *arg)
{
    int index = *(int *)arg; // Producer index (Used to identify thread and what row to calculate)
    int sum = 0;             // Sum of multiplication

    /*** DEBUG ***/
    DEBUG("Producer", index + 1, "STARTED.", NULL);
    /*** DEBUG ***/

    for (int i = 0; i < m2; i++)
    {
        // Calculate item
        sum = 0;
        for (int j = 0; j < m1; j++)
            sum += B[index][j] * C[j][i];

        sem_wait(&empty);           // If buffer is full, wait
        pthread_mutex_lock(&mutex); // Lock critical section

        // Insert result in buffer
        insertItem(sum, index, i);

        /*** DEBUG ***/
        DEBUG("Producer", index + 1, "Produced item", &sum);
        /*** DEBUG ***/

        pthread_mutex_unlock(&mutex); // Unlock critical section
        sem_post(&full);              // Signal that buffer is full
    }

    /*** DEBUG ***/
    DEBUG("Producer", index + 1, "FINISHED.", NULL);
    /*** DEBUG ***/

    // Free allocated memory and exit thread
    free(arg);
    pthread_exit(NULL);
}

/* Consumer Thread */
void *consumer(void *arg)
{
    int index = *(int *)arg; // Consumer index (Used to identify thread)

    /*** DEBUG ***/
    DEBUG("Consumer", index + 1, "STARTED.", NULL);
    /*** DEBUG ***/

    // Consume items from buffer until all items are consumed
    while (!shouldExit)
    {
        sem_wait(&full);            // If buffer is empty, wait
        pthread_mutex_lock(&mutex); // Lock critical section

        if (shouldExit)
        {
            pthread_mutex_unlock(&mutex);
            break;
        }

        // Get item from buffer
        BufferItem item = removeItem();

        // Insert item in matrix A
        A[item.i][item.j] = item.value;

        /*** DEBUG ***/
        DEBUG("Consumer", index + 1, "Consumed", &item.value);
        /*** DEBUG ***/

        // Check if all items are consumed
        shouldExit = buffer.consumed == n1 * m2;
        if (shouldExit)
            for (int i = 0; i < NUM_CONSUMERS; i++)
                sem_post(&full); // Signal all consumers to exit

        pthread_mutex_unlock(&mutex); // Unlock critical section
        sem_post(&empty);             // Signal that buffer is empty
    }

    /*** DEBUG ***/
    DEBUG("Consumer", index + 1, "FINISHED.", NULL);
    /*** DEBUG ***/

    // Free allocated memory and exit thread
    free(arg);
    pthread_exit(NULL);
}

/* Program Start */
int main(int argc, char *argv[])
{
    // Seed random number generator (Or else it will generate the same numbers every time)
    srand(time(NULL));

    // Ask user for buffer size and number of consumers
    printf("Enter buffer size: ");
    scanf("%d", &BUFFER_SIZE);
    printf("Enter number of consumers: ");
    scanf("%d", &NUM_CONSUMERS);

    // Make sure that buffer size and number of consumers are greater than 0
    if (BUFFER_SIZE <= 0 || NUM_CONSUMERS <= 0)
    {
        perror("Buffer size and number of consumers must be greater than 0.\n");
        exit(EXIT_FAILURE);
    }

    // Init buffer
    buffer.items = (BufferItem *)malloc(BUFFER_SIZE * sizeof(BufferItem));
    buffer.inIndex = 0;
    buffer.outIndex = 0;
    buffer.produced = 0;
    buffer.consumed = 0;

    // Ask for dimensions of matrices
    printf("Enter number of rows and cols in matrix B: ");
    scanf("%d %d", &n1, &m1);
    printf("Enter number of rows and cols in matrix C: ");
    scanf("%d %d", &n2, &m2);

    // Validate that demensions can form a matrix
    if (n1 <= 0 || m1 <= 0 || n2 <= 0 || m2 <= 0)
    {
        perror("Dimensions of matrices must be greater than 0.\n");
        exit(EXIT_FAILURE);
    }

    // Make sure that multiplication is possible
    if (m1 != n2)
    {
        perror("Multiplication of B and C is not possible.\n");
        exit(EXIT_FAILURE);
    }

    // Generate matrices B and C
    GENERATE_MATRIX(&B, n1, m1, "B");
    GENERATE_MATRIX(&C, n2, m2, "C");

    // Init matrix A
    A = (int **)malloc(n1 * sizeof(int *));
    for (int i = 0; i < n1; i++)
    {
        A[i] = (int *)malloc(m2 * sizeof(int));
    }
    if (A == NULL)
        perror("Memory allocation for matric A failed.\n");

    // Init semaphores
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

    // Declare threads arrays
    pthread_t producers_t[n1];
    pthread_t consumer_t[NUM_CONSUMERS];

    // Create threads
    for (int i = 0; i < n1; i++)
    {
        int *index = malloc(sizeof(int));
        *index = i;
        if (pthread_create(&producers_t[i], NULL, producer, index) != 0)
        {
            perror("Thread creation failed.\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < NUM_CONSUMERS; i++)
    {
        int *index = malloc(sizeof(int));
        *index = i;
        if (pthread_create(&consumer_t[i], NULL, consumer, index) != 0)
        {
            perror("Thread creation failed.\n");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for threads to finish
    for (int i = 0; i < n1; i++)
        pthread_join(producers_t[i], NULL);

    for (int i = 0; i < NUM_CONSUMERS; i++)
        pthread_join(consumer_t[i], NULL);

    // Print matrix A (Result)
    PRINT_MATRIX(A, n1, m2, "A");

    // Cleanup and exit
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    CLEAR_MATRIX(&A, n1);
    CLEAR_MATRIX(&B, n1);
    CLEAR_MATRIX(&C, n2);

    return 0;
}

/* Helper Functions */
void DEBUG(const char *threadType, int threadIndex, const char *msg, int *item)
{
    // This function should print the message in this format: [HH:MM:SS:MS][THREAD]: message
    // Where HH:MM:SS is the current time, THREAD is the name of the thread and message is the message itself
    // Example: [12:30:15:59][Consumer #1]: consumed item: 5
    // Helpful for debugging and understanding what is happening

    // Get current time
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);

    // Convert time to struct tm
    time_t now = currentTime.tv_sec;
    struct tm *t = localtime(&now);

    // Get milliseconds
    long milliseconds = currentTime.tv_usec / 1000;

    // Print message
    if (item == NULL)
        printf("[%02d:%02d:%02d:%03ld][%s #%d]: %s\n", t->tm_hour, t->tm_min, t->tm_sec, milliseconds, threadType, threadIndex, msg);
    else
        printf("[%02d:%02d:%02d:%03ld][%s #%d]: %s: %d\n", t->tm_hour, t->tm_min, t->tm_sec, milliseconds, threadType, threadIndex, msg, *item);
}

void PRINT_MATRIX(int **matrix, int n, int m, const char *name)
{
    printf("\n| Matrix %s\n", name);
    // Find the maximum number of digits in the matrix
    int maxDigits = 1;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            int num = matrix[i][j];
            int digits = 1;
            while (num /= 10)
            {
                digits++;
            }
            if (digits > maxDigits)
            {
                maxDigits = digits;
            }
        }
    }

    // Print the matrix with organized spacing
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            printf("%*d ", maxDigits, matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void GENERATE_MATRIX(int ***matrix, int n, int m, const char *name)
{
    // Generate matrix
    *matrix = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
        (*matrix)[i] = (int *)malloc(m * sizeof(int));

    if (matrix == NULL)
        perror("Memory allocation for matric failed.\n");

    // Fill matrix with random numbers
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            (*matrix)[i][j] = rand() % 10;

    // Pretty print matrix
    PRINT_MATRIX(*matrix, n, m, name);
}

void CLEAR_MATRIX(int ***matrix, int n)
{
    for (int i = 0; i < n; i++)
        free((*matrix)[i]);
    free(*matrix);
}