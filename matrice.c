#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>


#define TAILLE_MAX 50

#define THREAD_MAX 50

int A[TAILLE_MAX][TAILLE_MAX], B[TAILLE_MAX][TAILLE_MAX], C[TAILLE_MAX][TAILLE_MAX];
// Dimensions of matrices B and C
int n1, m1, n2, m2;

pthread_mutex_t mutex;
sem_t zero;
sem_t lob;

int iNDEX_BUFFER = 0;
int *RESULTAT_BUFFER;

// Function to calculate the product of two matrix ligne
int produit_DEUX_LIGNES(int ligne_B[TAILLE_MAX], int col_C[TAILLE_MAX]) {
    int resultat = 0;
    for (int i = 0; i < TAILLE_MAX; i++) {
        resultat += ligne_B[i] * col_C[i];
    }
    return resultat;
}

// Producer
void *producteur_MATRICE(void *arg) {
    int row = *(int *)arg;
    for (int i = 0; i < m2; i++) {
        int resultat = produit_DEUX_LIGNES(B[row], C[i]);
        sem_wait(&zero);
        pthread_mutex_lock(&mutex);
        RESULTAT_BUFFER[iNDEX_BUFFER] = resultat;
        iNDEX_BUFFER = (iNDEX_BUFFER + 1) % (n1 * m2);
        pthread_mutex_unlock(&mutex);
        sem_post(&lob);
    }
    return NULL;
}

// Consomateur
void *consomateur(void *arg) {
    int all_elements = n1 * m2;
    int cons_elements = 0;
    while (cons_elements < all_elements) {
        sem_wait(&lob);
        pthread_mutex_lock(&mutex);
        int resultat = RESULTAT_BUFFER[iNDEX_BUFFER];
        iNDEX_BUFFER = (iNDEX_BUFFER + 1) % (n1 * m2);
        pthread_mutex_unlock(&mutex);
        sem_post(&zero);
        // Place the resultat in matrix A
        int row = cons_elements / m2;
        int col = cons_elements % m2;
        A[row][col] = resultat;
        cons_elements++;
    }
    return NULL;
}

// Function to fill matrices with random values
void remplissage(int matrix[TAILLE_MAX][TAILLE_MAX], int ligne, int colonne) {
    for (int i = 0; i < ligne; i++) {
        for (int j = 0; j < colonne; j++) {
            matrix[i][j] = rand() % 10;
        }
    }
}

int main() {
    printf("Enter values for these following variables :\n");
    printf("   Lines of matrix B: ");
    scanf("%d", &n1);
    printf("\n");
    printf("   Columns of matrix B: ");
    scanf("%d", &m1);
    printf("\n");printf("\n");
    printf("   Lines of matrix C: ");
    scanf("%d", &n2);
    printf("   Columns of matrix C: ");
    scanf("%d", &m2);
    printf("\n");
    printf("\n");
    if (m1 != n2) {
        printf("Error, Matrix dimensions do not allow multiplication.\n");
        return -1;
    }

    // User information
    printf("Both matrices are filled with random values.\n");
    printf("Matrix A is the resultat of the multiplication of these two matrices.\n");

    // Allocate space for the buffer
    RESULTAT_BUFFER = (int *)malloc(n1 * m2 * sizeof(int));

    // Fill matrices B and C with random values
    remplissage(B, n1, m1);
    remplissage(C, n2, m2);

    pthread_t prod_THREAD[THREAD_MAX];
    pthread_t cons_THREAD;
    int prod_ARGS[THREAD_MAX];

    // Initialize semaphores and mutex
    sem_init(&zero, 0, n1 * m2);
    sem_init(&lob, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    // Create producer threads
    for (int i = 0; i < n1; i++) {
        prod_ARGS[i] = i;
        pthread_create(&prod_THREAD[i], NULL, producteur_MATRICE, &prod_ARGS[i]);
    }

    // Create consomateur thread
    pthread_create(&cons_THREAD, NULL, consomateur, NULL);

    // Wait for producer threads
    for (int i = 0; i < n1; i++) {
        pthread_join(prod_THREAD[i], NULL);
    }

    // Wait for the consomateur thread
    pthread_join(cons_THREAD, NULL);

    // Display resultating matrix A
    printf("Matrix A:\n");
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < m2; j++) {
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }

    // Destroy semaphores and mutex
    sem_destroy(&zero);
    sem_destroy(&lob);
    pthread_mutex_destroy(&mutex);

    // Free allocated space
    free(RESULTAT_BUFFER);

    return 0;
}
