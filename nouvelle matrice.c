#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_SIZE 10  // Taille maximale des matrices
#define MAX_THREADS 10  // Nombre maximal de threads
#define N 100  // Taille du tampon

int B[MAX_SIZE][MAX_SIZE], C[MAX_SIZE][MAX_SIZE];  // Matrices B et C
int n1, m1, n2, m2;  // Tailles des matrices B et C
int T[N];  // Tampon pour les résultats intermédiaires
int A[MAX_SIZE][MAX_SIZE];  // Matrice résultante A

// Verrou pour protéger l'accès à la matrice résultante A
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Fonction pour calculer une ligne de la matrice résultante A
void *producer(void *arg) {
    int row = *((int *)arg);

    // Calculer la ligne de la matrice résultante A et stocker le résultat dans le tampon T
    for (int j = 0; j < m2; j++) {
        T[row * m2 + j] = 0;
        for (int k = 0; k < m1; k++) {
            T[row * m2 + j] += B[row][k] * C[k][j];
        }
    }

    pthread_exit(NULL);
}

// Fonction pour consommer un élément du tampon T et le placer dans la matrice résultante A
void *consumer(void *arg) {
    int index = *((int *)arg);

    // Consommer l'élément T[index] et le placer dans la matrice résultante A au bon emplacement
    int row = index / m2;
    int col = index % m2;

    pthread_mutex_lock(&mutex);
    A[row][col] = T[index];
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main() {
    // Saisir les tailles des matrices B et C
    printf("Entrez le nombre de lignes de la matrice B : ");
    scanf("%d", &n1);
    printf("Entrez le nombre de colonnes de la matrice B : ");
    scanf("%d", &m1);
    printf("Entrez le nombre de lignes de la matrice C : ");
    scanf("%d", &n2);
    printf("Entrez le nombre de colonnes de la matrice C : ");
    scanf("%d", &m2);

    // Saisir les valeurs de la matrice B
    printf("Entrez les valeurs de la matrice B :\n");
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < m1; j++) {
            printf("B[%d][%d] : ", i, j);
            scanf("%d", &B[i][j]);
        }
    }

    // Saisir les valeurs de la matrice C
    printf("Entrez les valeurs de la matrice C :\n");
    for (int i = 0; i < n2; i++) {
        for (int j = 0; j < m2; j++) {
            printf("C[%d][%d] : ", i, j);
            scanf("%d", &C[i][j]);
        }
    }

    // Créer les threads producteurs
    pthread_t producer_threads[MAX_THREADS];
    int producer_arguments[MAX_THREADS];
    for (int i = 0; i < n1; i++) {
        producer_arguments[i] = i;
        pthread_create(&producer_threads[i], NULL, producer, (void *)&producer_arguments[i]);
    }

    // Créer les threads consommateurs
    pthread_t consumer_threads[N];
    int consumer_arguments[N];
    for (int i = 0; i < N; i++) {
        consumer_arguments[i] = i;
        pthread_create(&consumer_threads[i], NULL, consumer, (void *)&consumer_arguments[i]);
    }

    // Attendre la fin des threads consommateurs
    for (int i = 0; i < N; i++) {
        pthread_join(consumer_threads[i], NULL);
    }

    // Afficher la matrice résultante A
    printf("Matrice résultante A :\n");
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < m2; j++) {
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }

    return 0;
}
