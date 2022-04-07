#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>


typedef struct{
   struct nodo* siguiente;
   //Anadir un espacio mas para el PID
   int info[2];

}nodo;

nodo* primero = NULL;
nodo* ultimo = NULL;

void agregar(char *datos){
   nodo* dato = malloc(sizeof(nodo)); //Inicializa el nodo
   dato->siguiente = NULL;
   dato->info[0] = 0;
   dato->info[1] = 0;
   char *separador = ",";
   char *token = strtok(datos,separador);
   if(token != NULL){
      dato->info[0] = atoi(token);
      while (token != NULL)
      {
         token = strtok(NULL,separador);
         if(token != NULL){
            dato->info[1] = atoi(token);
         }else{
         }
      }
   }else{
   }

   if(primero == NULL){
      primero = dato;
      ultimo = dato;
   }
   else{
      ultimo->siguiente = dato;
      ultimo = dato;
   }
}

int largoLista(){
   int cont = 0;
   nodo* i = primero;
   while(i != NULL){
      cont++;
      i = i->siguiente;
   }
   return cont;
}

nodo* getPrimero(){
   return primero;
}

nodo* getUltimo(){
   return ultimo;
}

nodo* getElemento(int pos){
   /*if(pos == 0){
      return getPrimero;
   }*/
   nodo* aux = primero;
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

void eliminar(int pos){
   if(primero == NULL){
      return;
   }
   nodo* aux = primero;
   if(pos==0){
      if(primero->siguiente == NULL){
         ultimo = NULL;
         primero = NULL;
      }else{
         primero = primero->siguiente;
         aux->siguiente = NULL;
      }
      free(aux);
   }
   else{
      nodo* prev = NULL;
      for (int i = 0; i < largoLista(); ++i)
      {
         if(i==pos){
            if(pos == largoLista()-1){
               prev->siguiente = NULL;
               ultimo = prev;
            }
            else{
               prev->siguiente = aux->siguiente;
            }
            aux->siguiente = NULL;
            free(aux);
         }
         prev = aux;
         aux = aux->siguiente;
      }

   }
}

void mostrarLista(){
   nodo* i = primero;
   while(i != NULL){
      printf("%i  ,", i->info[0]);
      printf("%i  \n", i->info[1]);
      i = i->siguiente;
   }

}

void liberarLista(){
   if(primero == NULL){
      return;
   }
   nodo* aux = primero;
   primero = NULL;
   while (aux != ultimo)
   {
      nodo* prev = aux;
      aux = aux->siguiente;
      prev->siguiente = NULL;
      free(prev);
   }
   ultimo = NULL;
}


/*int main() {
   printf("Largo de la lista: %d\n", largoLista());
   agregar("1");
   printf("Largo de la lista: %d\n", largoLista());
   mostrarLista();
   // si se quiere agregar datos separados por coma, por algun motivo tiene que ser mediante un array
   char datos[] = "2,7";
   agregar(datos);
   printf("Largo de la lista: %d\n", largoLista());
   mostrarLista();
   char datos2[] = "3,1";
   agregar(datos2);
   mostrarLista();
   printf("Largo de la lista: %d\n", largoLista());
   eliminar(0);
   eliminar(0);
   printf("Largo de la lista: %d\n", largoLista());
   mostrarLista();
   nodo *prueba = getElemento(0);
   printf("Prueba de getElemento: %d\n", prueba->info[0]);
   prueba = getUltimo();
   printf("Prueba de getElemento: %d\n", prueba->info[0]);
   prueba = getPrimero();
   printf("Prueba de getElemento: %d\n", prueba->info[0]);
   free(prueba);
   liberarLista();
	return 0;
}*/
