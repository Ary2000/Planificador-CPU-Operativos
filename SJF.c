#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
//#include "planificador.c"

void *SJF(void* arg){
    bool* activo = (bool *)arg;
    nodo* proceso;
    while(*activo){
        printf("En el while\n");
        if(largoLista() == 0){
            sleep(1);
            //*activo = false;
        }else{
            int posProceso = getPosNextProcess(true);
            proceso = getElemento(posProceso);
            printf("En el while\n");
            printf("%i  ,", proceso->info[0]);
            printf("%i  ,", proceso->info[1]);
            printf("%i\n", proceso->info[2]);
            sleep(proceso->info[0]);
            printf("Vuelta\n");
            eliminar(posProceso);
        }
    }
}
/*
int main(){
    char datos[] = "5,7,1";
    agregar(datos);
    char datos2[] = "3,1,2";
    agregar(datos2);
    char datos3[] = "4,2,2";
    agregar(datos3);
    bool *activo = true;
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, SJF, &activo);
    if(activo){
        printf("Thread prueba\n");
    }
    pthread_join(thread_id, NULL);
    free(activo);
    return 0;
}*/