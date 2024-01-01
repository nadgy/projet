#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>


#define MAX_SIZE 5  

int n1, m1, n2, m2;       
int B[MAX_SIZE][MAX_SIZE], C[MAX_SIZE][MAX_SIZE], A[MAX_SIZE][MAX_SIZE]; 
int T[MAX_SIZE];          

sem_t empty, full, mutex;  

void cleanup()
{
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);
}

void sauvegarderMatrice(int matrice[MAX_SIZE][MAX_SIZE], const char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "w");
    if (fichier == NULL) {
        fprintf(stderr, "erreur lors de l'ouverture du fichier pour la sauvegarde");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < m1; j++) {
            fprintf(fichier, "%d ", matrice[i][j]);
        }
        fprintf(fichier, "\n");
    }

    fclose(fichier);
}

void chargerMatrice(int matrice[MAX_SIZE][MAX_SIZE], const char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        fprintf(stderr, "erreur lors de l'ouverture du fichier pour le chargement");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < m1; j++) {
            if (fscanf(fichier, "%d", &matrice[i][j]) != 1) {
                fprintf(stderr, "erreur lors de la lecture du fichier");
                fclose(fichier);
                exit(EXIT_FAILURE);
            }
        }
    }

    fclose(fichier);
}

void *producteur(void *arg) {
    int ligne = *((int *)arg);

    // Calculer la ligne de la matrice résultante A
    for (int j = 0; j < m2; j++) {
        int resultat = 0;
        for (int k = 0; k < m1; k++) {
            resultat += B[ligne][k] * C[k][j];
        }

        // Mettre le résultat dans le tampon
        sem_wait(&empty);
        sem_wait(&mutex);
        T[ligne] = resultat;
        sem_post(&mutex);
        sem_post(&full);
    }

    free(arg); 
    pthread_exit(NULL);
}

void *consommateur(void *arg) {
    int colonne = *((int *)arg);

    
    for (int i = 0; i < n1; i++) {
        sem_wait(&full);
        sem_wait(&mutex);
        A[i][colonne] = T[i];  // Place l'élément T[y] dans la matrice résultante A t
        sem_post(&mutex);
        sem_post(&empty);
    }

    free(arg); 
    pthread_exit(NULL);
}

int main() {
    

    
    sem_init(&empty, 0, MAX_SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    
    printf("donnez les dimensions de la matrice B (n1 m1, limitées à %d) : ", MAX_SIZE);
    scanf("%d %d", &n1, &m1);

    // Demander à l'utilisateur de saisir les dimensions de la matrice C
    printf("donnez les dimensions de la matrice C (n2 m2, limitées à %d) : ", MAX_SIZE);
    scanf("%d %d", &n2, &m2);

    if (m1 != n2) {
        fprintf(stderr, " NON,les dimensions des matrices ne sont pas compatibles pour la multiplication");
        return EXIT_FAILURE;
    }

    if (n1 > MAX_SIZE || m1 > MAX_SIZE || n2 > MAX_SIZE || m2 > MAX_SIZE) {
        fprintf(stderr, "NON,les dimensions des matrices dépassent la taille maximale autorisée");
        return EXIT_FAILURE;
    }

    
    printf("donnez les éléments de la matrice B (%d x %d) :\n", n1, m1);
    for (int i = 0; i < n1; i++)
        for (int j = 0; j < m1; j++)
            scanf("%d", &B[i][j]);

    printf("donnze les éléments de la matrice C (%d x %d) : ", n2, m2);
    for (int i = 0; i < n2; i++)
        for (int j = 0; j < m2; j++)
            scanf("%d", &C[i][j]);
      sauvegarderMatrice(B, "matriceB.txt");
      chargerMatrice(C, "matriceC.txt");
    // Initialiser la matrice résultante A avec des 0
    for (int i = 0; i < n1; i++)
        for (int j = 0; j < m2; j++)
            A[i][j] = 0;

    // Créer les threads producteurs
    pthread_t threadsProducteurs[n1];
    for (int i = 0; i < n1; i++) {
        int *ligne = malloc(sizeof(int)); // Allouer de la mémoire pour chaque index de ligne
        *ligne = i;
        pthread_create(&threadsProducteurs[i], NULL, producteur, (void *)ligne);
    }

    // Créer les threads consommateurs
    pthread_t threadsConsommateurs[m2];
    for (int j = 0; j < m2; j++) {
        int *colonne = malloc(sizeof(int)); // Allouer de la mémoire pour chaque index de colonne
        *colonne = j;
        pthread_create(&threadsConsommateurs[j], NULL, consommateur, (void *)colonne);
    }

    for (int i = 0; i < n1; i++)
        pthread_join(threadsProducteurs[i], NULL);

    for (int j = 0; j < m2; j++)
        pthread_join(threadsConsommateurs[j], NULL);

    
    printf("Matrice résultante A :\n");
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < m2; j++)
            printf("%d ", A[i][j]);
        printf("\n");
    }

    cleanup();
    return 0;
}
