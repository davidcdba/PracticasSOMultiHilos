#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>


#define N 10
#define ITER 5


sem_t mutex, full,empty;
int V[N];
int sumaP=0,sumaC=0;

void *consumidor();
void *productor();


int main(){
	extern sem_t mutex, full,empty;
	extern int sumaP,sumaC;
	//Iniciacion de semaforos
	sem_init(&mutex,0,1);
	sem_init(&full,0,0);
	sem_init(&empty,0,N);
	int status,i;
	//definicion de hilos
	pthread_t hilo[2];
	//Creacion de hilos
	//Productor
	if((status=pthread_create(&hilo[0],NULL,productor,NULL)))
		exit(status);
	//Consumidor
	if((status=pthread_create(&hilo[1],NULL,consumidor,NULL)))
		exit(status);

	//Espera a que muera
	for(i=0;i<2;i++){
		pthread_join(hilo[i],NULL);
		printf("El hilo %d con numero %lu a terminado \n",i,hilo[i]);

	}

	//Sumas finales
	printf("\n\nLa suma productor es %d\n",sumaP);
	printf("La suma consumidor es %d\n",sumaC);

	return(0);
 }
void *productor(){
	
	extern sem_t mutex, empty,full;
	extern int V[N];
	extern int sumaP;

	int i;

	for(i=0;i<ITER;i++){
		sem_wait(&empty);
		sem_wait(&mutex);
			V[i%N]=rand() % 1000;
			printf("Produce %d \n",V[i%N]);
			sumaP=V[i%N];
		sem_post(&mutex);
		sem_post(&full);
	}

	pthread_exit(NULL);
}
void *consumidor(){
	extern sem_t mutex, empty,full;
	extern int V[N];
	extern int sumaC;
	int i;
	
	for(i=0;i<ITER;i++){
		sem_wait(&full);
		sem_wait(&mutex);
			printf("El consumidor consume %d",V[i%N]);
		sumaC=V[i%N];
		sem_post(&mutex);
		sem_post(&empty);
	}

	pthread_exit(NULL);

}




