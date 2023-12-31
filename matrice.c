#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define MAX_ROWS 10
#define MAX_COLUMNS 10
#define N 10  // Size of the buffer
#define MAX_THREADS 10

// Node structure for the linked list
struct Node
{
    int data;
    int row;
    struct Node *next;
};

// Global variables
int n1 = MAX_ROWS, n2 = MAX_ROWS, m1 = MAX_COLUMNS, m2 = MAX_COLUMNS;

// Matrices
int B[MAX_ROWS][MAX_COLUMNS], C[MAX_ROWS][MAX_COLUMNS], A[MAX_ROWS][MAX_COLUMNS];

// Buffer
struct Node *head = NULL;
struct Node *tail = NULL;

// For synchronization
pthread_mutex_t mutex;
sem_t empty;
sem_t full;

// Function to check if the buffer is empty
int is_empty()
{
    return head == NULL;
}

// Function to check if the buffer is full
int is_full()
{
    // Assuming N is the maximum size of the buffer
    return count_elements_in_buffer() >= N;
}

// Function to count the number of elements in the buffer
int count_elements_in_buffer()
{
    int count = 0;
    struct Node *current = head;

    while (current != NULL)
    {
        count++;
        current = current->next;
    }

    return count;
}

// Function to insert an item into the buffer
void insert_item(int item, int row)
{
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    new_node->data = item;
    new_node->row = row;
    new_node->next = NULL;

    if (is_empty())
    {
        head = new_node;
        tail = new_node;
    }
    else
    {
        tail->next = new_node;
        tail = new_node;
    }
}

// Function to remove an item from the buffer
void remove_item(int *item, int *row)
{
    if (!is_empty())
    {
        struct Node *temp = head;
        *item = temp->data;
        *row = temp->row;
        head = temp->next;

        free(temp);
    }
}

void initialize_matrices()
{
    srand(time(NULL)); // Seed for random number generation

    for (int i = 0; i < n1; ++i)
    {
        for (int j = 0; j < m1; ++j)
        {
            B[i][j] = rand() % 10;
        }
    }

    for (int i = 0; i < n2; ++i)
    {
        for (int j = 0; j < m2; ++j)
        {
            C[i][j] = rand() % 10;
        }
    }
}


void multiplyMatrices(int B[MAX_ROWS][MAX_COLUMNS], int C[MAX_ROWS][MAX_COLUMNS], int result[MAX_ROWS][MAX_COLUMNS]) {
    for (int i = 0; i < n1; ++i) {
        for (int j = 0; j < m1; ++j) {
            result[i][j] = 0;
            for (int k = 0; k < n2; ++k) {
                result[i][j] += B[i][k] * C[k][j];
            }
        }
    }
}



// producer function
void *producer(void *arg) {
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);

    multiplyMatrices(B, C, A);

    pthread_mutex_unlock(&mutex);
    sem_post(&full);
    pthread_exit(NULL);
}

// Function to consume the item and row
void consume(int item, int row)
{
    for (int j = 0; j < m1; ++j)
    {
        A[row][j] = item;
    }
}

// Consumer
void *consumer(void *arg)
{
    int item, row;
    while (1) // Run continuously
    {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        if (is_empty()) // Check if the buffer is empty
        {
            pthread_mutex_unlock(&mutex);
            sem_post(&empty);
            break; // Exit the loop if the buffer is empty
        }
        remove_item(&item, &row);
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        consume(item, row);
    }
}

int main()
{
    // Initialize matrices B and C, set n1, n2, m1, m2 values
    initialize_matrices();
    // Print Matrix B
    printf("Matrix B:\n");
    for (int i = 0; i < n1; ++i)
    {
        for (int j = 0; j < m1; ++j)
        {
            printf("%d ", B[i][j]);
        }
        printf("\n");
    }

    // Print Matrix C
    printf("Matrix C:\n");
    for (int i = 0; i < n2; ++i)
    {
        for (int j = 0; j < m2; ++j)
        {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }

    // Initialize mutex and semaphores
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, N);
    sem_init(&full, 0, 0);

    // Create producer and consumer threads
    pthread_t producer_thread, consumer_thread;

    // Add code to set n1, n2, m1, m2 values and initialize matrices B and C

    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // Join threads
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // Print the resulting matrix A
    printf("Resulting Matrix A:\n");
    for (int i = 0; i < n1; ++i)
    {
        for (int j = 0; j < m1; ++j)
        {
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }

    // Cleanup
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
