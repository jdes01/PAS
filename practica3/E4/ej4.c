/*
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
int fildes[2];
const int BSIZE = 100;
char buf[BSIZE];
ssize_t nbytes;


status = pipe(fildes);
if (status == -1 ) 
{
	perror("Error en pipe"); 
	exit(EXIT_FAILURE);
}

switch (fork()) 
{
	case -1: // Ocurrió un error al hacer fork()
		perror("No se ha podido crear el proceso hijo...");
		exit(EXIT_FAILURE);

	case 0: // El hijo lee desde la tubería, no necesitamos escribir.
		if (close(fildes[1]) == -1) 
		{
			perror("Error en close"); 
			exit(EXIT_FAILURE);
		}
	
		// Leer usando READ. Es una llamada bloqueante.
		nbytes = read(fildes[0], buf, BSIZE);
		if (nbytes == -1)
		{
			perror("Error en read"); 
			exit(EXIT_FAILURE);
		}
		else if (nbytes == 0) //read() no ha leido nada -> Se llegaría a FEOF porque el padre habría cerrado la tuberia.
			printf("[HIJO]: Detecto que mi padre ha cerrado la tuberia...\n"); 
		else
			printf("[HIJO]: Leido %s de la tuberia.\n", buf); 

		if (close(fildes[0]) == -1) //Se termina de leer -> Cerramos filfes[0]
		{
			perror("Error en close"); 
			exit(EXIT_FAILURE);
		}
		else
			printf("[HIJO]: Tuberia cerrada.\n"); 

		exit(EXIT_SUCCESS);

	
	default: // El padre escribe en la tubería
		
		// No se necesita leer
		if (close(fildes[0]) == -1) 
		{
			perror("Error en close"); 
			exit(EXIT_FAILURE);
		}

		// Escribimos datos en la tubería
		if (write(fildes[1], "Hola Mundo!!", 14) == -1)
		{
			perror("Error en write"); 
			exit(EXIT_FAILURE);
		}
	
		// El hijo verá FEOF si posterior a esta llamada close del padre hiciera una read.
		if (close(fildes[1]) == -1) 
		{
			perror("Error en close"); 
			exit(EXIT_FAILURE);
		}
		else
			printf("[PADRE]: Tuberia cerrada.\n"); 
			
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
	}//switch		 
}
*/


































