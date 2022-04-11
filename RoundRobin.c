#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
//#include "planificador.c"

int quantum = 4;

void *RR(void* arg){
    bool* activo = (bool *)arg;
    int contador = 0;
    nodo* proceso;
    while(*activo){
        //printf("En el while\n");
        if(largoLista() == 0){
            sleep(1);
            //*activo = false;
        }else{
            if(contador >= largoLista()){
                contador = 0;
            }
            proceso = getElemento(contador);
            if(proceso->info[1] < proceso->tiempoEjecutado + quantum){
                sleep(proceso->info[1] - proceso->tiempoEjecutado);
                printf("[PID: %i,", proceso->info[0]);
                printf("BURST: %i,", proceso->info[1]);
                printf("PRIORIDAD: %i]\n", proceso->info[2]);
                //proceso = NULL;
                eliminar(contador);
                free(proceso);
                contador--;
                printf("Elimino correctamente\n");
            }else{
                sleep(quantum);
                proceso->tiempoEjecutado = proceso->tiempoEjecutado + quantum;
            }
            contador = contador + 1;
        }
    }
    proceso = NULL;
    free(proceso);
}

void setQuantum(int q){
    quantum = q;
}
/*

int main(){
    char datos[] = "5,7,0";
    agregar(datos);
    char datos2[] = "3,1,1";
    agregar(datos2);
    char datos3[] = "4,2,2";
    agregar(datos3);
    char datos4[] = "9,2,3";
    agregar(datos4);
    bool *activo = true;
    pthread_t thread_id;
    setQuantum(3);
    pthread_create(&thread_id, NULL, RR, &activo);
    if(activo){
        printf("Thread prueba\n");
    }
    pthread_join(thread_id, NULL);
    free(activo);
    return 0;
}*/