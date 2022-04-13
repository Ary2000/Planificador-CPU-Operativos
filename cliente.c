#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <netdb.h> 
#include <sys/socket.h> 
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h> 

/*#define SERVER_ADDRESS "192.168.0.20" Isaac*/
#define SERVER_ADDRESS  "192.168.0.15"     /* server IP */
#define PORT            8080 

int burstMenor;
int burstMayor;

char buf_rx[400]; //Variable para almacenar el mensaje obtenido por el servidor
void *hiloConexionCliente();
void *hiloClienteManual(void *lineaTXT);


int estadoHilo = 1;
int cliente_conexion_var; //variable global que indica la conexion de cliente servidor

void stop(void){
    estadoHilo = 0;
}

int terminarCiclo = 0;


//Se conecta al servidor desde el lado del cliente
int encenderServidorCliente(){
    int sockfd; 
    struct sockaddr_in servaddr; 
    
    /* Socket creation */
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) 
    { 
        printf("CLIENT: socket creation failed...\n"); 
        return -1;  
    } 
    else
    {
        printf("CLIENT: Socket successfully created..\n"); 
    }
    
    
    memset(&servaddr, 0, sizeof(servaddr));

    /* assign IP, PORT */
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr( SERVER_ADDRESS ); 
    servaddr.sin_port = htons(PORT); 
  
    /* try to connect the client socket to server socket */
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) 
    { 
        printf("connection with the server failed...\n");  
        return -1;
    } 
    
    printf("connected to the server..\n"); 
    return sockfd;
 }

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
    printf("[PROCESS CREATED]: Burst:%d-Prioridad:%d\n", burst, prioridad);
    

    //Aqui va el enviar mensaje
    char buf_tx[500]; 
    sprintf(buf_tx, "1,%d,%d", burst, prioridad);
    /* send test sequences*/
    write(cliente_conexion_var, buf_tx, sizeof(buf_tx));     
    read(cliente_conexion_var, buf_rx, sizeof(buf_rx));
    printf("%s\n", buf_rx); 
    printf("[Finished Process]\n");
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
    stop();
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
    pthread_t threadClienteManual[200];
    int j = 0;
    while((read = getline(&linea, &len, archivoProcesos)) != -1) {
        pthread_t threadClienteManual2;
        pthread_create(&threadClienteManual2, NULL, hiloClienteManual,(void *) linea);
        threadClienteManual[j] = threadClienteManual2;
        j = j + 1;
        sleep(rand() % 5 + 3);
    }
    for (int i = 0; i < j; ++i)
    {
        pthread_join(threadClienteManual[i], NULL);
    }
    stop(); // termina la conexion con el servidor
    fclose(archivoProcesos);
    if(linea)
        free(linea);
    return 0;
}

void *hiloClienteManual(void *lineaTXT)
{
    char *linea;
    linea = (char *) lineaTXT;
    sleep(2);
    //Aqui va el enviar mensaje
    char buf_tx[500]; 
    sprintf(buf_tx, "1,%s",linea);
    printf("[PROCESS CREATED]: (Burst-Prioridad) %s\n",buf_tx);
    write(cliente_conexion_var, buf_tx, sizeof(buf_tx));
    read(cliente_conexion_var, buf_rx, sizeof(buf_rx)); 
    printf("%s\n", buf_rx);
    printf("[Finished Process]\n");
    return NULL;   
}

void *hiloConexionCliente()
{
    //para terminar este hilo se debe ejecutar la funcion stop(); 
    int cliente_conexion = encenderServidorCliente();
    cliente_conexion_var = cliente_conexion;
    while(estadoHilo){
        sleep(1);
    }
    /* close the socket */
    close(cliente_conexion);
    return NULL;
}

void *hiloTerminar(){
    printf("-------------------------------------------\n");
    printf("--- PRESIONAR 3 PARA TERMINAR EJECUCION ---\n");
    printf("-------------------------------------------\n");
    int numTeclado;
    while(1){
        scanf("%d",&numTeclado);
        if(numTeclado==3){
            //stop();
            exit(EXIT_SUCCESS);
        }
    }
    return NULL;

}  

int main(){
    srand(time(NULL));
    pthread_t threadConexionCliente,threadTerminar;
    pthread_create(&threadConexionCliente, NULL, hiloConexionCliente, NULL); 
    pthread_create(&threadTerminar, NULL, hiloTerminar, NULL);
    //Crear menu de seleccion
    bool seleccion = true;
    while(seleccion) {
        printf("Seleccionar cual tipo de cliente se quiere correr\n1. Cliente manual\n2. Cliente Automatico\n");
        char character = getchar();
        while ((getchar()) != '\n');
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
    pthread_join(threadConexionCliente,NULL); //Termina la ejecucion del hilo de cliente
    pthread_join(threadTerminar,NULL);
    return 0;
}