/*
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <errno.h> //Control de errores
#include <string.h> //Para la funcion strerror(), que permite describir el valor de errno como cadena.

//**************************************************************************************************
//Cuando la comunicación es simple y ambos extremos de la comunicación han sido generados
//con un fork() o pertenecen al mismo proceso, se pueden utilizar tuberías anónimas (pipes).
//
//Implemente un programa que cree un proceso hijo mediante fork(), y usando tuberías realice
//lo siguiente:
//
//El proceso padre deberá generar cinco números aleatorios y cada uno de ellos irá en un mensaje
//independiente y lo introducirá en la tubería, mientras que el proceso hijo lo leerá de la tubería.
//
//Ejemplo de salida esperada (tenga en cuenta que depende del planificador):
//jfcaballero@NEWTS:~$ ./a.out
//[PADRE]: Mi PID es 3612 y el PID de mi hijo es 3613 
//[PADRE]: Escribo el número aleatorio 2028 en la tubería...
//[HIJO]: Mi PID es 3613 y mi PPID es 3612
//[PADRE]: Escribo el número aleatorio 2179 en la tubería...
//[HIJO]: Leo el número aleatorio 2028 de la tubería.
//[PADRE]: Escribo el número aleatorio 4716 en la tubería...
//[HIJO]: Leo el número aleatorio 2179 de la tubería.
//[PADRE]: Escribo el número aleatorio 2044 en la tubería...
//[HIJO]: Leo el número aleatorio 4716 de la tubería.
//[PADRE]: Escribo el número aleatorio 2788 en la tubería...
//[HIJO]: Leo el número aleatorio 2044 de la tubería.
//[HIJO]: Leo el número aleatorio 2788 de la tubería.
//[PADRE]: Tubería cerrada...
//[HIJO]: Tubería cerrada ...
//[PADRE]: Hijo con PID 3613 finalizado, status = 0
//[PADRE]: Valor de errno = 10, definido como: No child processes
//*************************************************************************************************



int main() 
{
	// Para realizar el fork
	pid_t rf;
	int flag, status;
	// Para controlar los valores devueltos por las funciones (control de errores)
	int resultado;
	// Lo que vamos a leer y escribir de la tubería
	int numeroAleatorio;
	// Descriptores de los dos extremos de la tubería
	int fileDes[2];
	// Iterador
	int i=0;

	// Creamos la tubería
	resultado = pipe(fileDes);
	if(resultado==-1)
	{
		printf("\nERROR al crear la tubería.\n");
		exit(1);
	}
	
	rf = fork();
	switch (rf)
	{
		case -1:
			printf ("No se ha podido crear el proceso hijo...\n");
			exit(EXIT_FAILURE);
		case 0:
			printf ("[HIJO]: Mi PID es %d y mi PPID es %d\n", getpid(), getppid());
			
			close(fileDes[1]);
	
			for(i=0; i<5; i++)
			{
				//Recibimos un mensaje a través de la cola
				resultado = read( fileDes[0], &numeroAleatorio, sizeof(int));
				
				if(resultado != sizeof(int))
				{
					printf("\n[HIJO]: ERROR al leer de la tubería...\n");
					exit(EXIT_FAILURE);
				}
				// Imprimimos el campo que viene en la tubería
				printf("[HIJO]: Leo el número aleatorio %d de la tubería.\n", numeroAleatorio);
			}		
			// Cerrar el extremo que he usado
			printf("[HIJO]: Tubería cerrada ...\n");
			close(fileDes[0]);
			break;

		default:
			printf ("[PADRE]: Mi PID es %d y el PID de mi hijo es %d \n", getpid(), rf);
			
			close(fileDes[0]);
			
			srand(time(NULL)); // Semilla de los números aleatorios establecida a la hora actual
			
			for(i=0; i<5; i++)
			{			
				// Rellenamos los campos del mensaje que vamos a enviar
				numeroAleatorio = rand()%5000; //Número aleatorio entre 0 y 4999
				
				printf("[PADRE]: Escribo el número aleatorio %d en la tubería...\n", numeroAleatorio);
				
				// Mandamos el mensaje
				resultado = write( fileDes[1], &numeroAleatorio, sizeof(int));
				
				if(resultado != sizeof(int))
				{
					printf("\n[PADRE]: ERROR al escribir en la tubería...\n");
					exit(EXIT_FAILURE);
				}
			}
			
			// Cerrar el extremo que he usado
			close(fileDes[1]);
			printf("[PADRE]: Tubería cerrada...\n");
			
			//Espera del padre a los hijos
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
	exit(EXIT_SUCCESS);
}
*/



















































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

