//KOROGHLI Mouna G01 

//section include..
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

//define
#define N 10 // places dans le buffer
#define TH 10 //nombre de threads
//variable globales
#define nB 3  // Taille de la première dimension de la matrice B
#define mB 3  // Taille de la deuxième dimension de la matrice B
#define nC 3  // Taille de la première dimension de la matrice C
#define mC 3  // Taille de la deuxième dimension de la matrice C
int cnt = 0;

//les matrices
 int B[nB][mB], C[nC][mC], A[nB][mC];

//le tampon
int T[N];

//pour la synchronisation 
pthread_mutex_t mutex;
sem_t empty;
sem_t full;

// Producteur
void *producer(void *args)
{
    int row = *(int *)args;
//pour chaque ligne
//for....
    for (int col = 0; col < mC; col++)
    {  
        int somme = 0;
        for (int i = 0; i < mB; i++)
        {
            somme += B[row][i] * A[i][col];
        }
        sem_wait(&empty);// attente d'une place libre
        pthread_mutex_lock(&mutex);
        T[cnt] = somme;
        cnt ++;
        pthread_mutex_unlock(&mutex);
        sem_post(&full); // il y a une place remplie en plus
    }
    free(args);
    pthread_exit(NULL);
}

//Consommateur
void *consumer(void *args)
{
    int col = *(int *)args;
    int resultat;

    for (int row = 0; row < nB; row++)
    {
        sem_wait(&full);// attente d'une place remplie
        pthread_mutex_lock(&mutex);
        cnt --;
        resultat = T[cnt];
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);// il y a une place libre en plus
        C[row][col] = resultat;
    }
    free(args);
    pthread_exit(NULL);
}

int main()
{
 
 //Remplir les matrices
    for (int i = 0; i < nB; i++)
    {
        for (int j = 0; j < mB; j++)
        {
            B[i][j] = rand() % 10;
        }
    }

    for (int i = 0; i < nC; i++)
    {
        for (int j = 0; j < mC; j++)
        {
            A[i][j] = rand() % 10; 
        }
    }
    pthread_t producers[TH], consumers[TH];
    
//Initialisation
   pthread_mutex_init(&mutex, NULL);//exclusion mutuelles
   sem_init(&empty, 0 , N);  // buffer vide
   sem_init(&full, 0 , 0);   // buffer vide

//Affichage des matrices   
    printf("La Matrice A:\n");
    for (int i = 0; i < nB; i++)
    {
        for (int j = 0; j < mC; j++)
        {
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }
    printf("La Matrice B:\n");
    for (int i = 0; i < nB; i++)
    {
        for (int j = 0; j < mC; j++)
        {
            printf("%d ", B[i][j]);
        }
        printf("\n");
    }

  //Creation des threads
    for (int i = 0; i < nB; i++)
    {
        int *p = malloc(sizeof(int));
        *p = i;
        pthread_create(&producers[i], NULL, producer, (void *)p);
    }
    for (int i = 0; i < mC; i++)
    {
        int *p = malloc(sizeof(int));
        *p = i;
        pthread_create(&consumers[i], NULL, consumer, (void *)p);
    }

// Attente des threads 
    for (int i = 0; i < nB; i++)
    {
        pthread_join(producers[i], NULL);
    }
    for (int i = 0; i < mC; i++)
    {
        pthread_join(consumers[i], NULL);
    }

  //Afficher la matrice C 
    printf("La Matrice C:\n");
    for (int i = 0; i < nB; i++)
    {
        for (int j = 0; j < mC; j++)
        {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }
  
//destruction...
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
