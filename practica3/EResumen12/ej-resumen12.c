//
// Crea dos procesos que se comunican mediante tuberias de manera bidireccional.
//

#include <stdio.h>
#include <stdlib.h> //exit, rand, srand
#include <unistd.h> //pipe, close, fork, usleep, read, write, getpid, getppid
#include <sys/wait.h> //wait
#include <errno.h> //Control de errores
#include <string.h> //strerror
#include <time.h> //Para la semilla del generador de aleatorios



//  #include <stdlib.h>
//  #include <stdio.h>
//  #include <string.h>
//  #include <sys/stat.h>
//  #include <sys/types.h>
//  #include <mqueue.h>
//  #include <time.h>
//  #include <errno.h>
//  #include "common.h"

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

            // Cola del servidor
        	mqd_t mq_server;

        	// Buffer para intercambiar mensajes
        	char buffer[MAX_SIZE];

            // Nombre para la cola
            char serverQueue[100];
        
        	// Abrir la cola del servidor. La cola CLIENT_QUEUE le servira en ejercicio resumen.
        	// No es necesario crearla si se lanza primero el servidor, sino el programa no funciona.
        
            // Nombre para la cola. Al concatenar el login sera unica en un sistema compartido.
        	sprintf(serverQueue, "%s-%s", SERVER_QUEUE, getenv("USER"));
            printf ("[Cliente]: El nombre de la cola es: %s\n", serverQueue);
        
            mq_server = mq_open(serverQueue, O_WRONLY);

            //mq_server = mq_open(SERVER_QUEUE, O_WRONLY);
        	if(mq_server == (mqd_t)-1 )
        	{
              perror("Error al abrir la cola del servidor");
              exit(-1);
        	}
            printf ("[Cliente]: El descriptor de la cola es: %d\n", (int) mq_server);
        
        	printf("Mandando mensajes al servidor (escribir \"%s\" para parar):\n", MSG_STOP);
        	do
        	{
        		printf("> ");
        
        		/* Leer por teclado. Según la documentación, fgets lo hace de esta manera:
        		It stops when either (n-1) characters are read, the newline character is read,
        		or the end-of-file is reached, whichever comes first.
        		Automáticamente fgets inserta el fin de cadena '\0'
        		*/
        		fgets(buffer, MAX_SIZE, stdin);
        
        		// Enviar y comprobar si el mensaje se manda
        		if(mq_send(mq_server, buffer, MAX_SIZE, 0) != 0)
        		{
        			perror("Error al enviar el mensaje");
        			exit(-1);
        		}
        	// Iterar hasta escribir el código de salida
        	}while (strncmp(buffer, MSG_STOP, strlen(MSG_STOP)));
        
        	// Cerrar la cola del servidor
        	if(mq_close(mq_server) == (mqd_t)-1)
        	{
        		perror("Error al cerrar la cola del servidor");
        		exit(-1);
        	}

		exit(EXIT_SUCCESS);

        break;

    }



    switch ( fork() ){ //SERVIDOR

    case -1: // Ocurrió un error al hacer fork()

		perror("No se ha podido crear el proceso hijo...");
		exit(EXIT_FAILURE);
        break;

	case 0:  

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
        printf ("[Servidor]: El nombre de la cola es: %s\n", serverQueue);
    
    	// Crear la cola de mensajes del servidor. La cola CLIENT_QUEUE le servira en ejercicio resumen
    	mq_server = mq_open(serverQueue, O_CREAT | O_RDONLY, 0644, &attr);
        //mq_server = mq_open(SERVER_QUEUE, O_CREAT | O_RDONLY, 0644, &attr);
    
    	if(mq_server == (mqd_t)-1 )
    	{
       		perror("Error al abrir la cola del servidor");
            exit(-1);
    	}
        printf ("[Servidor]: El descriptor de la cola es: %d\n", (int) mq_server);
    
    	do
    	{
    		// Número de bytes leidos
    		ssize_t bytes_read;
    
    		// Recibir el mensaje
    		bytes_read = mq_receive(mq_server, buffer, MAX_SIZE, NULL);

    		// Comprar que la recepción es correcta (bytes leidos no son negativos)
    		if(bytes_read < 0)
    		{
    			perror("Error al recibir el mensaje");
    			exit(-1);
    		}

    		// Cerrar la cadena
    		//buffer[bytes_read] = '\0';
    
    		// Comprobar el fin del bucle
    		if (strncmp(buffer, MSG_STOP, strlen(MSG_STOP))==0)
    			must_stop = 1;
    		else
    			printf("Recibido el mensaje: %s\n", buffer);
				
    	} while (!must_stop); 	// Iterar hasta que llegue el código de salida, es decir, la palabra exit
    
    	// Cerrar la cola del servidor
    	if(mq_close(mq_server) == (mqd_t)-1)
    	{
    		perror("Error al cerrar la cola del servidor");
    		exit(-1);
    	}
    
    	// Eliminar la cola del servidor
    	if(mq_unlink(serverQueue) == (mqd_t)-1)
    	{
    		perror("Error al eliminar la cola del servidor");
    		exit(-1);
    	}

		exit(EXIT_SUCCESS);

        break;

    }



        printf("padre\n");


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
