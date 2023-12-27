#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_SIZE 100

// Déclaration des variables globales
int n1, m1, n2, m2;  // Dimensions des matrices
int *B, *C, *A, *T;   // Pointeurs vers les matrices B, C, A et le tableau T
int nextRowToProduce = 0;  // Indice de la prochaine ligne à produire
int nextRowToConsume = 0;  // Indice de la prochaine ligne à consommer
int bufferSize;  // Taille du tampon


// Sémaphores
sem_t sem_empty, sem_full, mutex;

// Fonction exécutée par chaque thread producteur
void *producer(void *arg) {
    while (1) {
        int currentRow;

        sem_wait(&mutex);
        if (nextRowToProduce < n1) {
            currentRow = nextRowToProduce++;
            sem_post(&mutex);
 // Multiplication de la ligne de B par la matrice C
            for (int j = 0; j < m2; ++j) {
                A[currentRow * m2 + j] = 0;
                for (int k = 0; k < m1; ++k) {
                    A[currentRow * m2 + j] += B[currentRow * m1 + k] * C[k * m2 + j];
                }
            }

            sem_wait(&sem_empty);
            T[currentRow] = currentRow;
            sem_post(&sem_full);
        } else {
            sem_post(&mutex);
            break;  // Toutes les lignes ont été produites
        }
    }

    pthread_exit(NULL);
}

// Fonction exécutée par chaque thread consommateur
void *consumer(void *arg) {
    while (1) {
        sem_wait(&mutex);
        if (nextRowToConsume < n1) {
            int currentRow = nextRowToConsume++;
            sem_post(&mutex);

          // Copie de la ligne depuis le tableau T vers la matrice résultante A
            for (int j = 0; j < m2; ++j) {
                A[currentRow * m2 + j] = T[currentRow * m2 + j];
            }

            sem_post(&sem_empty);
        } else {
            sem_post(&mutex);
            break;  // Toutes les lignes ont été consommées
        }
    }

    pthread_exit(NULL);
}


int main() {

  //entrée les dimensions des matrices B et C
    printf("Entrez les dimensions de la matrice B (n1 m1) : ");
    scanf("%d %d", &n1, &m1);

    printf("Entrez les dimensions de la matrice C (n2 m2) : ");
    scanf("%d %d", &n2, &m2);

    // Vérifications pour des dimensions valides
    if (n1 <= 0 || m1 <= 0 || n2 <= 0 || m2 <= 0) {
        printf("Les dimensions des matrices doivent être des entiers positifs.\n");
        return 1;
    }

    if (m1 != n2) {
        printf("Impossible de multiplier les matrices. Le nombre de colonnes de B doit être égal au nombre de lignes de C.\n");
        return 1;
    }

    // Allocation dynamique de mémoire pour les matrices
    B = (int *)malloc(n1 * m1 * sizeof(int));
    C = (int *)malloc(n2 * m2 * sizeof(int));
    A = (int *)malloc(n1 * m2 * sizeof(int));
    T = (int *)malloc(n1 * sizeof(int));

    /// Initialiser la matrice B avec des valeurs aléatoires
    printf("Matrice B :\n");
    for (int i = 0; i < n1; ++i) {
        for (int j = 0; j < m1; ++j) {
            B[i * m1 + j] = rand() % 10;
            printf("%d ", B[i * m1 + j]);
        }
        printf("\n");
    }

    // Initialiser la matrice C avec des valeurs aléatoires
    printf("Matrice C :\n");
    for (int i = 0; i < n2; ++i) {
        for (int j = 0; j < m2; ++j) {
            C[i * m2 + j] = rand() % 10;
            printf("%d ", C[i * m2 + j]);
        }
        printf("\n");
    }

  // Entrée de la taille du tampon
    printf("Entrez la taille du tampon : ");
    scanf("%d", &bufferSize);


    // Initialisation des sémaphores
    sem_init(&sem_empty, 0, bufferSize);
    sem_init(&sem_full, 0, 0);
    sem_init(&mutex, 0, 1);

    pthread_t producerThread, consumerThread;

    // Création du thread producteur
    pthread_create(&producerThread, NULL, producer, NULL);

    // Création du thread consommateur
    pthread_create(&consumerThread, NULL, consumer, NULL);

    // Attendre de la fin des threads 
    pthread_join(producerThread, NULL);
    pthread_join(consumerThread, NULL);

    // Affichage de la matrice résultante A
    printf("Matrice résultante A :\n");
    for (int i = 0; i < n1; ++i) {
        for (int j = 0; j < m2; ++j) {
            printf("%d ", A[i * m2 + j]);
        }
        printf("\n");
    }

    // Libération de la mémoire
    free(B);
    free(C);
    free(A);
    free(T);

    // Destruction des sémaphores
    sem_destroy(&sem_empty);
    sem_destroy(&sem_full);
    sem_destroy(&mutex);

    return 0;
}
