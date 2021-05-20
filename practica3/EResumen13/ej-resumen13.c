#ifndef COMMON_H_
#define COMMON_H_

#define SERVER_QUEUE  "/server_queue"
#define CLIENT_QUEUE  "/client_queue"
#define MAX_SIZE     1024
#define MSG_STOP     "exit"


#endif // #ifndef COMMON_H_ 


#include <stdio.h>
#include <stdlib.h> //exit, rand, srand
#include <unistd.h> //pipe, close, fork, usleep, read, write, getpid, getppid
#include <sys/wait.h> //wait
#include <errno.h> //Control de errores
#include <string.h> //strerror
#include <time.h> //Para la semilla del generador de aleatorios


int main() 
{
    
    pid_t flag;
    int status;



    switch ( fork() ){ //CLIENTE

    case -1: // Ocurrió un error al hacer fork()

		perror("No se ha podido crear el proceso hijo...");
		exit(EXIT_FAILURE);
        break;

	case 0:  

        printf("[CLIENT] -> \n");

        // Cola del servidor
    	mqd_t mq_server;

    	// Buffer para intercambiar mensajes
    	char buffer[MAX_SIZE];

        // Nombre para la cola
        char serverQueue[100];
    
        // Nombre para la cola. Al concatenar el login sera unica en un sistema compartido.
    	sprintf(serverQueue, "%s-%s", SERVER_QUEUE, getenv("USER"));
        printf ("[CLIENT] El nombre de la cola es: %s\n", serverQueue);
    
        mq_server = mq_open(serverQueue, O_WRONLY);

    	if(mq_server == (mqd_t)-1 ){
    	
          perror("[CLIENT] Error al abrir la cola del servidor");
          exit(-1);
    	}
        printf ("[CLIENT] El descriptor de la cola es: %d\n", (int) mq_server);
    
    	printf("[CLIENT] Mandando mensajes al servidor (escribir \"%s\" para parar):\n", MSG_STOP);
    	do
    	{
    		printf("> ");

    		fgets(buffer, MAX_SIZE, stdin);
    
    		// Enviar y comprobar si el mensaje se manda
    		if(  mq_send(mq_server, buffer, MAX_SIZE, 0) != 0)
    		{
    			perror("[CLIENT] Error al enviar el mensaje");
    			exit(-1);
    		}

            // Número de bytes leidos
    		ssize_t bytes_read;
    
    		// Recibir el mensaje
    		bytes_read = mq_receive(mq_server, buffer, MAX_SIZE, NULL);

    		// Comprobar que la recepción es correcta (bytes leidos no son negativos)
    		if(bytes_read < 0)
    		{
    			perror("[CLIENT] Error al recibir el mensaje");
    			exit(-1);
    		}

            printf("[CLIENT] Recibido el size: %s\n", buffer); 
    	    
    	}while (strncmp(buffer, MSG_STOP, strlen(MSG_STOP)));
    
    	// Cerrar la cola del servidor
    	if(mq_close(mq_server) == (mqd_t)-1)
    	{
    		perror("[CLIENT] Error al cerrar la cola del servidor");
    		exit(-1);
    	}

		exit(EXIT_SUCCESS);

        break;

    }



















        printf("[SERVER]\n");

        // Cola del servidor
    	mqd_t mq_server;

    	// Atributos de la cola
    	struct mq_attr attr;

    	// Buffer para intercambiar mensajes
    	char buffer[MAX_SIZE];

    	// flag que indica cuando hay que parar. Se escribe palabra exit
    	int must_stop = 0;

    	// Inicializar los atributos de la cola
    	attr.mq_maxmsg = 10;        // Maximo número de mensajes
    	attr.mq_msgsize = MAX_SIZE; // Maximo tamaño de un mensaje

        // Nombre para la cola
        char serverQueue[100];

        // Nombre para la cola. Al concatenar el login sera unica en un sistema compartido.
    	sprintf(serverQueue, "%s-%s", SERVER_QUEUE, getenv("USER"));
        printf ("[SERVER] El nombre de la cola es: %s\n", serverQueue);
    
    	// Crear la cola de mensajes del servidor. La cola CLIENT_QUEUE le servira en ejercicio resumen
    	mq_server = mq_open(serverQueue, O_CREAT | O_RDONLY, 0644, &attr);

        if(mq_server == (mqd_t)-1 ){
    	
       	    perror("[SERVER] Error al abrir la cola del servidor");
            exit(-1);
    	}
        printf ("[SERVER] El descriptor de la cola es: %d\n", (int) mq_server);

    	do
    	{
    		// Número de bytes leidos
    		ssize_t bytes_read;
    
    		// Recibir el mensaje
    		bytes_read = mq_receive(mq_server, buffer, MAX_SIZE, NULL);

    		// Comprobar que la recepción es correcta (bytes leidos no son negativos)
    		if(bytes_read < 0)
    		{
    			perror("[SERVER] Error al recibir el mensaje");
    			exit(-1);
    		}
    
    		// Comprobar el fin del bucle
    		if (strncmp(buffer, MSG_STOP, strlen(MSG_STOP))==0){ must_stop = 1; }

    		else{ 
                
                printf("[SERVER] Recibido el mensaje: %s\n", buffer); 

                int n = strlen(buffer);

                sprintf(buffer, "%d", n);

                printf("[SERVER] Enviando size: %s\n", buffer);
    
    		    // Enviar y comprobar si el mensaje se manda
    		    if(  mq_send(mq_server, buffer, MAX_SIZE, 0) != 0)
    		    {
    			    perror("[SERVER] Error al enviar el mensaje");
    			    exit(-1);
    		    }

                printf("[SERVER] size enviado con exito: %s\n", buffer);
            }

    	} while (!must_stop); 	// Iterar hasta que llegue el código de salida, es decir, la palabra exit
    
    	// Cerrar la cola del servidor
    	if(mq_close(mq_server) == (mqd_t)-1)
    	{
    		perror("[SERVER] Error al cerrar la cola del servidor");
    		exit(-1);
    	}
    
    	// Eliminar la cola del servidor
    	if(mq_unlink(serverQueue) == (mqd_t)-1)
    	{
    		perror("[SERVER] Error al eliminar la cola del servidor");
    		exit(-1);
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