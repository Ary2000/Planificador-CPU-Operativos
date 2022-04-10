#include "planificador.c"
#include "FIFO.c"
#include "HPF.c"
#include "RoundRobin.c"
#include "SJF.c"

/***************************************************************************************/
/* @file    server_secuencial.c                                                        */
/* @brief   Secuencial server. TCP sockets                                             */
/***************************************************************************************/

/*standard symbols */
#include <unistd.h>  

/* sockets */
#include <netdb.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <arpa/inet.h>

/* strings / errors*/
#include <errno.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

/* server parameters */
#define SERV_PORT       8080              /* port */
#define SERV_HOST_ADDR "192.168.0.15"     /* IP, only IPV4 support  */
#define BUF_SIZE        100               /* Buffer rx, tx max size  */
#define BACKLOG         5                 /* Max. client pending connections  */



int  len_rx, len_tx = 0;                     /* received and sent length, in bytes */
char buff_tx[BUF_SIZE];
char buff_rx[BUF_SIZE];   /* buffers for reception  */


int encenderServidorPrincipal()
{

	int sockfd, connfd ;  /* listening socket and connection socket file descriptors */
    unsigned int len;     /* length of client address */
    struct sockaddr_in servaddr, client; 
     
    /* socket creation */
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) 
    { 
        fprintf(stderr, "[SERVER-error]: socket creation failed. %d: %s \n", errno, strerror( errno ));
        return -1;
    } 
    else
    {
        printf("[SERVER]: Socket successfully created..\n"); 
    }
    
    /* clear structure */
    memset(&servaddr, 0, sizeof(servaddr));
  
    /* assign IP, SERV_PORT, IPV4 */
    servaddr.sin_family      = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR); 
    servaddr.sin_port        = htons(SERV_PORT); 
    
    
    /* Bind socket */
    if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) 
    { 
        fprintf(stderr, "[SERVER-error]: socket bind failed. %d: %s \n", errno, strerror( errno ));
        return -1;
    } 
    else
    {
        printf("[SERVER]: Socket successfully binded \n");
    }
  
    /* Listen */
    if ((listen(sockfd, BACKLOG)) != 0) 
    { 
        fprintf(stderr, "[SERVER-error]: socket listen failed. %d: %s \n", errno, strerror( errno ));
        return -1;
    } 
    else
    {
        printf("[SERVER]: Listening on SERV_PORT %d \nPara terminar la conexion dar ENTER\n\n", ntohs(servaddr.sin_port) ); 
    }
    
    len = sizeof(client); 
  
      /* Accept the data from incoming sockets in a iterative way */
      while(1)
      {
        connfd = accept(sockfd, (struct sockaddr *)&client, &len); 
        if (connfd < 0) 
        { 
            fprintf(stderr, "[SERVER-error]: connection not accepted. %d: %s \n", errno, strerror( errno ));
            return -1;
        } 
        else
        {              
            while(1) /* read data from a client socket till it is closed */ 
            {

                /* read client message, copy it into buffer */
                len_rx = read(connfd, buff_rx, sizeof(buff_rx));  
                
                if(len_rx == -1)
                {
                    fprintf(stderr, "[SERVER-error]: connfd cannot be read. %d: %s \n", errno, strerror( errno ));
                }
                else if(len_rx == 0) /* if length is 0 client socket closed, then exit */
                {
                    printf("[SERVER]: client socket closed \n\n");
                    close(connfd);
                    break; 
                }
                else
                {
                    write(connfd, buff_tx, strlen(buff_tx));
                    printf("[SERVER]: %s \n", buff_rx);
                }            
            }  
        }                      
    }
    return 0; 
}

void *hiloConexionServidor()
{
    //para terminar este hilo se debe ejecutar la funcion stop(); 
    int servidor_conexion = encenderServidorPrincipal();
    return NULL;
}
void enviarMensaje(char cadena[]){
	sprintf(buff_tx,"%s", cadena);
}
int largo_cadena(char cadena[])
{
	int largo=0;
	while (cadena[largo]!='\0') 
		largo++;
	return largo;
}

void *jobScheduler(){
	char mensaje[50];
	char vacio[10]="";
	while(1){
		//Si el buffer de lectura es diferente a vacio
		if(strcmp(buff_rx,"")!=0){
			agregar(buff_rx);
			sprintf(mensaje,"La lista tiene un largo de %d \n", largoLista());
			enviarMensaje(mensaje);
			sprintf(buff_rx,"%s",vacio);
		}
	}
	return NULL;

}

int main(int argc, char* argv[])          
{
	pthread_t threadConexionServidor,threadJobScheduler;
    pthread_create(&threadConexionServidor, NULL, hiloConexionServidor, NULL);
    pthread_create(&threadJobScheduler, NULL, jobScheduler, NULL);
    //algoritmos
    pthread_t thread_id;
    bool *activo = true;
    if(strcmp(argv[1],"FIFO") == 0){
    	printf("...FIFO...\n");
	    pthread_create(&thread_id, NULL, FIFO, &activo);
    }
    else if(strcmp(argv[1],"SJF") == 0){
    	printf("...SJF...\n");
	    pthread_create(&thread_id, NULL, SJF, &activo);	   
    }
    else if(strcmp(argv[1],"HPF") == 0){
    	printf("...HPF...\n");
	    pthread_create(&thread_id, NULL, HPF, &activo);
    }
    else if(strcmp(argv[1],"RoundRobin") == 0){
    	printf("...Round Robin...\n");
	    setQuantum(3);
	    pthread_create(&thread_id, NULL, RR, &activo);
    }
    else{
    	printf("Error al ingresar parametro\n Porfavor ingrese un parametro: ./servidor parametro \n siendo parametro(FIFO,SJF,HPF,RoundRobin)");
    }
    free(activo);
    pthread_join(thread_id, NULL);
    pthread_join(threadJobScheduler,NULL); //Termina la ejecucion del hilo de cliente
    pthread_join(threadConexionServidor,NULL);
	return 0;
} 