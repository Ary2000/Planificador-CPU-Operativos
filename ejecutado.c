#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
//#include "planificador.c"

typedef struct{
   struct proceso* siguiente;
   //Anadir un espacio mas para el PID
   int info[3];
   //int star, finish;
   int TAT, WT;
}proceso;

proceso* start = NULL;
proceso* finish = NULL;

int tiempoInicial;
int tiempoFinal;
void setInicial(){
   tiempoInicial = time(NULL);
}

void finalizo(nodo *node){
   proceso* dato = malloc(sizeof(proceso)); //Inicializa el proceso
   dato->siguiente = NULL;
   dato->info[0] = node->info[0];
   dato->info[1] = node->info[1];
   dato->info[2] = node->info[2];
   //dato->star = node->star;
   //dato->finish = node->finish;
   dato->TAT = node->finish - node->star;
   dato->WT = dato->TAT - dato->info[1];

   if(start == NULL){
      start = dato;
      finish = dato;
   }
   else{
      finish->siguiente = dato;
      finish = dato;
   }
   free(node);
}

proceso* getStart(){
   return start;
}

proceso* getUltimoProcesoFinalizado(){
   return finish;
}

proceso* getElementoProcesoFinalizado(int pos){
   proceso* aux = start;
   int cont = 0;
   while (aux != NULL)
   {
      if(cont == pos){
         return aux;
      }else{
         aux = aux->siguiente;
      }
      cont = cont + 1;
   }
   return NULL;
}

void mostrarEjecutados(){
   proceso* i = start;
   while(i != NULL){
      /*printf("[%i,", i->info[0]);
      printf("%i,", i->info[1]);
      printf("%i]->", i->info[2]);*/
      printf("TAT: %i,", i->TAT);
      printf("WT: %i\n", i->WT);
      i = i->siguiente;
   }
   printf("\n");
}

void liberarProcesos(){
   if(start == NULL){
      return;
   }
   proceso* aux = start;
   start = NULL;
   while (aux != finish)
   {
      proceso* prev = aux;
      aux = aux->siguiente;
      prev->siguiente = NULL;
      free(prev);
   }
   finish = NULL;
   free(aux);
}

void lenProcesosEjecutados(){
   int cont = 0;
   proceso* i = start;
   while(i != NULL){
      cont++;
      i = i->siguiente;
   }
   printf("[Executed processes]: %d\n",cont);
}
void cpuOcioso(){
   tiempoFinal = time(NULL);
   int sumBurst = 0;
   int total = 0;
   proceso* i = start;
   while(i != NULL){
      sumBurst = sumBurst + i->info[1];
      i = i->siguiente;
   }
   total = (tiempoFinal - tiempoInicial) - sumBurst;
   printf("Cantidad de segundos con CPU ocioso: %d\n",total);
   
}
void tablaTAT_WT(){
   proceso* i = start;
   printf("---------------------------\n");
   printf("-------TABLA TAT Y WT------\n");
   printf("---------------------------\n");
   while(i != NULL){
      printf("--- PID: %d - TAT: - %d WT: %d ---\n",i->info[0],i->TAT,i->WT);
      i = i->siguiente;
   }
   printf("---------------------------\n");
   
}
void promedioTAT_WT(){
   int cont = 0;
   int sumTAT = 0;
   int sumWT = 0;
   double promedioTAT,promedioWT;
   proceso* i = start;
   while(i != NULL){
      cont++;
      sumTAT = sumTAT + i->TAT;
      sumWT  = sumWT  + i->WT;
      i = i->siguiente;
   }
   promedioTAT = sumTAT / cont;
   promedioWT = sumWT / cont;
   printf("Promedio Turn Around Time: %f -- Promedio Waiting Time: %f\n",promedioTAT,promedioWT);
}
