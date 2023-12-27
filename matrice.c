// Includes
// SETTI MOHAMED
// GROUP 3
// EMAIL : settim453@gmail.com
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

// Global variables
int n1, m1, n2, m2;
int **A;
int **B;
int **C;
int shouldExit = 0;

// Buffer
#define N 5 // Max number of items in buffer
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

  for (int i = 0; i < m2; i++)
  {
    item = 0;
    for (int j = 0; j < m1; j++)
      item += B[index][j] * C[j][i];

    // Wait for an empty slot in the buffer
    sem_wait(&empty);
    // Lock the buffer for modification
    pthread_mutex_lock(&mutex);

    // Produce item into the shared buffer
    sharedBuffer.data[sharedBuffer.in] = item;
    sharedBuffer.posX[sharedBuffer.in] = index;
    sharedBuffer.posY[sharedBuffer.in] = i;
    sharedBuffer.in = (sharedBuffer.in + 1) % N;
    sharedBuffer.count++;
    sharedBuffer.produced++;

    // Unlock the buffer
    pthread_mutex_unlock(&mutex);
    // Signal that the buffer is no longer empty
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
    // Wait for a full slot in the buffer
    sem_wait(&full);
    // Lock the buffer for modification
    pthread_mutex_lock(&mutex);

    if (shouldExit)
    {
      // Unlock the buffer and break if the exit condition is met
      pthread_mutex_unlock(&mutex);
      break;
    }

    // Consume item from the shared buffer
    item = sharedBuffer.data[sharedBuffer.out];
    A[sharedBuffer.posX[sharedBuffer.out]][sharedBuffer.posY[sharedBuffer.out]] = item;
    sharedBuffer.out = (sharedBuffer.out + 1) % N;
    sharedBuffer.count--;
    sharedBuffer.consumed++;

    // Check the exit condition
    shouldExit = sharedBuffer.consumed == n1 * m2;
    if (shouldExit)
      for (int i = 0; i < N; i++)
        // Signal that the buffer is no longer full
        sem_post(&full);

    // Unlock the buffer
    pthread_mutex_unlock(&mutex);
    // Signal that the buffer is no longer empty
    sem_post(&empty);
  }

  pthread_exit(NULL);
}

// Generate random matrices
void generateMatrices(void)
{
  // Generate matrix B
  B = (int **)malloc(n1 * sizeof(int *));
  for (int i = 0; i < n1; i++)
    B[i] = (int *)malloc(m1 * sizeof(int));

  if (B == NULL)
    perror("Memory allocation for matrix B failed.\n");

  // Generate matrix C
  C = (int **)malloc(n2 * sizeof(int *));
  for (int i = 0; i < n2; i++)
    C[i] = (int *)malloc(m2 * sizeof(int));

  if (C == NULL)
    perror("Memory allocation for matrix C failed.\n");

  // Fill matrix B with random numbers
  for (int i = 0; i < n1; i++)
    for (int j = 0; j < m1; j++)
      B[i][j] = rand() % 10;

  // Fill matrix C with random numbers
  for (int i = 0; i < n2; i++)
    for (int j = 0; j < m2; j++)
      C[i][j] = rand() % 10;

  // Pretty print both matrices
  printf("\n| Matrix B\n");
  for (int i = 0; i < n1; i++)
  {
    for (int j = 0; j < m1; j++)
      printf("%d  ", B[i][j]);
    printf("\n");
  }
  printf("\n| Matrix C\n");
  for (int i = 0; i < n2; i++)
  {
    for (int j = 0; j < m2; j++)
      printf("%d  ", C[i][j]);
    printf("\n");
  }
  printf("\n\n");
}

int main(int argc, char *argv[])
{
  // Seed the random number generator
  srand(time(NULL));

  // Ask the user for n1, m1 for B[n1][m1] and n2, m2 for C[n2][m2]
  printf("Enter the number of rows and columns in matrix B: ");
  scanf("%d %d", &n1, &m1);
  printf("Enter the number of rows and columns in matrix C: ");
  scanf("%d %d", &n2, &m2);

  // Make sure that multiplication is possible
  if (m1 != n2)
  {
    perror("Multiplication of B and C is not possible.\n");
    exit(EXIT_FAILURE);
  }

  // Generate matrices
  generateMatrices();

  // Initialize the shared buffer
  sharedBuffer.data = (int *)malloc(N * sizeof(int));
  sharedBuffer.posX = (int *)malloc(N * sizeof(int));
  sharedBuffer.posY = (int *)malloc(N * sizeof(int));
  sharedBuffer.count = 0;
  sharedBuffer.in = 0;
  sharedBuffer.out = 0;
  sharedBuffer.produced = 0;
  sharedBuffer.consumed = 0;

  // Initialize matrix A
  A = (int **)malloc(n1 * sizeof(int *));
  for (int i = 0; i < n1; i++)
  {
    A[i] = (int *)malloc(m2 * sizeof(int));
  }
  if (A == NULL)
    perror("Memory allocation for matrix A failed.\n");

  // Initialize semaphores and mutex
  if (pthread_mutex_init(&mutex, NULL) != 0)
  {
    perror("Mutex initialization failed.\n");
    exit(EXIT_FAILURE);
  }
  if (sem_init(&empty, 0, N) != 0)
  {
    perror("Semaphore [empty] initialization failed.\n");
    exit(EXIT_FAILURE);
  }
  if (sem_init(&full, 0, 0) != 0)
  {
    perror("Semaphore [full] initialization failed.\n");
    exit(EXIT_FAILURE);
  }

  // Declare threads
  pthread_t producers_t[n1];
  pthread_t consumers_t[N];

  // Create producer threads
  for (int i = 0; i < n1; i++)
  {
    int *index = malloc(sizeof(int));
    *index = i;
    if (pthread_create(&producers_t[i], NULL, producer, index) != 0)
    {
      perror("Producer thread creation failed.\n");
      exit(EXIT_FAILURE);
    }
  }

  // Create consumer threads
  for (int i = 0; i < N; i++)
  {
    int *index = malloc(sizeof(int));
    *index = i;
    if (pthread_create(&consumers_t[i], NULL, consumer, index) != 0)
    {
      perror("Consumer thread creation failed.\n");
      exit(EXIT_FAILURE);
    }
  }

  // Join producer threads
  for (int i = 0; i < n1; i++)
    pthread_join(producers_t[i], NULL);

  // Join consumer threads
  for (int i = 0; i < N; i++)
    pthread_join(consumers_t[i], NULL);

  // Pretty print matrix A
  printf("\n| Matrix A\n");
  for (int i = 0; i < n1; i++)
  {
    for (int j = 0; j < m2; j++)
      printf("%d  ", A[i][j]);
    printf("\n");
  }

  // Cleanup
  pthread_mutex_destroy(&mutex);
  sem_destroy(&empty);
  sem_destroy(&full);
  for (int i = 0; i < n1; i++)
  {
    free(A[i]);
  }
  free(A);
  for (int i = 0; i < n1; i++)
  {
    free(B[i]);
  }
  free(B);
  for (int i = 0; i < n1; i++)
  {
    free(C[i]);
  }
  free(C);
  free(sharedBuffer.data);
  free(sharedBuffer.posX);
  free(sharedBuffer.posY);

  return 0;
}
