/* %%%%% AMOURI %%%%%%%%% YACINE %%%%%%% GROUPE 3 %%%%%%%% L3 %%%%%%*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define N 100

int n1, m1, n2, m2;
int **A, **B, **C;
int *BUFFER;

pthread_mutex_t mutex;
sem_t empty, full;

void *producer(void *arg)
{
    int index = *(int *)arg;
    int somme;

    for (int i = 0; i < m2; i++)
    {
        somme = 0;
        for (int j = 0; j < m1; j++)
        {
            somme += B[index][j] * C[j][i];
        }

        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        

        BUFFER[index * m2 + i] = somme;

        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }

    pthread_exit(NULL);
}

void *consumer(void *arg)
{
    int index = *(int *)arg;

    for (int i = 0; i < m2; i++)
    {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        // Utilisez (index * m2 + i) pour accéder à la bonne position dans BUFFER
        A[index][i] = BUFFER[(index * m2 + i) % (n1 * m2)];

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }

    pthread_exit(NULL);
}

void generatorMatrices()
{
    B = (int **)malloc(n1 * sizeof(int *));
    C = (int **)malloc(m1 * sizeof(int *));
    for (int i = 0; i < n1; i++)
    {
        B[i] = (int *)malloc(m1 * sizeof(int));
    }
    for (int i = 0; i < m1; i++)
    {
        C[i] = (int *)malloc(m2 * sizeof(int));
    }
    //Remplire matrices B et C
    for (int i = 0; i < n1; i++)
    {
        for (int j = 0; j < m1; j++)
        {
            B[i][j] = rand() % 10;
        }
    }

    for (int i = 0; i < m1; i++)
    {
        for (int j = 0; j < m2; j++)
        {
            C[i][j] = rand() % 10;
        }
    }

    BUFFER = (int *)malloc(n1 * m2 * sizeof(int));
}

void printMatrices()
{
//Afficher les matrices B et C
    printf("\n| Matrice B\n");
    for (int i = 0; i < n1; i++)
    {
        for (int j = 0; j < m1; j++)
        {
            printf("%d  ", B[i][j]);
        }
        printf("\n");
    }

    printf("\n| Matrice C\n");
    for (int i = 0; i < m1; i++)
    {
        for (int j = 0; j < m2; j++)
        {
            printf("%d  ", C[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
// intialise les sempahore
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, N);
    pthread_mutex_init(&mutex, NULL);

    printf("Entre nombre des lignes et colognes de matrice B:\n");
    scanf("%d %d", &n1, &m1);
    printf("Entre nombre des lignes et colognes de matrice C\n");
    scanf("%d %d", &n2, &m2);

    if (m1 != n2)
    {
        perror("impossible de multiple  B et C.\n");
        exit(EXIT_FAILURE);
    }

    generatorMatrices();
    printMatrices();

    A = (int **)malloc(n1 * sizeof(int *));
    for (int i = 0; i < n1; i++)
    {
        A[i] = (int *)malloc(m2 * sizeof(int));
    }

    pthread_t producers_t[n1];
    pthread_t consumer_t[n1];

    for (int i = 0; i < n1; i++)
    {
        int *index = malloc(sizeof(int));
        *index = i;
        if (pthread_create(&producers_t[i], NULL, producer, index) != 0)
        {
            perror("Erreur de creation thread de producer.\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < n1; i++)
    {
        int *index = malloc(sizeof(int));
        *index = i;
        if (pthread_create(&consumer_t[i], NULL, consumer, index) != 0)
        {
            perror("Erreur de creation thread de consumer.\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < n1; i++)
    {
        pthread_join(producers_t[i], NULL);
    }

    for (int i = 0; i < n1; i++)
    {
        pthread_join(consumer_t[i], NULL);
    }

    printf("\n| Matrice A\n");
    for (int i = 0; i < n1; i++)
    {
        for (int j = 0; j < m2; j++)
        {
            printf("%d  ", A[i][j]);
        }
        printf("\n");
    }

   
    free(BUFFER);

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
