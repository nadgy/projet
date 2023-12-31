/*
 * Ahcene Guebli - Rafik Alim - G3 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define N 10

int **A, **B, **C, n1, m1, n2, m2, T[N], **POS;
int comp_p = 0, comp_c = 0, comp_t = 0;

pthread_mutex_t mutex;
sem_t empty, full;

void *producer(void *arg)
{
    int i = *(int *)arg;
    int s = 0;

    for (int j = 0; j < m2; j++)
    {
        s = 0;
        for (int k = 0; k < m1; k++)
        {
            s += B[i][k] * C[k][j];
        }

        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        T[comp_p] = s;
        POS[comp_p][0] = i;
        POS[comp_p][1] = j;

        comp_p = (comp_p + 1) % N;

        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }

    free(arg);
    pthread_exit(NULL);
}

void *consumer()
{
    while (comp_t < m2 * n1)
    {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        A[POS[comp_c][0]][POS[comp_c][1]] = T[comp_c];
        comp_c = (comp_c + 1) % N;
        comp_t++;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    sem_init(&empty, 0, N);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);
    srand(time(NULL));

    if (argc != 5)
    {
        printf("Usage: %s n1 m1 n2 m2\n", argv[0]);
        exit(1);
    }

    n1 = atoi(argv[1]);
    m1 = atoi(argv[2]);
    n2 = atoi(argv[3]);
    m2 = atoi(argv[4]);

    if (m1 != n2)
    {
        printf("Error: m1 != n2\n");
        exit(1);
    }

    A = (int **)malloc(n1 * sizeof(int *));
    for (int i = 0; i < n1; i++)
    {
        A[i] = (int *)malloc(m2 * sizeof(int));
    }

    POS = (int **)malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++)
    {
        POS[i] = (int *)malloc(2 * sizeof(int));
    }

    B = (int **)malloc(n1 * sizeof(int *));
    for (int i = 0; i < n1; i++)
    {
        B[i] = (int *)malloc(m1 * sizeof(int));
        for (int j = 0; j < m1; j++)
        {
            B[i][j] = rand() % 10;
        }
    }

    C = (int **)malloc(n2 * sizeof(int *));
    for (int i = 0; i < n2; i++)
    {
        C[i] = (int *)malloc(m2 * sizeof(int));
        for (int j = 0; j < m2; j++)
        {
            C[i][j] = rand() % 10;
        }
    }

    printf("B = \n");
    for (int i = 0; i < n2; i++)
    {
        for (int j = 0; j < m2; j++)
        {
            printf("%d ", B[i][j]);
        }
        printf("\n");
    }

    printf("\n");

    printf("C = \n");
    for (int i = 0; i < m2; i++)
    {
        for (int j = 0; j < n2; j++)
        {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }

    printf("\n");

    pthread_t producer_t[N];
    pthread_t consumer_t;

    for (int i = 0; i < n1; i++)
    {
        int *ligne = malloc(sizeof(int));
        *ligne = i;
        pthread_create(&producer_t[i], NULL, producer, ligne);
    }

    pthread_create(&consumer_t, NULL, consumer, NULL);

    for (int i = 0; i < n1; i++)
    {
        pthread_join(producer_t[i], NULL);
    }

    pthread_join(consumer_t, NULL);

    printf("A = \n");
    for (int i = 0; i < n1; i++)
    {
        for (int j = 0; j < m2; j++)
        {
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    for (int i = 0; i < n1; i++)
    {
        free(A[i]);
        free(B[i]);
    }
    free(A);
    free(B);

    for (int i = 0; i < n2; i++)
    {
        free(C[i]);
    }
    free(C);

    for (int i = 0; i < N; i++)
    {
        free(POS[i]);
    }
    free(POS);

    return 0;
}
