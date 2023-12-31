/*  
    GACEM ABDELMOUNAIM
    GROUPE: 1
*/

#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define MAX 100 

//variable globales
int n1, m1, n2, m2;

int N;//Taille du tampon

//les matrices
int B[MAX][MAX];
int C[MAX][MAX];
int A[MAX][MAX];

//le tampon
int *T; 

//pour la synchronisation 
pthread_mutex_t mutex;
sem_t empty;
sem_t full;


// Fonction pour imprimer les matrices
void printMatrix(int rows, int cols, int matrix[MAX][MAX]);
// Producteur
void *producer(void *args)
{
    int row = *(int *)args;
    int somme;

    printf("Producteur #%d DÉMARRÉ.\n", row + 1);

    for (int col = 0; col < m2; col++){
        somme = 0;

        for (int i = 0; i < m1; i++){
            somme += B[row][i] * C[i][col];
        }
        // Attendre un emplacement vide dans le tampon
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        T[row * m2 + col] = somme;

        pthread_mutex_unlock(&mutex);
        sem_post(&full);

        printf("Producteur #%d a mis: %d\n", row + 1, somme);
        usleep(10000);  // pour une meilleure visualisation
    }

    free(args);
    printf("Producteur #%d TERMINÉ.\n", row + 1);
    pthread_exit(NULL);
}


void *consumer(void *args)
{
    int col = *(int *)args;
    int result;

    printf("Consommateur #%d DÉMARRÉ.\n", col + 1);

    for (int row = 0; row < n1; row++)
    {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        // attente tant que la case du tampon est vide (-1)
        while (T[row * m2 + col] == -1) {
            pthread_mutex_unlock(&mutex);
            usleep(100);  // pour éviter l'attente active
            pthread_mutex_lock(&mutex);
        }

        result = T[row * m2 + col];
        T[row * m2 + col] = -1;  // Marquer la case consommée comme vide

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);

        A[row][col] = result;

        printf("Consommateur #%d a obtenu: %d\n", col + 1, result);
        usleep(10000);  // pour une meilleure visualisation 
    }

    free(args);
    printf("Consommateur #%d TERMINÉ.\n", col + 1);
    pthread_exit(NULL);
}


int main ()
{
    printf("Entrez la taille du tampon (e.g : 100) : ");
    if (scanf("%d", &N) != 1 || N <= 0){
        fprintf(stderr, "Erreur : la taille du tampon doit être un entier positif.\n");
        return 1;
    }

// Allocation mémoire pour le tampon.. 
    T = malloc(N * N * sizeof(int));// a 1D array as if it were a 2D array.
    if (T == NULL){
        perror("Erreur d'allocation mémoire pour T.\n");
        return 1;
    }
// Pour indiquer les emplacements vides
    for (int i = 0; i < N * N; i++) {
        T[i] = -1;
    }

    
    printf("Entrez le nombre de lignes et de colonnes de la matrice B (lignes colonnes) : ");
    if (scanf("%d %d", &n1, &m1) != 2 || n1 <= 0 || m1 <= 0){
        fprintf(stderr, "Erreur : entrée non valide pour les dimensions de la matrice B.\n");
        return 1;
    }
    printf("Entrez le nombre de lignes et de colonnes de la matrice C (lignes colonnes) : ");
    if (scanf("%d %d", &n2, &m2) != 2 || n2 <= 0 || m2 <= 0){
        fprintf(stderr, "Erreur : entrée non valide pour les dimensions de la matrice C.\n");
        return 1;
    }

// Vérifications pour des dimensions valides
    if (n1 <= 0 || m1 <= 0 || n2 <= 0 || m2 <= 0){
        fprintf(stderr, "Erreur : les dimensions de la matrice doivent être des entiers positifs.\n");
        return 1;
    }

    if (m1 != n2){
        fprintf(stderr, "Erreur : le nombre de colonnes de B doit être égal au nombre de lignes de C.\n");
        return 1;
    }

// Initialiser les matrices B et C avec des valeurs aléatoires
    for (int i = 0; i < n1; i++){
        for (int j = 0; j < m1; j++) {
            B[i][j] = rand() % 10;
        }
    }

    for (int i = 0; i < n2; i++){
        for (int j = 0; j < m2; j++){
            C[i][j] = rand() % 10;
        }
    }
    // Afficher B & C
    printf("\n Matrice B : \n");
    printMatrix(n1, m1, B);
    printf("\n Matrice C : \n");
    printMatrix(n2, m2, C);
    printf("\n");
    printf("**Donnez au tampon une taille plus grande si l'exécution s'arrête sans terminer.**\n \n");

// Initialisation
    pthread_t producers[n1], consumers[m2];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0 , N);  
    sem_init(&full, 0 , 0);   
    //creation des threads
    for (int i = 0; i < n1; i++){
        int *rowIndex = malloc(sizeof(int));
        if (rowIndex == NULL){
            perror("Erreur : échec de l'allocation mémoire pour le paramètre du thread.\n");
            return 1;
        }
        *rowIndex = i;
        if (pthread_create(&producers[i], NULL, producer, (void *)rowIndex) != 0){
            perror("Erreur : échec de la création du thread.\n");
            return 1;
        }
    }

    for (int i = 0; i < m2; i++){
        int *colIndex = malloc(sizeof(int));
        if (colIndex == NULL){
            perror("Erreur : échec de l'allocation mémoire pour le paramètre du thread.\n");
            return 1;
        }
        *colIndex = i;
        if (pthread_create(&consumers[i], NULL, consumer, (void *)colIndex) != 0){
            perror("Erreur : échec de la création du thread.\n");
            return 1;
        }
    }



