// Noui Mehdi G02
// Abdelmadjid Larik G02

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#define N 100 

int L1,C1,L2,C2;
int **A, **B, **C;
int *BUFFER;
pthread_mutex_t mutex;
sem_t empty,full;

void Matrices_Generator(){
     //memory allocation of the matrices
     int i,j;
     B=(int**)malloc(L1*sizeof(int*)); 
     C=(int**)malloc(C1*sizeof(int*));
     for(i=0;i<C1;i++){ 
         B[i]=(int*)malloc(C1*sizeof(int)); 
         C[i]=(int*)malloc(C2*sizeof(int)); 
     } 
     //generates a random set of numbers for the matrices
     //For B
     for (i=0;i<L1;i++){ 
         for(j=0;j<C1;j++){ 
             B[i][j]=rand()%10; 
         } 
     } 
     //For C
     for(i=0;i<L2;i++) { 
         for(j=0;j<C2;j++) { 
             C[i][j]=rand()%10; 
         } 
     } 
     //set the precise size for the buffer
     BUFFER=(int*)malloc(L1*C2*sizeof(int)); 
 }
 void Matrices_Printer(){
     int i,j; 
     //print Matrix B
     printf("\nMatrix B:\n"); 
     for(i=0;i<L1;i++){ 
         for (j=0;j<C1;j++){ 
             printf("%d\t",B[i][j]); 
         } 
         printf("\n"); 
     } 
     //print Matrix C
     printf("\nMatrix C:\n"); 
     for(i=0;i<L2;i++){
         for(j=0;j<C2;j++){ 
             printf("%d\t",C[i][j]); 
         } 
         printf("\n"); 
     } 
     printf("\n"); 
 } 

void Matrices_Destroyer(){
     int i,j;
     //free the allocated memoty of the matrix B
     for(i=0;i<L1;i++)free(B[i]);free(B);
     //free the allocated memory of the matrix C
     for(i=0;i<C1;i++)free(C[i]);free(C);
     //free the allocated memory of the buffer
     free(BUFFER);
}

void *producer(void *args){
     int row=*(int *)args,temp;
     int i,j;
     for (i=0;i<C2;i++){
	 temp=0;
	 //calculate the row's elements
         for(j=0;j<C1;j++){
             temp+=B[row][j]*C[j][i];
         }
	 sem_wait(&empty);
         pthread_mutex_lock(&mutex);
         BUFFER[row*C2+i]=temp;
         pthread_mutex_unlock(&mutex);
         sem_post(&full);
     }
    pthread_exit(NULL);
 }

 void *consumer(void *args){
     int row=*(int*)args;
     int i,j;
     for (i=0;i<C2;i++){
         sem_wait(&full);
         pthread_mutex_lock(&mutex);
         A[row][i]=BUFFER[(row*C2+i)%(L1*C2)];
         pthread_mutex_unlock(&mutex);
         sem_post(&empty);
     }
     pthread_exit(NULL);
 }
 
 int main(int argc, char *argv[]) 
 { 
     sem_init(&full,0,0); 
     sem_init(&empty,0,N); 
     pthread_mutex_init(&mutex, NULL); 
  	
     //SETTING THE MATRICES UP  
     printf("Set the dimensions of Matrix B:\n"); 
     printf("Set the number of rows :");
     scanf("%d",&L1);
     printf("set the number of Columns :");
     scanf("%d",&C1);

     printf("Set the dimensions of Matrix C:\n"); 
     printf("Set the number of rows :");
     scanf("%d",&L2);
     printf("set the number of Columns :");
     scanf("%d",&C2);
     printf("\n");
     
     if(C1!=L2){ 
         perror("Failure.\nMatrix dimensions do not match for a multipicaiton\n"); 
         exit(EXIT_FAILURE); 
     }
     Matrices_Generator(); 
     Matrices_Printer(); 
     
     //memory allocation for A	
     int i,j;
     A=(int**)malloc(L2*sizeof(int*)); 
     for (i=0;i<L2; i++){ 
         A[i]= (int*)malloc(C2*sizeof(int)); 
     } 
  	
     //setting up the threads
     pthread_t producers_thread[L1],consumer_thread[L1]; 
  
     //creating the threads
     for (i=0;i<L2;i++){ 
         int *row=malloc(sizeof(int)); 
         *row=i; 
         if(pthread_create(&producers_thread[i],NULL,producer,row)!=0){ 
             perror("\n Thread creation error !\n"); 
             exit(EXIT_FAILURE); 
         } 
         if(pthread_create(&consumer_thread[i],NULL,consumer,row)!=0){ 
             perror("\n Thread creation error !\n"); 
             exit(EXIT_FAILURE); 
         } 
     } 
     
     //waiting for the threads
     for(i=0;i<L1;i++){ 
         pthread_join(producers_thread[i],NULL); 
     } 
     for(i=0;i<L1;i++){ 
         pthread_join(consumer_thread[i],NULL); 
     } 
  
     //print Matrix C
     printf("\nTHE RESULT (Matrix A):\n"); 
     for(i=0;i<C2;i++){
         for(j=0;j<L2;j++){ 
             printf("%d\t",A[i][j]); 
         } 
         printf("\n"); 
     } 
     printf("\n"); 

     //free the allocated memory
     Matrices_Destroyer();

     pthread_mutex_destroy(&mutex); 
     sem_destroy(&empty); 
     sem_destroy(&full); 
  
     return 0; 
  }
