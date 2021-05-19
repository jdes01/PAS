#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>

int main(int argc, char **argv) {
    int c;
    opterr = 0;

    // Declaracion de las estructuras que almacenarán la informarción de un usuario y de un grupo

    char *lgn;
    int uid;

    char *gname;
    int gid;

    char *name;
    struct passwd *pw;
    struct group *gr;

    // Estructura a utilizar por getoptlong

    static struct option long_options[] = {

        {"user",      required_argument, NULL, 'u'},
        {"group",     required_argument, NULL, 'g'},
        {"active",    no_argument,       NULL, 'a'},
        {"maingroup", no_argument,       NULL, 'm'},
        {"allgroups", no_argument,       NULL, 's'},
        {"help",      no_argument,       NULL, 'h'},

        {0, 0, 0, 0}
	};

    //Estas variables servirán para almacenar el resultado de procesar la línea de comandos 

    char *uvalue = NULL;
    char *gvalue = NULL;

    bool aflag = false;
    bool mflag = false;
    bool sflag = false;
    bool hflag = false;

    while ( (c= getopt_long(argc, argv, "u:g:amsh", long_options, NULL)) != -1 ){

        switch (c) {
        
        case 'u':
            uvalue = optarg;
            break;
       
        case 'g':
            gvalue = optarg;
            break;

        case 'a':
            aflag = true;
            break;

        case 'm':
            mflag = true;
            break;

        case 's':
            sflag = true;
            break;

        case 'h':
            hflag = true;
            break;
        }
    }

    // Imprimir el resto de argumentos de la línea de comandos que no son opciones con "-"

    if (optind < argc) {

        printf("Argumentos del argv que no son opciones: ");
        
        while (optind < argc){ printf("%s ", argv[optind++]); }
        putchar('\n');
    }

    if (hflag)
    {
        printf("Uso del programa: ejercicio1 [opciones]\n");
        printf("Opciones:\n");
        printf("-h, --help Imprimir esta ayuda:\n");
        printf("-u, --user (<nombre>|<uid>) Información sobre el usuario\n");
        printf("-a, --active Información sobre el usuario activo actual\n");
        printf("-m, --maingroup Además de info de usuario, imprimir la info de su grupo principal\n");
        printf("-g, --group (<nombre>|<gid>) Información sobre el grupo\n");
        printf("-s, --allgroups Muestra info de todos los grupos del sistema\n");
    }
    else
    {
        if (uvalue != NULL)
        {
            if (isdigit(*uvalue)){

                uid = atoi(uvalue);

                if ((pw = getpwuid(uid)) == NULL){ //DEVUELVE LA ESTRUCTURA TRAS RECIBIR lgn COMO PARÁMETRO

                    fprintf(stderr, "Fallo al obtener información de usuario.\n");
                    exit(1);
                }
            }

            else{
            
                lgn = uvalue;

                if ((pw = getpwnam(lgn)) == NULL){ //DEVUELVE LA ESTRUCTURA TRAS RECIBIR lgn COMO PARÁMETRO
                
                    fprintf(stderr, "Fallo al obtener información de usuario.\n");
                    exit(1);
                }
            }
            
            
            if (mflag)
            {
                //Aqui ya se dispone de la estructura que contiene informacion del usuario

                printf("Usuario:\n");
                printf("Nombre: %s\n",   strtok(pw->pw_gecos, ",")); //No es lo mismo el nombre de usuario asociado a un login que el propio login
                printf("Login: %s\n",    pw->pw_name);
                printf("Password: %s\n", pw->pw_passwd);
                printf("UID: %d\n",      pw->pw_uid);
                printf("Home: %s\n",     pw->pw_dir);
                printf("Shell: %s\n",    pw->pw_shell);
                printf("Número de grupo principal: %d\n", pw->pw_gid);

                // Obtenemos la estructura de información del grupo a través del número de grupo al que pertenece el usuario
                gr = getgrgid(pw->pw_gid);

                //Se imprime el campo de la estructura que nos interesa
                printf("Grupo:\n");
                printf("Nombre del grupo principal: %s\n", gr->gr_name);
                printf("GID: %d\n", gr->gr_gid);
                printf("Miembros secundarios: %s\n", *gr->gr_mem);
                exit(0);
            }

            else
            {
                //Aqui ya se dispone de la estructura que contiene informacion del usuario
                printf("Usuario:\n");
                printf("Nombre: %s\n",   strtok(pw->pw_gecos, ",")); //No es lo mismo el nombre de usuario asociado a un login que el propio login
                printf("Login: %s\n",    pw->pw_name);
                printf("Password: %s\n", pw->pw_passwd);
                printf("UID: %d\n",      pw->pw_uid);
                printf("Home: %s\n",     pw->pw_dir);
                printf("Shell: %s\n",    pw->pw_shell);
                printf("Número de grupo principal: %d\n", pw->pw_gid);

                // Obtenemos la estructura de información del grupo a través del número de grupo al que pertenece el usuario
                gr = getgrgid(pw->pw_gid);

                //Se imprime el campo de la estructura que nos interesa
                printf("Nombre del grupo principal: %s\n", gr->gr_name);

                exit(0);
            }
        }

        else if (aflag)
        {
            if ((lgn = getenv("USER")) == NULL || (pw = getpwnam(lgn)) == NULL){ 
            
                fprintf(stderr, "Fallo al obtener información de usuario.\n");
                exit(1);
            }

            if (mflag){
            
                //Aqui ya se dispone de la estructura que contiene informacion del usuario
                printf("Usuario:\n");
                printf("Nombre: %s\n",   strtok(pw->pw_gecos, ",")); //No es lo mismo el nombre de usuario asociado a un login que el propio login
                printf("Login: %s\n",    pw->pw_name);
                printf("Password: %s\n", pw->pw_passwd);
                printf("UID: %d\n",      pw->pw_uid);
                printf("Home: %s\n",     pw->pw_dir);
                printf("Shell: %s\n",    pw->pw_shell);
                printf("Número de grupo principal: %d\n", pw->pw_gid);

                // Obtenemos la estructura de información del grupo a través del número de grupo al que pertenece el usuario
                gr = getgrgid(pw->pw_gid);

                //Se imprime el campo de la estructura que nos interesa
                printf("Grupo:\n");
                printf("Nombre del grupo principal: %s\n", gr->gr_name);
                printf("GID: %d\n", gr->gr_gid);
                printf("Miembros secundarios: %s\n", *gr->gr_mem);
                exit(0);
            }

            else
            {
                //Aqui ya se dispone de la estructura que contiene informacion del usuario
                printf("Usuario:\n");
                printf("Nombre: %s\n",   strtok(pw->pw_gecos, ",")); //No es lo mismo el nombre de usuario asociado a un login que el propio login
                printf("Login: %s\n",    pw->pw_name);
                printf("Password: %s\n", pw->pw_passwd);
                printf("UID: %d\n",      pw->pw_uid);
                printf("Home: %s\n",     pw->pw_dir);
                printf("Shell: %s\n",    pw->pw_shell);
                printf("Número de grupo principal: %d\n", pw->pw_gid);

                // Obtenemos la estructura de información del grupo a través del número de grupo al que pertenece el usuario
                gr = getgrgid(pw->pw_gid);

                //Se imprime el campo de la estructura que nos interesa
                printf("Nombre del grupo principal: %s\n", gr->gr_name);

                exit(0);
            }
        }
        
        else if (gvalue != NULL)
        {
            if (isdigit(*gvalue))
            {
                gid = atoi(gvalue);

                if ((gr = getgrgid(gid)) == NULL) //DEVUELVE LA ESTRUCTURA TRAS RECIBIR lgn COMO PARÁMETRO
                {
                    fprintf(stderr, "Fallo al obtener información de usuario.\n");
                    exit(1);
                }
            }

            else
            {
                gname = gvalue;

                if ((gr = getgrnam(gname)) == NULL) //DEVUELVE LA ESTRUCTURA TRAS RECIBIR lgn COMO PARÁMETRO
                {
                    fprintf(stderr, "Fallo al obtener información de usuario.\n");
                    exit(1);
                }
            }
            
            //Se imprime el campo de la estructura que nos interesa
            printf("Grupo:\n");
            printf("Nombre del grupo principal: %s\n", gr->gr_name);
            printf("GID: %d\n", gr->gr_gid);
            printf("Miembros secundarios: %s\n", *gr->gr_mem);
            exit(0);
        }
        
        else if (sflag)
        {
            int num;
            FILE *f;

            // use appropriate location if you are using MacOS or Linux
            f = fopen("/etc/group","r");

            if(f == NULL){
            
               printf("Error al abrir el fichero /etc/group");   
               exit(1);             
            }

            char buffer[256];
            char *groupname;
            char *groupid;
            char *groupmembers;

            printf("Grupo:\n");

            while (fgets(buffer, 256, f) != NULL){
            
                groupname = strtok(buffer, ":");
                groupid =   strtok(NULL,   ":");

                strtok(NULL, ":");

                groupmembers = strtok(NULL, ":");

                //Se imprime el campo de la estructura que nos interesa

                printf("Nombre del grupo principal: %s\n", groupname);
                printf("GID: %s\n", groupid);
                printf("Miembros secundarios: %s", groupmembers);
            }
            
        }
        else
        {
            if ((lgn = getenv("USER")) == NULL || (pw = getpwnam(lgn)) == NULL){
            
                fprintf(stderr, "Fallo al obtener información de usuario.\n");
                exit(1);
            }

            //Aqui ya se dispone de la estructura que contiene informacion del usuario
            printf("Usuario:\n");
            printf("Nombre: %s\n",   strtok(pw->pw_gecos, ",")); //No es lo mismo el nombre de usuario asociado a un login que el propio login
            printf("Login: %s\n",    pw->pw_name);
            printf("Password: %s\n", pw->pw_passwd);
            printf("UID: %d\n",      pw->pw_uid);
            printf("Home: %s\n",     pw->pw_dir);
            printf("Shell: %s\n",    pw->pw_shell);
            printf("Número de grupo principal: %d\n", pw->pw_gid);

            // Obtenemos la estructura de información del grupo a través del número de grupo al que pertenece el usuario
            gr = getgrgid(pw->pw_gid);
            
            printf("Grupo:\n");
            //Se imprime el campo de la estructura que nos interesa
            printf("Nombre del grupo principal: %s\n", gr->gr_name);
            printf("GID: %d\n", gr->gr_gid);
            printf("Miembros secundarios: %s\n", *gr->gr_mem);
            exit(0);
        }
        
    }   

    exit(0);
}