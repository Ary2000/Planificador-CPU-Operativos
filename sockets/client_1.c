/*************************************************************************************/
/* @file    client_1.c                                                               */
/* @brief   This clients connects,                                                   */
/*          sends a text, reads what server and disconnects                          */
/*************************************************************************************/

#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <unistd.h>

#define SERVER_ADDRESS  "192.168.0.15"     /* server IP */
#define PORT            8080 

/* Test sequences */     
char buf_rx[100];                     /* receive buffer */

void *hiloConexionCliente();


int estadoHilo = 1;
int cliente_conexion_var;

void stop(void){
    estadoHilo = 0;
}

 
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



/* This clients connects, sends a text and disconnects */
int main() 
{


    pthread_t threadConexionCliente;
    
    
    pthread_create(&threadConexionCliente, NULL, hiloConexionCliente, NULL); 
     
    pthread_join(threadConexionCliente,NULL);
   


    
    //char buf_tx[] = "hola todo bien"; 
  
    /* send test sequences*/
    //write(cliente_conexion, buf_tx, sizeof(buf_tx));     
    //read(cliente_conexion, buf_rx, sizeof(buf_rx));
    //printf("CLIENT:Received: %s \n", buf_rx); 
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
 