int main() 
{
pid_t flag;
int status, status_1, status_2, num, num2;
int fildes_1[2], fildes_2[2];
const int BSIZE = 100;
char buf[BSIZE], mensaje[10];
ssize_t nbytes;


status_1 = pipe(fildes_1);
status_2 = pipe(fildes_2);

if (status_1 == -1 ) 
{
	perror("Error en pipe 1"); 
	exit(EXIT_FAILURE);
}
else if (status_2 == -1 ) 
{
	perror("Error en pipe 2"); 
	exit(EXIT_FAILURE);
}

switch (fork()) 
{
	case -1: // Ocurrió un error al hacer fork()

		perror("No se ha podido crear el proceso hijo...");
		exit(EXIT_FAILURE);

	case 0: 

		if (close(fildes_1[1]) == -1) // En esta tubería solo va a leer
		{
			perror("Error en close"); 
			exit(EXIT_FAILURE);
		}
		if (close(fildes_2[0]) == -1) // En esta tubería solo va a escribir
		{
			perror("Error en close"); 
			exit(EXIT_FAILURE);
		}
	
		// Leer usando READ
		nbytes = read(fildes_1[0], buf, BSIZE);

		if (nbytes == -1)
		{
			perror("Error en read");
			exit(EXIT_FAILURE);
		}
		else if (nbytes == 0) //read() no ha leido nada -> Se ha llegado a EOF -> El padre ha cerrado la tuberia
			printf("[HIJO]: Detecto que mi padre ha cerrado la tuberia 1...\n"); 

		else
		{
			printf("[HIJO]: Leido %s de la tuberia 1.\n", buf);
			int cont = 0;
			char * token;
			char * token_2;
			token = strtok(buf, ";"); //Leo el primer número
			token_2 = strtok(NULL,";"); //Leo el segundo número

			for(int d=1; d <= atoi(token);d++)
			{
    			if(atoi(token)%d==0)
        			cont++;
    		}

    		if(cont==2) //El primer numero es primo
    		{ 
    			cont = 0;

    			for(int d=1; d <= atoi(token_2);d++) //Calculamos que el segundo número sea primo.
    			{
    				if(atoi(token_2)%d==0)
        				cont++;
    			}

    			if(cont==2) //El segundo numero es primo 
    			{ 
    				if(atoi(token_2) - atoi(token) == 2	) //Son primos gemelos.
    					sprintf(mensaje, "gemelos");

    				else //Ambos son primos, pero no gemelos
    					sprintf(mensaje, "primos");
    			}

    			else //El segundo numero no es primo
    				sprintf(mensaje, "no-primos");
    		}

    		else //El primer numero no es primo
    			sprintf(mensaje, "no-primos");
		}
		
		if (close(fildes_1[0]) == -1) //Se termina de leer -> Cerramos filfes[0]
		{
			perror("Error en close"); 
			exit(EXIT_FAILURE);
		}
		else
			printf("[HIJO]: Tuberia 1 cerrada.\n"); 


		if ( write(fildes_2[1], mensaje, 10) == -1)
		{
			perror("Error en write"); 
			exit(EXIT_FAILURE);
		}
		printf("[HIJO]: He escrito en la tuberia 2.\n");


		if (close(fildes_2[1]) == -1) //Se termina de leer -> Cerramos filfes[0]
		{
			perror("Error en close"); 
			exit(EXIT_FAILURE);
		}
		else
			printf("[HIJO]: Tuberia 2 cerrada.\n"); 

		exit(EXIT_SUCCESS);

	
	default:
		
		if (close(fildes_1[0]) == -1) // No se necesita leer de la tubería 1
		{
			perror("Error en close"); 
			exit(EXIT_FAILURE);
		}

		if (close(fildes_2[1]) == -1) // No se necesita escribir en la tubería 2
		{
			perror("Error en close"); 
			exit(EXIT_FAILURE);
		}

		printf("[PADRE]: Inserte dos números enteros para determinar si son primos gemelos, primos o alguno de los dos no es primo.\n");
		printf("[PADRE]: ");
		scanf("%d", &num);
		printf("[PADRE]: ");
		scanf("%d", &num2);
		sprintf(buf, "%d;%d", num, num2); //mete ambos numeros en el buffer buf

		// Escribimos datos en la tubería
		if ( write(fildes_1[1], buf, 10) == -1)
		{
			perror("Error en write"); 
			exit(EXIT_FAILURE);
		}
		printf("[PADRE]: He escrito los dos números en la tuberia 1.\n");
	
		// El hijo verá FEOF (por hacer close)
		if (close(fildes_1[1]) == -1) 
		{
			perror("Error en close"); 
			exit(EXIT_FAILURE);
		}
		else
			printf("[PADRE]: Tuberia 1 cerrada.\n"); 


		nbytes = read(fildes_2[0], mensaje, 10);
		if (nbytes == -1)
		{
			perror("Error en read");
			exit(EXIT_FAILURE);
		}
		else if (nbytes == 0) //read() no ha leido nada -> Se ha llegado a EOF -> El padre ha cerrado la tuberia
			printf("[PADRE]: Detecto que mi hijo ha cerrado la tuberia 2...\n"); 
		else
			printf("[PADRE]: Leido %s de la tuberia 2.\n", mensaje);
		

		if (close(fildes_2[0]) == -1) //Se termina de leer -> Cerramos filfes[0]
		{
			perror("Error en close"); 
			exit(EXIT_FAILURE);
		}
		else
			printf("[PADRE]: Tuberia 2 cerrada.\n"); 


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
	}//switch		 
}

