//section include..
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
//==================define========================================
#define MAX_SIZE 100 // Maximum size for matrices
#define BUFFER_SIZE_MAX 100 // Size of the buffer
//==================variable globales============================= 
int matrixA[MAX_SIZE][MAX_SIZE];// Resultant matrix
int matrixB[MAX_SIZE][MAX_SIZE];
int matrixC[MAX_SIZE][MAX_SIZE];
int rowsC, colsC, rowsB, colsB; // Dimensions of matrices A and B
int indexSuivInserez = 0, indexSuivEnlevez = 0;
int BUFFER_SIZE;
//=================pour la synchronisation======================== 
sem_t empty;
sem_t full;
sem_t mutex1;
pthread_mutex_t mutex;

//=================Structure for buffer item=======================
typedef struct {
    int data;
    int row;
    int col;
} BufferItem;

BufferItem *buffer;
//==========================functions=========================

                    /*insert_item*/
void insert_item(int s,int index,int j,int indexSuivInserez){
        buffer[indexSuivInserez].data=s;
        buffer[indexSuivInserez].row=index;
        buffer[indexSuivInserez].col=j;
        
}


/*Function to perform matrix multiplication*/
void* producer(void* arg) {
    
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        int index =*(int*) arg;
        int s = 0;
        int j=0;
        while(j<colsB){
            s=0;
            for (int k = 0; k <rowsB ; k++) {
                s += matrixC[index][k] * matrixB[k][j];
            }
            insert_item(s,index,j,indexSuivInserez);
            indexSuivInserez = (indexSuivInserez + 1) % BUFFER_SIZE;
            j++;
        }
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        pthread_exit(NULL);
   
}

        /*remove_item*/
void remove_item() {
    int l=0;
    for(l=0;l<BUFFER_SIZE;l++){
    int inr =buffer[indexSuivEnlevez].row;
    int inc =buffer[indexSuivEnlevez].col;
    matrixA[inr][inc]=buffer[indexSuivEnlevez].data;
    indexSuivEnlevez = (indexSuivEnlevez + 1) % BUFFER_SIZE;
    }
}
       /*consumer*/
void *consumer(){
 
        sem_wait(&full);
        pthread_mutex_unlock(&mutex);

        remove_item();
        //free(buffer);
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        pthread_exit(NULL);
}

//===================MAIN=================================//
int main() {
int i=0,j=0;

//initialisation des dimension des matrice:
printf("veuillez saisire nombre des lignes de MatC");
scanf("%d",&rowsC);
printf("veuillez saisire nombre des colomnes de MatC");
scanf("%d",&colsC);
printf("veuillez saisire nombre des lignes de MatB");
scanf("%d",&rowsB);
printf("veuillez saisire nombre des colomnes de MatB");
scanf("%d",&colsB);


// Initialize matrices A and B with appropriate dimensions and values
    //remplissage des matrice C et B:
for (i = 0; i < rowsC; i++){
    for (j = 0; j < colsC; j++){
        matrixC[i][j] = rand () % 100;
	}
}
  
for(i = 0; i < rowsB; i++){
    for (j = 0; j < colsB; j++){
	    matrixB[i][j] = rand () % 100;
	}
}
    //affichage  des Matrice C et B:
for(i = 0; i < rowsC; i++){
    for (j = 0; j < colsC; j++){
        printf ("%d\t", matrixC[i][j]);
	}
    printf ("\n");
}
  
printf ("=================================\n");
  
for(i = 0; i < rowsB; i++){
    for(j = 0; j < colsB; j++){
	    printf ("%d\t", matrixB[i][j]);
    }
    printf ("\n");
}


if(colsC!=rowsB){
    printf("vous ne pouvez pas fair la Multiplication de ces deux Matrice!");
    exit(EXIT_FAILURE);
}

// Initialize semaphores and mutex
sem_init(&empty, 0,1);
sem_init(&full, 0, 0);
pthread_mutex_init(&mutex, NULL);
//Initialize BUFFER_SIZE
BUFFER_SIZE=colsB;
buffer=(BufferItem*)malloc(BUFFER_SIZE*sizeof(BufferItem));

//========creation des threads===========================
pthread_t prodecteur[rowsC],consommateur[rowsC];
    /*thread prodecteur*/ 
for (i = 0; i < rowsC; i++) {
    int *index = malloc(sizeof(int));
    *index = i;
    pthread_create(&prodecteur[i], NULL, &producer, index);
    }
for(i = 0; i < rowsC; i++){
     pthread_create (&consommateur[i], NULL, &consumer, NULL);
}

//=================l'attente des threads===================
    /*l'attente des threads prodecteurs*/ 
for(i = 0; i < rowsC; i++){
    pthread_join(prodecteur[i], NULL);
}
for(i = 0; i < rowsC; i++){
     pthread_join(consommateur[i], NULL);
}

// Display the resultant matrix C
printf("Resultant Matrix C:\n");
    for (int i = 0; i < rowsC; ++i) {
        for (int j = 0; j < colsB; ++j) {
            printf("%d ", matrixA[i][j]);
        }
        printf("\n");
    }

    // Destroy semaphores and mutex
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
    free(buffer);
    return 0;
}
