#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define N 3 // Taille du tampon
#define n1 3 // Nombre de lignes de B
#define m1 3 // Nombre de colonnes de B 
#define n2 3 // Nombre de colonnes de C

// Matrices
double B[n1][m1], C[m1][n2], A[n1][n2];
// Tampon
double T[N];

// Synchronisation
pthread_mutex_t mutex;
sem_t empty;
sem_t full;

// Fonction factice pour illustrer l'idée
int getNextRow() {
    static int currentRow = 0;
    return (currentRow < n1) ? currentRow++ : -1;
}

// Fonction factice pour illustrer l'idée
void insert_item(int row, double *result) {
    // Ici, nous copions le résultat dans la matrice résultante A
    for (int col = 0; col < n2; ++col) {
        A[row][col] = result[col];
    }
}

// Fonction factice pour illustrer l'idée
int remove_item() {
    // Ici, nous retournons simplement une valeur factice (-1) pour illustrer la fin
    static int currentRow = 0;
    if (currentRow < n1) {
        return currentRow++;
    } else {
        return -1;
    }
}

// Producteur
void *producer(void *arg) {
    int row;
    while (1) {
        // Obtenir la prochaine ligne à calculer
        pthread_mutex_lock(&mutex);
        row = getNextRow();
        pthread_mutex_unlock(&mutex);

        // Vérifier la condition de terminaison
        if (row == -1)
            break;

        // Calculer la ligne de la matrice résultante A et la stocker dans le tampon T
        for (int col = 0; col < n2; ++col) {
            double result = 0.0;
            for (int i = 0; i < m1; ++i) {
                result += B[row][i] * C[i][col];
            }
            T[col] = result;
        }

        sem_wait(&empty); // Attente d'une place libre
        pthread_mutex_lock(&mutex);
        // Section critique
        insert_item(row, T);
        pthread_mutex_unlock(&mutex);
        sem_post(&full); // Il y a une place remplie en plus
    }

    pthread_exit(NULL);
}

// Consommateur
void *consumer(void *arg) {
    int row;

    while (1) {
        sem_wait(&full); // Attente d'une place remplie
        pthread_mutex_lock(&mutex);
        // Section critique
        row = remove_item();
        pthread_mutex_unlock(&mutex);
        sem_post(&empty); // Il y a une place libre en plus

        // Vérifier la condition de terminaison et quitter si nécessaire
        if (row == -1)
            break;

        // Afficher la ligne résultante
        printf("Consumer: Row %d\n", row);
    }

    pthread_exit(NULL);
}

// Fonction principale
int main() {
    // ... (le reste du code reste inchangé)
    return 0;
}
