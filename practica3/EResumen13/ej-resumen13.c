


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mqueue.h>
#include <time.h>
#include <signal.h> 
#include <errno.h>
#include "ej-resumen12-common.h"


int main() 
{
    
    pid_t flag;
    int status;

	mqd_t mq_server;
    mqd_t mq_client;
    
    char writeBuffer[MAX_SIZE];
    char readBuffer[MAX_SIZE];
    
    char serverQueue[100];
    char clientQueue[100];

	struct mq_attr attr;

    int must_stop = 0;
    
    attr.mq_maxmsg = 10;        // Maximo número de mensajes
    attr.mq_msgsize = MAX_SIZE; // Maximo tamaño de un mensaje


    switch ( fork() ){ //CLIENTE

    case -1: // Ocurrió un error al hacer fork()

		perror("No se ha podido crear el proceso hijo...");
		exit(EXIT_FAILURE);
        break;

	case 0:  

    
    	sprintf(serverQueue, "%s-%s", SERVER_QUEUE, getenv("USER"));
        	printf ("[CLIENT]: El nombre de la cola del servidor es: %s\n", serverQueue);
    
    	sprintf(clientQueue, "%s-%s", CLIENT_QUEUE, getenv("USER"));
        	printf ("[CLIENT]: El nombre de la cola del cliente es: %s\n", clientQueue);
    
        mq_server = mq_open(serverQueue, O_WRONLY);
        mq_client = mq_open(clientQueue, O_RDONLY);
    
    	if(mq_server == (mqd_t)-1 )
    	{
          perror("[CLIENT]: Error al abrir la cola del servidor");
          exit(-1);
    	}
    
    	if(mq_client == (mqd_t)-1 )
    	{
          perror("[CLIENT]: Error al abrir la cola del cliente");
          exit(-1);
    	}
    
        printf ("[CLIENT]: El descriptor de la cola del servidor es: %d\n", (int) mq_server);
        printf ("[CLIENT]: El descriptor de la cola del cliente es: %d\n",  (int) mq_client);
    
    	printf("Mandando mensajes al servidor (escribir \"%s\" para parar):\n", MSG_STOP);
    	do
    	{
    		printf("> ");
    
    		fgets(writeBuffer, MAX_SIZE, stdin);
    
    		if(mq_send(mq_server, writeBuffer, MAX_SIZE, 0) != 0)
    		{
    			perror("Error al enviar el mensaje");
    			exit(-1);
    		}
    
    		ssize_t bytes_read;
    
    		bytes_read = mq_receive(mq_client, readBuffer, MAX_SIZE, NULL);
    
    		if(bytes_read < 0)
    		{
    			perror("Error al recibir el mensaje");
    			exit(-1);
    		}
    
    		printf("%s\n", readBuffer);
    
    	}while (strncmp(writeBuffer, MSG_STOP, strlen(MSG_STOP)));
    
    	if(mq_close(mq_server) == (mqd_t)-1)
    	{
    		perror("Error al cerrar la cola del servidor");
    		exit(-1);
    	}
    
    	if(mq_close(mq_client) == (mqd_t)-1)
    	{
    		perror("Error al cerrar la cola del cliente");
    		exit(-1);
    	}
    
		exit(EXIT_SUCCESS);
        //break;














	default:
    
    	sprintf(serverQueue, "%s-%s", SERVER_QUEUE, getenv("USER"));
    
            printf ("[SERVER]: El nombre de la cola del servidor es: %s\n", serverQueue);
    	
    	sprintf(clientQueue, "%s-%s", CLIENT_QUEUE, getenv("USER"));
    
            printf ("[SERVER]: El nombre de la cola del cliente es: %s\n", clientQueue);
    	
    	mq_server = mq_open(serverQueue, O_CREAT | O_RDONLY, 0644, &attr); // ABRIMOS AMBAS COLAS
    	mq_client = mq_open(clientQueue, O_CREAT | O_WRONLY, 0644, &attr);
    
        // COMPROBAMOS QUE NO HAYA ERRORES
    
    	if(mq_server == (mqd_t)-1 ) 
    	{
       	    perror("Error al abrir la cola del servidor");
            exit(-1);
    	}
    
    	if(mq_client == (mqd_t)-1 )
    	{
       	    perror("Error al abrir la cola del cliente");
            exit(-1);
    	}
    
        printf ("[SERVER]: El descriptor de la cola del servidor es: %d\n", (int) mq_server);
        printf ("[SERVER]: El descriptor de la cola del cliente es: %d\n",  (int) mq_client);
    
    	do{
    	
    		ssize_t bytes_read;
    
            // RECIBE EL MENSAJE DEL CLIENTE Y LO GUARDA EN EL BUFFER
    
    		bytes_read = mq_receive(mq_server, readBuffer, MAX_SIZE, NULL);
    
    		if(bytes_read < 0)
    		{
    			perror("Error al recibir el mensaje");
    			exit(-1);
    		}
    
            // COMPARA EL BUFFER CON EL MENSAJE DE DENTENCION
    
    		if( strncmp(readBuffer, MSG_STOP, strlen(MSG_STOP)) == 0 ){ must_stop = 1; }
    
    		else{ // ENTRA SI NO ES EL MENSAJE DE DETENCION
    
    			printf("Recibido el mensaje: %s", readBuffer);
    
                // GUARDA EN EL WRITEBUFFER EL NUMERO DE CARACTERES
    
    			sprintf(writeBuffer, "Número de caracteres recibidos: %ld", (strlen(readBuffer)-1));
    
                // ENVIA A LA COLA CLIENTE EL WRITEBUFFER CON EL TAMANO
    
    			if(mq_send(mq_client, writeBuffer, MAX_SIZE, 0) != 0)
    			{
    				perror("Error al enviar el mensaje");
    				exit(-1);
    			}
            }
    
    	} while (!must_stop); 	
    
        // CIERRA AMBAS COLAS
    
    	if(mq_close(mq_server) == (mqd_t)-1) 
    	{
    		perror("Error al cerrar la cola del servidor");
    		exit(-1);
    	}
    	
    	if(mq_close(mq_client) == (mqd_t)-1)
    	{
    		perror("Error al cerrar la cola del cliente");
    		exit(-1);
    	}
    
        //ELIMINA LAS COLAS
    
    	if(mq_unlink(serverQueue) == (mqd_t)-1)
    	{
    		perror("Error al eliminar la cola del servidor");
    		exit(-1);
    	}
    
    	if(mq_unlink(clientQueue) == (mqd_t)-1)
    	{
    		perror("Error al eliminar la cola del cliente");
    		exit(-1);
    	}			
    
		exit(EXIT_SUCCESS);
    }
































	    while ( (flag=wait(&status)) > 0 ) 
	    {
		    if (WIFEXITED(status)) {
			    printf("Proceso Padre, Hijo con PID %ld finalizado, status = %d\n", (long int)flag, WEXITSTATUS(status));
		    } 
		    else if (WIFSIGNALED(status)) {  //Para seniales como las de finalizar o matar
			    printf("Proceso Padre, Hijo con PID %ld finalizado al recibir la señal %d\n", (long int)flag, WTERMSIG(status));
		    } 		
	    }

	    if (flag==(pid_t)-1 && errno==ECHILD)
	    {
		    printf("Proceso Padre %d, no hay mas hijos que esperar. Valor de errno = %d, definido como: %s\n", getpid(), errno, strerror(errno));
	    }

	    else
	    {
		    printf("Error en la invocacion de wait o waitpid. Valor de errno = %d, definido como: %s\n", errno, strerror(errno));
		    exit(EXIT_FAILURE);
	    }	
		
		exit(EXIT_SUCCESS);	
    

} 