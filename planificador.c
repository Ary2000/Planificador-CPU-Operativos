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
   char* nombre;

}nodo;

nodo* primero = NULL;
nodo* ultimo = NULL;

void agregar(char nombreAux[]){
   nodo* dato = malloc(sizeof(nodo)); //Inicializa el nodo
   dato->nombre = nombreAux;
   dato->siguiente = NULL;

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

/*void eliminar(int pos){
   nodo* aux = primero;
   if(pos==0){
      primero = primero->siguiente;
   }
   else{
      for (int i = 0; i < largoLista(); ++i)
      {
         if(i==pos){
            nodo* aeliminar = aux->siguiente;
            if(pos == largoLista()-1){
               ultimo = aux
               aux->siguiente = NULL
            }
            else{
               aux->siguiente = aeliminar->siguiente
            }
            int cont=1;
            while(cont<pos){
             aux = aux->siguiente;
             cont++;
            }
            delete(aeliminar)
            aux = aux->siguiente;
            aux->siguiente = aux->siguiente;
         }
         aux = aux->siguiente;
      }

   }
   
}*/

void mostrarLista(){
   nodo* i = primero;
   while(i != NULL){
      printf("%s  \n", i->nombre);
      i = i->siguiente;
   }

}


int main() {
   agregar("Juan1");
   agregar("Juan2");
   agregar("Juan3");
   printf("Largo de la lista: %d\n", largoLista());
   //eliminar(0);
   mostrarLista();
	return 0;
}
