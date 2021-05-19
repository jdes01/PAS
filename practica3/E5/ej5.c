/*

A continuación se verá un primer ejemplo simple en el que se hace uso de dos elementos de POSIX: 
fork() y colas de mensajes. Concretamente el ejemplo permite comunicarse mediante colas de mensajes 
a un proceso principal o main() con un proceso hijo. El código correspondiente se encuentra en el fichero ejemplo-mq.c. 
Ábralo y consúltelo concienzu- damente.

Las primeras líneas de código (previas a la llamada a fork()) son ejecutadas por el proceso original o padre (antes de clonarse):

- Se definen las propiedades de la cola a utilizar (número máximo de mensajes en la cola en un determinado instante y tamaño máximo de cada mensaje).

- En un sistema compartido, debemos asegurar que la cola de mensajes que estamos uti- lizando es única para el usuario. 
Por ejemplo, si dos de vosotros os conectaseis por ssh a ts.uco.es y utilizarais el código del ejemplo, 
los programas de ambos usuarios interactuarían entre si y los resultados no serían los deseados. Para evitar esto, 
el nombre de la cola que utilicéis podría ser el nombre original seguido vuestro nombre de usua- rio, es decir, “nombre_original-usuario”. 
Para obtener el nombre de usuario, se puede consultar la variable de entorno correspondiente.

- Se hace la llamada a fork().

Tras la llamada a fork(), siguiendo la rama del switch correspondiente, el proceso
hijo realiza las siguientes acciones:

- Abre o crea la cola en modo solo escritura (el hijo solo va a escribir).
Si hay que crearla, se le ponen permisos de lectura y escritura al usuario actual.

- Construye el mensaje dentro de la variable buffer, introduciendo un número aleato- rio entre 0 y 4999.
En lugar de transformar el número a cadena, se podría haber enviado directamente, 
realizando un casting del puntero correspondiente ((char *) &numeroAleatorio, y la longitud correcta para un int). 
Esto habría que haberlo tenido en cuenta también en el proceso padre.

- Envía el mensaje por la cola mq, cierra la cola y sale del programa. 
En el caso del proceso padre:

- Abre o crea la cola en modo solo lectura.
Si hay que crearla, se le ponen permisos de lectura y escritura al usuario actual y de solo lectura al resto. 
Recuerde que tanto el padre como el proceso hijo están ejecutando en paralelo en el sistema, 
por lo que cualquiera de los dos puede ser el primero en crear la cola.

- Esperamos a recibir un mensaje por la cola mq. La espera (bloqueante) se prolonga hasta que haya un mensaje 
en la cola, es decir, hasta que el proceso hijo haya realizado el envío.

- Imprimimos el número aleatorio que viene en el mensaje.

- Cierra la cola y, como sabe que nadie más va a utilizarla, la elimina. 
Por último, espe- ramos a que el hijo finalice y salimos del programa.

*/









#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <mqueue.h>
#include <sys/wait.h>
#include <errno.h> //Control de errores
#include <string.h> //Para la funcion strerror()

#define MAX_SIZE    5
#define QUEUE_NAME  "/una_cola"

int main()
{
	// Descriptor de la cola
	mqd_t mq;
	// Buffer para la lectura/escritura
	char buffer[MAX_SIZE];
	// Atributos de la cola
	struct mq_attr attr;
    // Almacena el nombre de la cola
    char queue_name[100];
	// Resultado de las operaciones
	int resultado;
	// Para realizar el fork
    pid_t rf, flag;
    int status;
	// Numero aleatorio a generar
	int numeroAleatorio;
	// Inicializar los atributos de la cola.
	attr.mq_maxmsg = 10;        // Maximo número de mensajes
	attr.mq_msgsize = MAX_SIZE; // Maximo tamaño de un mensaje. Tener en cuenta que debe incluir el '/0'
    // Nombre para la cola. Al concatenar el login, sera unica en un sistema compartido.
    sprintf(queue_name, "%s-%s", QUEUE_NAME, getenv("USER"));

	// Realizar el fork
	rf = fork();
	switch (rf)
	{
		// Error
		case -1:
			printf ("No he podido crear el proceso hijo \n");
			exit(1);

		case 0: // Hijo. El hijo solo se encargará de escribir.

			/* Apertura de la cola
			   O_CREAT: si no existe, se crea
			   O_RDWR: lectura/escritura
			   O_RDONLY: solo lectura
			   O_WRONLY: solo escritura
			   0644: permisos rw-r--r--
			         permisos de lectura y escritura para el propietario, y de sólo lectura para el grupo y para otros
			   attr: estructura con atributos para la cola  */

			mq = mq_open(queue_name, O_CREAT | O_WRONLY, 0644, &attr);

            printf ("[HIJO]: El nombre de la cola es: %s\n", queue_name);
            printf ("[HIJO]: El descriptor de la cola es: %d\n", (int) mq);

			if(mq==-1)
			{
				perror("[HIJO]: Error en la apertura de la cola");
				exit(-1);
			}
			printf ("[HIJO]: Mi PID es %d y mi PPID es %d\n", getpid(), getppid());

			/* Rellenamos el buffer que vamos a enviar
			   Semilla de los números aleatorios, establecida a la hora actual*/
			srand(time(NULL));
			// Número aleatorio entre 0 y 4999
			numeroAleatorio = rand()%5000;
			sprintf(buffer,"%d",numeroAleatorio); // La funcion sprintf escribe en una cadena el valor indicado y añade el '/0'.
			printf("[HIJO]: Generado el mensaje \"%s\"\n", buffer);

			// Mandamos el mensaje
			printf("[HIJO]: Enviando mensaje...\n");

			resultado = mq_send(mq, buffer, MAX_SIZE, 0);

			if(resultado == -1)
			{
				perror("[HIJO]: Error al enviar mensaje");
				exit(-1);
			}

			printf("[HIJO]: Mensaje enviado!\n");
			// Cerrar la cola
			if(mq_close(mq) == -1)
			{
				perror("[HIJO]: Error cerrando la cola");
				exit(-1);
			}
            printf("[HIJO]: Cola cerrada.\n");
			break; //Saldría del switch()

		default: // Padre. El padre solo se encargará de leer

			/* Apertura de la cola */
			mq = mq_open(queue_name, O_CREAT | O_RDONLY, 0644, &attr);

            printf ("[PADRE]: El nombre de la cola es: %s\n", queue_name);
            printf ("[PADRE]: El descriptor de la cola es:%d\n", (int) mq);

			if(mq==-1)
			{
				perror("[PADRE]: Error en la apertura de la cola");
				exit(-1);
			}

			printf ("[PADRE]: Mi PID es %d y el PID de mi hijo es %d \n", getpid(), rf);
			printf ("[PADRE]: Recibiendo mensaje (espera bloqueante)...\n");

			// Recibimos un mensaje a través de la cola
			resultado = mq_receive(mq, buffer, MAX_SIZE, NULL);

			if(resultado <= 0)
			{
				perror("[PADRE]: Error al recibir el mensaje");
				exit(-1);
			}

			// Imprimimos el mensaje recibido
			printf("[PADRE]: El mensaje recibido es \"%s\"\n", buffer);

			// Cerrar la cola
			if(mq_close(mq) == -1)
			{
				perror("[PADRE]: Error cerrando la cola");
				exit(-1);
			}
			printf("[PADRE]: Cola cerrada.\n");

			// Eliminar la cola
			if(mq_unlink(queue_name) == -1)
			{
				perror("[PADRE]: Error eliminando la cola");
				exit(-1);
			}

    	/*Espera del padre a los hijos*/
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
	}
	exit(0);
}