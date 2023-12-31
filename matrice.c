#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_SIZE 100
#define BUFFER_SIZE 100

struct Element {
    int value;
    int row;
    int col;
};

int B[MAX_SIZE][MAX_SIZE], C[MAX_SIZE][MAX_SIZE];
int A[MAX_SIZE][MAX_SIZE];
struct Element T[BUFFER_SIZE]; // Tampon circulaire de structures Element
int n1, m1, n2, m2; // Tailles des matrices

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;
int T_index = 0; // Indice du tampon circulaire

struct ThreadData {
    int thread_id;
};

void remplir(int mat[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mat[i][j] = rand() % 10;
        }
    }
}

void *produire(void *arg) {
    struct ThreadData *data = (struct ThreadData *)arg;

    for (int i = 0; i < n1; i++) {
   
       
       
        for (int j = 0; j < m2; j++) {
           int result = 0;
            for (int k = 0; k < m2; k++) {
                result += B[i][k] * C[k][j];
            }

            pthread_mutex_lock(&mutex);
            while (T_index >= BUFFER_SIZE) {
                pthread_cond_wait(&empty, &mutex);
            }
            T[T_index].value = result;
            T[T_index].row = i;
            T[T_index].col = j;
            T_index++;
            pthread_cond_signal(&full);
            pthread_mutex_unlock(&mutex);
        }
    }

    pthread_exit(NULL);
}

void *consommerResultats(void *arg) {
    int consumed = 0;

    while (consumed < n1 * m2) {
        pthread_mutex_lock(&mutex);
        while (T_index == 0) {
            pthread_cond_wait(&full, &mutex);
        }
        T_index--;
        struct Element currentElement = T[T_index];
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);

 
      A[currentElement.row][currentElement.col] = currentElement.value;
        consumed++;
    }

    pthread_exit(NULL);
}


int main() {
    printf("Entrer le nombre de lignes de la matrice B : ");
    scanf("%d", &n1);
    printf("Entrer le nombre de colonnes de la matrice B : ");
    scanf("%d", &m1);
    printf("Entrer le nombre de lignes de la matrice C : ");
    scanf("%d", &n2);
    printf("Entrer le nombre de colonnes de la matrice C : ");
    scanf("%d", &m2);

    if (m1 != n2) {
        printf("Les dimensions ne sont pas compatibles, la multiplication est impossible.\n");
        return 1;
    } else {
   
        remplir(B, n1, m1);
        remplir(C, n2, m2);
        printf("Matrice B : \n");
        // Affichage de la matrice B
        for (int i = 0; i < n1; i++) {
            for (int j = 0; j < m1; j++) {
                printf("%d\t", B[i][j]);
            }
            printf("\n");
        }

        printf("Matrice C : \n");
        // Affichage de la matrice C
        for (int i = 0; i < n2; i++) {
            for (int j = 0; j < m2; j++) {
                printf("%d\t", C[i][j]);
            }
            printf("\n");
        }
    }

    pthread_t threads[n1];
    pthread_t consumer_thread;

    struct ThreadData thread_data[n1];
    struct ThreadData consumer_data;

    // Création des threads producteurs (calculateurs de lignes)
    for (int i = 0; i < n1; i++) {
        thread_data[i].thread_id = i;
        pthread_create(&threads[i], NULL, produire, (void *)&thread_data[i]);
    }

    // Création du thread consommateur
    pthread_create(&consumer_thread, NULL, consommerResultats, NULL);

    // Attente de la fin des threads producteurs
    for (int i = 0; i < n1; i++) {
        pthread_join(threads[i], NULL);
    }

    // Attente de la fin du thread consommateur
    pthread_join(consumer_thread, NULL);

    printf("Le produit : \n");
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < m2; j++) {
            printf("%d\t", A[i][j]);
        }
        printf("\n");
    }

    return 0;
}
