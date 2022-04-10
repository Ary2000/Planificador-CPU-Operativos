#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
//#include "planificador.c"

void *FIFO(void* arg){
    bool* activo = (bool *)arg;
    while(*activo){
        if(largoLista() == 0){
            sleep(1);
        }else{
            nodo* first = getPrimero();
            printf("%i  ,", first->info[0]);
            printf("%i,", first->info[1]);
            printf("%i\n", first->info[2]);
            sleep(first->info[1]);
            eliminar(0);

        }
    }
}

/*
int main(){
    char datos[] = "2,7";
    agregar(datos);
    char datos2[] = "3,1";
    agregar(datos2);
    bool *activo = true;
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, FIFO, &activo);
    if(activo){
        printf("Thread prueba\n");
    }
    pthread_join(thread_id, NULL);
    free(activo);
    return 0;
}*/