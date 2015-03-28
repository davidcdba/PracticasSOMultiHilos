#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>

#define N 3
#define ITER 5
#define C 5
#define P 5



int V[N];
int indiceP=0,indiceC=0,sumaC=0,sumaP=0;

sem_t mutex,empty,full;

void *productor();
void *consumidor();
int main(){
	extern sem_t mutex,empty,full;
	extern int sumaP,sumaC;
	int status,i;

		
	//iniciar hilos
	pthread_t hiloP[P],hiloC[C];
	//inicia semaforos
	sem_init(&mutex,0,1);
	sem_init(&full,0,0);
	sem_init(&empty,0,N);
	//Creacion
	for(i=0;i<P;i++)
		if((status=pthread_create(&hiloP[i],NULL,productor,NULL)))
			exit(status);
	
	for(i=0;i<C;i++)
		if((status=pthread_create(&hiloC[i],NULL,consumidor,NULL)))
			exit(status);

	//Terminan los hilos
		for(i=0;i<P;i++){
			pthread_join(hiloP[i],NULL);
			printf("El hilo Productor %d con numero %lu a terminado \n",i,hiloP[i]);
		}
	
	for(i=0;i<C;i++){
		pthread_join(hiloC[i],NULL);
		printf("El hilo Consumidor %d con numero %lu a terminado \n",i,hiloC[i]);
	}	

	//Sumas finales
	printf("\n\nLa suma productor es %d\n",sumaP);
	printf("La suma consumidor es %d\n",sumaC);
	return(0);
}
void *productor(){

	extern sem_t mutex,empty,full;
	extern int V[N],indiceP,sumaP;

	int i;
	
	for(i=0;i<ITER;i++){
		sem_wait(&empty);
		sem_wait(&mutex);
			V[indiceP%N]=rand() % 1000;
			printf("Produce %d\n",V[indiceP%N]);
			sumaP+=V[indiceP%N];indiceP++;
		sem_post(&mutex);
		sem_post(&full);
	}
	pthread_exit(NULL);
	
}
void *consumidor(){


	extern sem_t mutex,empty,full;
	extern int V[N],indiceC,sumaC;

	int i;
	
	for(i=0;i<ITER;i++){
		sem_wait(&full);
		sem_wait(&mutex);
			printf("Consume %d\n",V[indiceC%N]);
			
			sumaC+=V[indiceC%N];indiceC++;
		sem_post(&mutex);
		sem_post(&empty);
	}
	pthread_exit(NULL);





}

