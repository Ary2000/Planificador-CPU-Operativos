#include <time.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "planificador.c"

time_t* inicio = NULL;

int prueba(){
    clock_t inicio2 = inicio;
}

int main(){
    // No quiere ser global, da un problema de que el inicializador no es una constante
    time_t start = time(NULL); 
    inicio = &start;
    prueba();
    sleep(10);
    time_t end = time(NULL);
    printf("El tiempo transcurrido es: %d\n", end-*inicio);
    return 0;

}