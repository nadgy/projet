#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int B[100][100];
int C[100][100];
int A[100][100];
int D[100][100];

// int T[12];
pthread_mutex_t mutex;
sem_t mute; 
sem_t empty;
sem_t full;
typedef struct argument{
    int ligne;
    int size;
    int zaki;
}argument;

typedef struct buffer{
    int val;
    int cas;
    int i;
}buffer;
buffer T[12];



void producter(void arg)
{
    argument a= (argument)arg;
   
    int j,somme=0,k,truee;
    for(j=0;j<a.zaki;j++)
        {
            somme=0;
            for( k=0;k<a.size;k++)
            {
                somme+=B[a.ligne][k]*C[k][j];
            }
            
            
            while(1){
            sem_wait(&empty);
            sem_wait(&mute);
            
              if(T[j].cas==-1){  
            T[j].val=somme;
            T[j].cas=j;
            T[j].i=a.ligne;
            sem_post(&mute);
            sem_post(&full);
                  break;
              }
                else {
                    sem_post(&mute);
                    sem_post(&full);
                    
                }
            }
        
}
}
 

 void *consumer(void *arg)
{
    int tab = (int)arg;
    
    int i,tru,q,l;
    // for(i=0;i<tab[0];i++){
            for(int j=0;j<tab[1];j++){
            
            while(1){
            sem_wait(&full);
            sem_wait(&mute);
             
            
            if(T[j].cas!=-1){
            q=T[j].i;
            l=T[j].cas;
            D[q][l]=T[l].val;
            
            T[j].cas=-1;
            sem_post(&mute);
            sem_post(&empty);
            break;
            }
                else {
                    sem_post(&mute);
                    sem_post(&empty);}
            }
        }
    // }
}


int main() {
    int i, j;
    int n,k,m,b;
    
    for (int i = 0; i < 12; i++) {
        T[i].cas = -1;
    }
    
    printf("entrer l'ordre de les matrice B:");
    scanf("%d",&n);
    scanf("%d",&k);
    printf("entrer l'ordre de les matrice  C:");
    scanf("%d",&b);
    scanf("%d",&m);
    if (k==b){
    sem_init(&mute,0,1); 
    sem_init(&empty,0,m);
    sem_init(&full,0,0); 
    // Remplir B:
    printf("remplir la matrice B:\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < k; j++) {
            B[i][j]=1;
        }
    }

    // Remplir C:
    printf("remplir la matrice C:\n");
    for (i = 0; i < k; i++) {
        for (j = 0; j < m; j++) {
            C[i][j]=1;
        
        }
    }


    // Affiche B
    printf("Matrice B :\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < k; j++) {
            printf("%d ", B[i][j]);
        }
        printf("\n");
    }

    // Affiche C
    printf("Matrice C :\n");
    for (i = 0; i < k; i++) {
        for (j = 0; j < m; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }
    
    argument a[n];
    pthread_t threads[n],thread[n];
   
    pthread_t cons,prod;
    int tab[2];
    tab[0]=n;
    tab[1]=m;
    // pthread_create(&cons,NULL,&consumer,&tab);

    for(i=0;i<n;i++)
    {
        a[i].ligne = i;
        a[i].size = k;
        a[i].zaki = m;
        pthread_create(&threads[i],NULL,&producter,&a[i]);
        
    }
    for(i=0;i<n;i++){
        pthread_create(&thread[i],NULL,&consumer,&tab);
        }
    for(i = 0; i < n; i++) {
    pthread_join(threads[i], NULL);
}
    for(i = 0; i < n; i++) {
    pthread_join(thread[i], NULL);
}
    // pthread_join(cons,NULL);
    

      printf("Matrice D :\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            printf("%d ", D[i][j]);
        }
        printf("\n");
    }
    }
    else printf("faux");


    

    return 0;
}
