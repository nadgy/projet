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
int rowsC, colsC, rowsB, colsB; // Dimensions of matrices C and B
int indexSuivInserez = 0, indexSuivEnlevez = 0;
int BUFFER_SIZE;
int count=0,i=0,j=0;
//=================pour la synchronisation======================== 
sem_t empty;
sem_t full;
pthread_mutex_t mutex;
//==================struct============
typedef struct BufferItem{
    int data;
    int row;
    int col;
} BufferItem;

BufferItem *buffer;
//==================insert_item=============================
void insert_item(int s,int index,int j,int indexSuivInserez){
        buffer[indexSuivInserez].data=s;
        buffer[indexSuivInserez].row=index;
        buffer[indexSuivInserez].col=j;
        
}
//==================producer=============================
void* producer(void* arg) {
int index =*(int*) arg;
int s = 0;
int j=0;
while(j<colsB){
    for (int k = 0; k <rowsB ; k++) {
        s += matrixC[index][k] * matrixB[k][j];
    }
    while(count==BUFFER_SIZE){
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);}
    insert_item(s,index,j,indexSuivInserez);
    indexSuivInserez = (indexSuivInserez + 1) % BUFFER_SIZE;
    count++;
    s=0;
    j++;
    }
   if(count==1){
    pthread_mutex_unlock(&mutex);
    sem_post(&full);}
    pthread_exit(NULL);
}
void remove_item() {
    int inr =buffer[indexSuivEnlevez].row;
    int inc =buffer[indexSuivEnlevez].col;
    matrixA[inr][inc]=buffer[indexSuivEnlevez].data;
}
       /*consumer*/
void *consumer(){
        if(count==0){
        sem_wait(&full);
        pthread_mutex_lock(&mutex);}
        
        remove_item();
        indexSuivEnlevez = (indexSuivEnlevez + 1) % BUFFER_SIZE;
        count--;
        
    if(count==BUFFER_SIZE-1){
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);}

        pthread_exit(NULL);
}

int main() {
    
//initialisation la taille de buffer:
printf("veuillez saisire BUFFER_SIZE<100:");
scanf("%d",&BUFFER_SIZE);

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
printf("================Matrix C:==================\n");
for(i = 0; i < rowsC; i++){
    for (j = 0; j < colsC; j++){
        printf ("%d\t", matrixC[i][j]);
	}
    printf ("\n");
}
  

printf("===============Matrix B:=================================\n");
for(i = 0; i < rowsB; i++){
    for(j = 0; j < colsB; j++){
	    printf ("%d\t", matrixB[i][j]);
    }
    printf ("\n");
}
// verifier que la taille de buffer > 0: 
if (BUFFER_SIZE <= 0){
    perror("Buffer size  doit etre superieur a 0.\n");
    exit(EXIT_FAILURE);
}
 // verifier c'est les mat B et C repondre aux exigences:
if (((rowsC <= 0 || rowsC >= 100) || (colsC <= 0 || colsC >= 100) ) || ((rowsB <= 0 || rowsB >= 100) || (colsB <= 0 || colsB >= 100))){
        printf("les dimension des matrice doit etre sup 0 et inf ou egal 100\n");
        exit(EXIT_FAILURE);
    }
// verifie c'est les mat B et C peut multiplier:
if(colsC!=rowsB){
    printf("vous ne pouvez pas fair la Multiplication de ces deux Matrice!");
    exit(EXIT_FAILURE);
}

//allocation de buffer:
buffer=(BufferItem*)malloc(BUFFER_SIZE*sizeof(BufferItem));

// Initialize semaphores and mutex
sem_init(&empty, 0,BUFFER_SIZE);
sem_init(&full, 0, 0);
pthread_mutex_init(&mutex, NULL);
//========creation des threads===========================
pthread_t prodecteur[rowsC],consommateur[rowsC*colsB];
    /*thread prodecteur*/ 
for (i = 0; i < rowsC; i++) {
    int *index = malloc(sizeof(int));
    *index = i;
    pthread_create(&prodecteur[i], NULL, &producer, index);
    }
for(i = 0; i < rowsC*colsB; i++){
     pthread_create (&consommateur[i], NULL, &consumer, NULL);
}

//=================l'attente des threads===================
    /*l'attente des threads prodecteurs*/ 
for(i = 0; i < rowsC; i++){
    pthread_join(prodecteur[i], NULL);
}
for(i = 0; i < rowsC*colsB; i++){
     pthread_join(consommateur[i], NULL);
}

// Display the resultant matrix C
printf("==================Resultant Matrix A==================:\n");
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







