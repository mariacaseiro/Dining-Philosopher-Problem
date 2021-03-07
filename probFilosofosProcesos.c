//PROBLEMA DE LOS FILÓSOFOS
//Code created by: @mariacaseiro

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>

/* 1. Cada filosofo es un hilo. */
/* 2. Los filosofos se alternan entre pensar y comer. */
/* 3. Para similar ambas acciones introduzco un sleep random. */
/* 4. Llamamos a tomarTenedor() cuando el filosofo quiere comer
y a ponerTenedor() cuando termina. */

/*Dos filósofos consecutivos no podran comer a la
vez, lo cual se comprobara gracias a la función probar().
Filosofos -> procesos
Tenedores -> recursos*/

//Estados de los filosofos
#define PENSANDO 0
#define HAMBRIENTO 1
#define COMIENDO 2

//control de vecinos de los filosofos
#define IZQ (i+maxProcesos-1)%maxProcesos
#define DCHA (i+1)%maxProcesos

//Variables globales
int maxProcesos;
int *estado; //lleva el registro del estado de todos

//Declaracion de Semaforos
sem_t *mutex; //control de la region critica
sem_t *sem; //control de si los filosofos consiguen o no los tenedores

//Inicializar y finalizar proyeccion en el mapa de memoria compartido por los procesos
void inicializarDatos(){
  	int i;
  	char a[2];
  	a[1]='\0';

    sem=(sem_t *) malloc(maxProcesos*sizeof(sem_t));
	srand(2);

    //variables comun a todos los procesos
	estado=(int*)mmap(NULL,maxProcesos*sizeof(int), PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);

    //Al principio todos están pensando
    for(i=0;i<maxProcesos;i++){
        estado[i]=PENSANDO;
    }

 	sem_unlink("MUTEX"); //por si hubiese quedado abierto
 	mutex=sem_open("MUTEX",O_CREAT,0700,1);
 	for(i=0;i<maxProcesos;++i){
		 //un semaforo por hilo (representa los filosofos)
		 a[0]=48+'\0';
		 sem_unlink(a); //por si hubiese quedado abierto
		 sem[i]=*sem_open(a,O_CREAT,0700,0);
  	}
}

//Cerramos la proyeccion de memoria y los semaforos (no se va a ejecutar)
void finalizarDatos(){
  int i;
  sem_close(mutex);
  for (i=0;i<maxProcesos;++i){
	  //un semaforo por hilo
	  sem_close(&sem[i]);
  }
  munmap((void *)estado,maxProcesos*sizeof(int));
}

//--------------------------------------------------------------------

//Saber si un filosofo esta comiendo
/*En este funcion se comprueba si las condiciones son optimas para que el
filosofo se ponga a comer, es decir, que su estado sea hambirneto y que
su compañero de la derecha y su compañero de la izquierda no esten comiendo,
es decir, que ambos tenedores de sus laterales esten libres*/
void probar(int i){
	if((estado[i] == HAMBRIENTO) && (estado[IZQ] != COMIENDO)
			&& (estado[DCHA] != COMIENDO)){
        printf("\x1b[32m");
        printf("El filósofo %d está COMIENDO.\n", i);
        estado[i] = COMIENDO;
        sleep(rand()%3);
		sem_post(&sem[i]); //se hace un up -> 1, se consiguieron los tenedores
	}
}

//Cuando un filosofo esta pensando
/*Se llama a esta funcion en el momento en el que un filosofo deja de comer
y se pone a pensar, cambiando su estado a pensando y liberando los tenedores,
probando si alguno de sus vecinos pueden comer ahora (estaban esperando)*/
void ponerTenedor(int i){
	sem_wait(mutex); //region critica
		printf("\x1b[34m");
    	printf("El filósofo %d deja el tenedor y se pone a PENSAR.\n", i);
		estado[i] = PENSANDO;
       	sleep(rand()%5);
	   	probar(IZQ); //comprobamos si el de la izda puede comer
	   	probar(DCHA); //comprobamos si el de la dcha puede comer
	sem_post(mutex); //salir region critica
}


//Para que el filosofo cambie a hambriento
/*En este momento el filósofo se prepara para comer, en caso de que alguno de
los tenedores no estén disponibles se queda en estado hambriento y esperando a
que el tenedor que le falta este disponible (se activara en el momento en el
que el filosofo vecino que este comiendo llame a ponerTenedor y se cumpla que ambos
tenedores de sus laterales esten disponibles)*/
void tomarTenedor(int i){
    sem_wait(mutex); //entremos en la region critica (variable estado)
        printf("\x1b[33m");
        printf("El filósofo %d está HAMBRIENTO e intenta coger el tenedor.\n", i);
	    estado[i] = HAMBRIENTO; //estado hambriento = 1
	    probar(i); //comprobamos si puede comer
		/*for(int j=0;j<maxHilos;j++){
			printf("%d |",estado[j]);
		}
		printf("\n");*/
        printf("\x1b[31m");
        if(estado[i]!=COMIENDO){
            printf("Oh Oh!! Parece que el tenedor está ocupado :(\n");
        }
	sem_post(mutex); //salimos de la region critica
	sem_wait(&sem[i]); //se bloquea si no se consiguieron los tenedores
	/*Si se habian conseguido los tenedores entonces pasa a valer 0, pero en
	el caso de que no se consiguiesen el valor del semaforo ya era 0 y por lo
	tanto se bloquea*/
}

//Una ejecucion por cada proceso
void filosofo(int i){//Funciones que ejecutan los filosofos

    printf("El filósofo %d se sienta a la mesa.\n", i);

    while(1){
        sleep(rand()%2);
		tomarTenedor(i); //cuando el filosofo quiere comer
		ponerTenedor(i); //cuando el filosofo quiere pensar
	}
}

int main(){

    int i=0;
    pid_t pid; //para la creacion de hijos

    printf("Introduzca el número de filósofos que van a acudir a la cena:\n");
    scanf("%d", &maxProcesos);

    //inicializar datos
	inicializarDatos();

    printf("La mesa está puesta!! Hay %d tenedores.\n",maxProcesos);

	//Creación de filósofos
	for(i=0;i<maxProcesos;++i){
	    pid=fork();
	    if(pid==0){
	      	//hijo
	      	filosofo(i);
	      	exit(0);
	    }else if(pid<0){
	      	perror("fork");
	      	exit(0);
	    }
	}

    /*Debido a la existencia de un while(1) en la funcion filosofo la ejecucion
	del codigo que aparece a partir de este punto no tendra lugar, pero se incluye
	para mostrar como seria su correcta implementacion*/


  	//Esperamos a que acaben todos los procesos
  	for(i=0;i<maxProcesos;++i){
		wait(NULL);
		//ponemos null pq nos de igual el estado de salida
	}

	finalizarDatos();

    //liberar memoria
    free(sem);

	return 0;
}
