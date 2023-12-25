#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define N ... // Taille du tampon
#define n1 ... // Nombre de lignes de B
#define m1 ... // Nombre de colonnes de B (également nombre de lignes de C)
#define n2 ... // Nombre de colonnes de C

// Matrices
double B[n1][m1], C[m1][n2], A[n1][n2];
// Tampon
double T[N];

// Synchronisation
pthread_mutex_t mutex;
sem_t empty;
sem_t full;

// Producteur
void producer(void)
{
    int row;
    while (1)
    {
        // Obtenir la prochaine ligne à calculer
        pthread_mutex_lock(&mutex);
        row = getNextRow();
        pthread_mutex_unlock(&mutex);

        // Vérifier la condition de terminaison
        if (row == -1)
            break;

        // Calculer la ligne de la matrice résultante A et la stocker dans le tampon T
        for (int col = 0; col < n2; ++col)
        {
            double result = 0.0;
            for (int i = 0; i < m1; ++i)
            {
                result += B[row][i] * C[i][col];
            }
            T[col] = result;
        }

        sem_wait(&empty); // attente d'une place libre
        pthread_mutex_lock(&mutex);
        // section critique
        insert_item(row, T);
        pthread_mutex_unlock(&mutex);
        sem_post(&full); // il y a une place remplie en plus
    }
}

// Consommateur
void consumer(void)
{
    int row;
    double resultRow[n2];

    while (1)
    {
        sem_wait(&full); // attente d'une place remplie
        pthread_mutex_lock(&mutex);
        // section critique
        row = remove_item(resultRow);
        pthread_mutex_unlock(&mutex);
        sem_post(&empty); // il y a une place libre en plus

        // Placer la ligne résultante dans la matrice résultante A
        for (int col = 0; col < n2; ++col)
        {
            A[row][col] = resultRow[col];
        }

        // Vérifier la condition de terminaison et quitter si nécessaire
        if (row == -1)
            break;
    }
}

int main()
{
    // Initialisation des matrices B et C avec des valeurs aléatoires
    // ...

    // Initialisation
    sem_init(&empty, 0, N);  // buffer vide
    sem_init(&full, 0, 0);   // buffer vide
    pthread_mutex_init(&mutex, NULL); // exclusion mutuelle

    // Création des threads producteurs
    pthread_t producers[n1];
    for (int i = 0; i < n1; ++i)
    {
        pthread_create(&producers[i], NULL, producer, NULL);
    }

    // Création des threads consommateurs
    pthread_t consumers[n1];
    for (int i = 0; i < n1; ++i)
    {
        pthread_create(&consumers[i], NULL, consumer, NULL);
    }

    // Attente des threads producteurs
    for (int i = 0; i < n1; ++i)
    {
        pthread_join(producers[i], NULL);
    }

    // Indiquer aux consommateurs que la production est terminée
    for (int i = 0; i < n1; ++i)
    {
        sem_post(&full);
    }

    // Attente des threads consommateurs
    for (int i = 0; i < n1; ++i)
    {
        pthread_join(consumers[i], NULL);
    }

    // Destruction des ressources
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}