//attente des threads
    for (int i = 0; i < n1; i++){
        pthread_join(producers[i], NULL);
    }

    for (int i = 0; i < m2; i++){
        pthread_join(consumers[i], NULL);
    }
    printf("\n Matrice A : \n");
    for (int i = 0; i < n1; i++){
        for (int j = 0; j < m2; j++){
            printf("%-4d", A[i][j]);
        }
        printf("\n");
    }


//destruction...
    pthread_mutex_destroy(&mutex);
    sem_destroy(&full);
    sem_destroy(&empty);

    free(T);



    return 0;
}

void printMatrix(int rows, int cols, int matrix[N][N]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%-4d ", matrix[i][j]);  // j'ai utilisé %-4d pour l'alignement à gauche
        }
        printf("\n");
    }
}



// *** All comments have been translated TO French ***


/*
>OUTPUT:

Entrez la taille du tampon (e.g : 100) : 100
Entrez le nombre de lignes et de colonnes de la matrice B (lignes colonnes) : 5 12
Entrez le nombre de lignes et de colonnes de la matrice C (lignes colonnes) : 12 4

 Matrice B : 
3    6    7    5    3    5    6    2    9    1    2    7    
0    9    3    6    0    6    2    6    1    8    7    9    
2    0    2    3    7    5    9    2    2    8    9    7    
3    6    1    2    9    3    1    9    4    7    8    4    
5    0    3    6    1    0    6    3    2    0    6    1    

 Matrice C : 
5    5    4    7    
6    5    6    9    
3    7    4    5    
2    5    4    7    
4    4    3    0    
7    8    6    8    
8    4    3    1    
4    9    2    0    
6    8    9    2    
6    6    4    9    
5    0    4    8    
7    1    7    2    

**Donnez au tampon une taille plus grande si l'exécution s'arrête sans terminer.**
 
Producteur #1 DÉMARRÉ.
Producteur #1 a mis: 304
Producteur #2 DÉMARRÉ.
Producteur #2 a mis: 309
Producteur #4 DÉMARRÉ.
Producteur #4 a mis: 293
Producteur #3 DÉMARRÉ.
Producteur #3 a mis: 319
Consommateur #1 DÉMARRÉ.
Consommateur #1 a obtenu: 304
Consommateur #2 DÉMARRÉ.
Producteur #5 DÉMARRÉ.
Producteur #5 a mis: 159
Consommateur #3 DÉMARRÉ.
Consommateur #4 DÉMARRÉ.
Producteur #1 a mis: 298
Producteur #4 a mis: 285
Producteur #2 a mis: 271
Producteur #3 a mis: 232
Consommateur #1 a obtenu: 309
Producteur #5 a mis: 148
Consommateur #2 a obtenu: 298
Producteur #1 a mis: 299
Producteur #4 a mis: 250
Producteur #2 a mis: 276
Producteur #3 a mis: 245
Consommateur #1 a obtenu: 319
Consommateur #3 a obtenu: 299
Producteur #5 a mis: 132
Consommateur #2 a obtenu: 271
Producteur #1 a mis: 248
Producteur #4 a mis: 262
Producteur #3 a mis: 256
Producteur #2 a mis: 336
Consommateur #3 a obtenu: 276
Consommateur #1 a obtenu: 293
Producteur #5 a mis: 152
Consommateur #4 a obtenu: 248
Consommateur #2 a obtenu: 232
Producteur #4 TERMINÉ.
Producteur #3 TERMINÉ.
Producteur #2 TERMINÉ.
Producteur #1 TERMINÉ.
Consommateur #1 a obtenu: 159
Consommateur #3 a obtenu: 245
Consommateur #2 a obtenu: 285
Producteur #5 TERMINÉ.
Consommateur #4 a obtenu: 336
Consommateur #1 TERMINÉ.
Consommateur #3 a obtenu: 250
Consommateur #2 a obtenu: 148
Consommateur #4 a obtenu: 256
Consommateur #3 a obtenu: 132
Consommateur #2 TERMINÉ.
Consommateur #4 a obtenu: 262
Consommateur #3 TERMINÉ.
Consommateur #4 a obtenu: 152
Consommateur #4 TERMINÉ.

 Matrice A : 
304 298 299 248 
309 271 276 336 
319 232 245 256 
293 285 250 262 
159 148 132 152 

*/
