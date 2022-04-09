#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

int burstMenor;
int burstMayor;

int terminarCiclo = 0;

//Obtenido de https://www.codegrepper.com/code-examples/c/c+isnumber
int isNumber(char s[]) {
    for(int i = 0; s[i] != '\0'; i++){
        if (isdigit(s[i]) == 0)
            return 0;
    }
    return 1;
}

// Automatico
void* creadorProcesos() {
    int burst = rand() % (burstMayor - burstMenor) + burstMenor;
    int prioridad = rand() % 5 + 1;
    sleep(2);
    printf("Burst: %d   Prioridad: %d\n", burst, prioridad);
    //Aqui va el enviar mensaje
}

// Automatico
void* creadorThreads() {
    pthread_t tid[200];
    for(int i = 0; terminarCiclo == 0; i++){
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, creadorProcesos, NULL);
        tid[i] = thread_id;
        sleep(rand() % 5 + 3);
    }
    for(int i = 0; i < 200; i++) {
        if(tid[i] == NULL)
            break;
        pthread_join(tid[i], NULL);
    }
}

// Funcion que se encarga de manegar de como funciona el cliente en caso
// de que se seleccione que va a ser automatico
void clienteAutomatico() {
    char inputMenor[5];
    char inputMayor[5];
    char* posicionNulo1;
    char* posicionNulo2;
    printf("Insertar el burst mas pequeno de los bursts: ");
    fgets(inputMenor, sizeof inputMenor, stdin);
    if((posicionNulo1 = strchr(inputMenor, '\n')) != NULL) {
        *posicionNulo1 = '\0';
    }
    if(isNumber(inputMenor) == 0) {
        printf("Input no aceptado\n");
    }
    printf("Insertar el burst mas grande de los bursts: ");
    fgets(inputMayor, sizeof inputMayor, stdin);
    if((posicionNulo2 = strchr(inputMayor, '\n')) != NULL) {
        *posicionNulo2 = '\0';
    }
    if(isNumber(inputMayor) == 0) {
        printf("Input no aceptado\n");
    }
    
    burstMenor = atoi(inputMenor);
    burstMayor = atoi(inputMayor);

    if( burstMenor > burstMayor) {
        printf("Burst menor es mas grande que el bust mayor\n");
    }

    pthread_t thread_id;
    pthread_create(&thread_id, NULL, creadorThreads, NULL);
    getchar();
    terminarCiclo = 1;
    pthread_join(thread_id, NULL);
}

// Funcion que se encarga de manegar el cliente si este selecciona leer
// un archivo para obtener procesos
int clienteManual(){
    //Incluir threads
    char inputUsuario[64];
    char* posicionNulo;
    printf("Insertar el nombre del archivo que desea leer: ");
    if(!(fgets(inputUsuario, sizeof inputUsuario, stdin))){
        printf("No se leyo ningun input");
        return 1;
    }

    if((posicionNulo = strchr(inputUsuario, '\n')) != NULL) {
        *posicionNulo = '\0';
    }

    FILE* archivoProcesos = fopen(inputUsuario, "r");
    char* linea;
    size_t len = 0;
    ssize_t read;

    if(archivoProcesos == NULL) {
        printf("Archivo no se puede encontrar\n");
        return -1;
    }

    while((read = getline(&linea, &len, archivoProcesos)) != -1) {
        printf("Linea: %s\n", linea);
        sleep(rand() % 5 + 3);
    }

    fclose(archivoProcesos);
    if(linea)
        free(linea);
    return 0;
}

int main(){
    srand(time(NULL));
    //Crear menu de seleccion
    bool seleccion = true;
    while(seleccion) {
        printf("Seleccionar cual tipo de cliente se quiere correr\n1. Cliente manual\n2. Cliente Automatico\n");
        char character = getchar();
        switch (character)
        {
        case '1':
            clienteManual();
            seleccion = false;
            break;

        case '2':
            clienteAutomatico();
            seleccion = false;
            break;

        default:
            printf("\n\n\n\n\n\n\nPor favor insertar uno de los numeros presentes\n");
            break;
        }
    }
    return 0;
}