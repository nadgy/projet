#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define N 10 // Taille du tampon 
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

int getNextRow() {
    static int currentRow = 0;
    return (currentRow < n1) ? currentRow++ : -1;
}

void insert_item(int row, double *result) {
    // Ici, nous copions le résultat dans la matrice résultante A
    for (int col = 0; col < n2; ++col) {
        A[row][col] = result[col];
    }
}

int remove_item(double *resultRow) {
    // Ici, nous retournons simplement une valeur factice (-1) pour illustrer la fin
    static int currentRow = 0;
    if (currentRow < n1) {
        // Copier la ligne de la matrice résultante A dans resultRow
        for (int col = 0; col < n2; ++col) {
            resultRow[col] = A[currentRow][col];
        }
        return currentRow++;
    } else {
        return -1;
    }
}

// Fonction pour saisir une matrice à partir de l'utilisateur
void inputMatrix(double matrix[][n2], int rows, int cols) {
    printf("Enter matrix (%d x %d):\n", rows, cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf("Enter element at position (%d, %d): ", i + 1, j + 1);
            scanf("%lf", &matrix[i][j]);
        }
    }
}

// Fonction pour afficher une matrice
void printMatrix(double matrix[][n2], int rows, int cols, const char *name) {
    printf("Matrix %s:\n", name);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf("%f ", matrix[i][j]);
        }
        printf("\n");
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
    double resultRow[n2];

    while (1) {
        sem_wait(&full); // Attente d'une place remplie
        pthread_mutex_lock(&mutex);
        // Section critique
        row = remove_item(resultRow);
        pthread_mutex_unlock(&mutex);
        sem_post(&empty); // Il y a une place libre en plus

        // Vérifier la condition de terminaison et quitter si nécessaire
        if (row == -1)
            break;

        // Placer la ligne résultante dans la matrice résultante A
        // Notez que dans une application réelle, vous pourriez effectuer d'autres opérations avec resultRow
        // plutôt que de simplement les copier dans la matrice résultante A.
    }

    pthread_exit(NULL);
}

int main() {
   
    inputMatrix(B, n1, m1);
    inputMatrix(C, m1, n2);

    // Initialisation
    sem_init(&empty, 0, N);  // Buffer vide
    sem_init(&full, 0, 0);   // Buffer vide
    pthread_mutex_init(&mutex, NULL); // Exclusion mutuelle

    // Création des threads producteurs
    pthread_t producers[n1];
    for (int i = 0; i < n1; ++i) {
        pthread_create(&producers[i], NULL, producer, NULL);
    }

    // Création des threads consommateurs
    pthread_t consumers[n1];
    for (int i = 0; i < n1; ++i) {
        pthread_create(&consumers[i], NULL, consumer, NULL);
    }

    // Attente des threads producteurs
    for (int i = 0; i < n1; ++i) {
        pthread_join(producers[i], NULL);
    }

    // Indiquer aux consommateurs que la production est terminée
    for (int i = 0; i < n1; ++i) {
        sem_post(&full);
    }

    // Attente des threads consommateurs
    for (int i = 0; i < n1; ++i) {
        pthread_join(consumers[i], NULL);
    }

    // Afficher les matrices et le résultat à la fin du programme
    printMatrix(B, n1, m1, "B");
    printMatrix(C, m1, n2, "C");
    printMatrix(A, n1, n2, "A");

    // Destruction des ressources
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}
