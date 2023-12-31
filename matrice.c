#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
//draifi abdelahak et mansouri lounes 

#define MAX_THREADS 10
#define MAX_SIZE 10
#define BUFFER_SIZE 10

struct ElementMatrice {
    int valeur;
    int ligne;
    int colonne;
};

int MatriceB[MAX_SIZE][MAX_SIZE];
int MatriceC[MAX_SIZE][MAX_SIZE];
int MatriceA[MAX_SIZE][MAX_SIZE];
struct ElementMatrice Tampon[BUFFER_SIZE];
int LignesB, ColonnesB, LignesC, ColonnesC;
pthread_mutex_t mutex ;
sem_t vide;
sem_t plein;
int IndiceTampon = 0;

struct DonneesThread {
    int IDThread;
};

void InitialiserMatrice(int matrice[MAX_SIZE][MAX_SIZE], int lignes, int colonnes) {
    for (int i = 0; i < lignes; ++i) {
        for (int j = 0; j < colonnes; ++j) {
            matrice[i][j] = rand() % 10;
        }
    }
}

void AfficherMatrice(int matrice[MAX_SIZE][MAX_SIZE], int lignes, int colonnes) {
    for (int i = 0; i < lignes; ++i) {
        for (int j = 0; j < colonnes; ++j) {
            printf("%d ", matrice[i][j]);
        }
        printf("\n");
    }
}

void *Producteur(void *arg) {
    struct DonneesThread *donnees = (struct DonneesThread *)arg;

    for (int i = 0; i < LignesB; ++i) {
        for (int j = 0; j < ColonnesC; ++j) {
            int resultat = 0;
            for (int k = 0; k < ColonnesB; ++k) {
                resultat += MatriceB[i][k] * MatriceC[k][j];
            }

            sem_wait(&vide);
            pthread_mutex_lock(&mutex);
            Tampon[IndiceTampon].valeur = resultat;
            Tampon[IndiceTampon].ligne = i;
            Tampon[IndiceTampon].colonne = j;
            IndiceTampon++;
            pthread_mutex_unlock(&mutex);
            sem_post(&plein);
        }
    }

    pthread_exit(NULL);
}

void *Consommateur(void *arg) {
    int consomme = 0;

    while (consomme < LignesB * ColonnesC) {
        sem_wait(&plein);
        pthread_mutex_lock(&mutex);
        IndiceTampon--;
        struct ElementMatrice elementCourant = Tampon[IndiceTampon];
        pthread_mutex_unlock(&mutex);
        sem_post(&vide);

        MatriceA[elementCourant.ligne][elementCourant.colonne] = elementCourant.valeur;
        consomme++;
    }

    pthread_exit(NULL);
}

int main() {
    sem_init(&vide, 0, BUFFER_SIZE);
    sem_init(&plein, 0, 0);

    printf("Nombre de lignes de la matrice B : ");
    scanf("%d", &LignesB);
    printf("Nombre de colonnes de la matrice B : ");
    scanf("%d", &ColonnesB);
    printf("Nombre de lignes de la matrice C : ");
    scanf("%d", &LignesC);
    printf("Nombre de colonnes de la matrice C : ");
    scanf("%d", &ColonnesC);

    if (ColonnesB != LignesC) {
        printf("Les dimensions ne sont pas compatibles pour la multiplication.\n");
        return 1;
    } else {
        InitialiserMatrice(MatriceB, LignesB, ColonnesB);
        InitialiserMatrice(MatriceC, LignesC, ColonnesC);

        printf("Matrice B :\n");
        AfficherMatrice(MatriceB, LignesB, ColonnesB);
        printf("\nMatrice C :\n");
        AfficherMatrice(MatriceC, LignesC, ColonnesC);
    }

    pthread_t threadsProducteurs[MAX_THREADS];
    pthread_t threadConsommateur;

    struct DonneesThread donneesProducteur[MAX_THREADS];
    struct DonneesThread donneesConsommateur;

    
    for (int i = 0; i < LignesB; ++i) {
        donneesProducteur[i].IDThread = i;
        pthread_create(&threadsProducteurs[i], NULL, Producteur, (void *)&donneesProducteur[i]);
    }

    pthread_create(&threadConsommateur, NULL, Consommateur, NULL);

    
    for (int i = 0; i < LignesB; ++i) {
        pthread_join(threadsProducteurs[i], NULL);
    }

  
    pthread_join(threadConsommateur, NULL);

    sem_destroy(&vide);
    sem_destroy(&plein);

    printf("Matrice A résultante :\n");
    AfficherMatrice(MatriceA, LignesB, ColonnesC);

    return 0;
}
