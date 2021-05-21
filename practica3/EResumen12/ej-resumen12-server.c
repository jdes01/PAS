/*

Implemente un programa en C que utilice colas de mensajes y comunique dos procesos,
de forma que cumpla los siguientes requisitos (puede utilizar como base el código de los ficheros 
common.h, servidor.c y cliente.c que se le han proporcionado como ejemplo):

1. Hay un proceso cliente que enviará cadenas leídas desde teclado y las envía mediante
mensajes a un proceso servidor cada vez que pulsamos INTRO.

2. El servidor recibirá los mensajes y contará el número de caracteres recibidos exceptuando el fin de cadena 
(un espacio en blanco se considerará un carácter). Tras esto, el
servidor mandará un mensaje al cliente, por otra cola distinta, con la cadena “Número
de caracteres recibidos: X”, siendo X el número de caracteres calculados.
Por tanto habrá dos colas, ambas creadas por el servidor:

	a) Una cola servirá para que el cliente le envíe al servidor las cadenas de texto.
	De esta cola leerá el servidor para obtener dichas cadenas y analizarlas para contar
	el número de caracteres que tienen.

	b) Otra cola por la que el servidor enviará al cliente el número de caracteres calculados en la cadena de texto recibida por la primera cola.
	De esta segunda cola leerá el cliente para mostrar el número de caracteres calculados que le ha enviado el servidor.
	Se han de tener en cuenta los siguientes items:
	La cola de mensajes para el texto “Número de caracteres recibidos: X”, enviados
	desde el servidor al cliente, se creará y eliminará por parte del servidor (que siempre es el primero en lanzarse) y la abrirá el cliente.
	Si el servidor tiene cualquier problema en su ejecución deberá mandar el mensaje
	de salida, para forzar al cliente a parar.

3. Asegurar que el nombre de las colas sea diferente para su ejecución en un sistema
compartido. Puede usar la idea de anexar el login al nombre de la cola.

4. En el código de que se dispone en Moodle (ficheros common.h, servidor.c y cliente.c),
tanto el cliente como el servidor tienen incluidas unas funciones de log. Estas funciones
implementan un pequeño sistema de registro o log. Utilizándolas se registran en ficheros de texto los mensajes que los programas van mostrando por pantalla (log-servidor.txt
y log-cliente.txt).

Por ejemplo, si queremos registrar en el cliente un mensaje simple, haríamos la siguiente llamada:
1 funcionLog("Error al abrir la cola del servidor");
Si quisiéramos registrar un mensaje más complejo (por ejemplo, donde incluimos el
mensaje recibido a través de la cola), la llamada podría hacerse del siguiente modo:
char msgbuf[100];
2 ...
3 sprintf(msgbuf,"Recibido el mensaje: %s\n", buffer);
4 funcionLog(msgbuf);
Utilice estas llamadas para dejar registro en fichero de texto de todos los mensajes que
se muestren por pantalla en la ejecución del cliente y el servidor, incluidos los errores
que se imprimen por consola.
5. El programa cliente capturará las señales SIGTERM y SIGINT para gestionar adecuadamente el fin del programa servidor y de él mismo. Puede asociar estas señales con
una misma función que pare el programa.
Dicha función deberá, en primer lugar, registrar la señal capturada (y su número
entero) en el fichero de log del cliente.
El cliente, antes de salir, deberá mandar a la cola correspondiente, un mensaje de
fin de sesión (que debe interpretar el servidor), que hará que el otro extremo deje
de esperar mensajes. Este mensaje también se registrará en los logs.
Se deberá cerrar, en caso de que estuvieran abiertas, aquellas colas que se estén
utilizando y el fichero de log.

*/




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

void sigint(int signal)
{
    printf("Capturé la señal SIGINT y no salgo!\n");
    return;
}

void sigterm(int signal)
{
    int i;

    printf("Capturé la señal SIGTERM y voy a salir de manera ordenada\n");

    for(i=0; i<3; i++)
    {
        printf("Hasta luego... cerrando ficheros...\n");

        sleep(1);
    }
    exit(0);
}

void funcionLog(char *);

FILE *fLog = NULL;

int main(int argc, char **argv)
{

	mqd_t mq_server; // COLAS
	mqd_t mq_client;

	struct mq_attr attr;

	char readBuffer[MAX_SIZE];  // BUFFERS
	char writeBuffer[MAX_SIZE];

	int must_stop = 0;

	attr.mq_maxmsg = 10;        // Maximo número de mensajes
	attr.mq_msgsize = MAX_SIZE; // Maximo tamaño de un mensaje

    char serverQueue[100];
    char clientQueue[100];

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
			funcionLog(writeBuffer);
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

	return 0;
}







void funcionLog(char *mensaje)
{
	int resultado;

	char nombreFichero[100];
	char mensajeAEscribir[300];

	time_t t;

	sprintf(nombreFichero,"log-servidor.txt");

	if(fLog==NULL)
	{
		fLog = fopen(nombreFichero,"at");
        
		if(fLog==NULL)
		{
			perror("Error abriendo el fichero de log");
			exit(1);
		}
	}

	t = time(NULL);

	struct tm * p = localtime(&t);

	strftime(mensajeAEscribir, 1000, "[%Y-%m-%d, %H:%M:%S]", p);

	sprintf(mensajeAEscribir, "%s ==> %s\n", mensajeAEscribir, mensaje);

	resultado = fputs(mensajeAEscribir,fLog);

	if (resultado < 0){	perror("Error escribiendo en el fichero de log"); }

	fclose(fLog);
	fLog=NULL;
}