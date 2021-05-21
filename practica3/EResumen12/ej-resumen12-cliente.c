/*
Cliente que envia por una cola abierta para escritura una cadena de caracteres
recogida por teclado, mientras que el valor de esa cadena sea distinto a la palabra exit
*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mqueue.h>
#include <time.h>
#include <errno.h>
#include <signal.h>   
#include "ej-resumen12-common.h"


void sigint(int signal){

    printf("Capturé la señal SIGINT y no salgo!\n");
    return;
}

void sigterm(int signal){

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

	mqd_t mq_server;
	mqd_t mq_client;

	char writeBuffer[MAX_SIZE];
	char readBuffer[MAX_SIZE];

   char serverQueue[100];
   char clientQueue[100];

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
		funcionLog(writeBuffer);

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

	return 0;
}











void funcionLog(char *mensaje)
{
	int resultado;

	char nombreFichero[100];
	char mensajeAEscribir[300];

	time_t t;

	sprintf(nombreFichero,"log-cliente.txt");

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