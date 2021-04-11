#Desarolla un script que muestre por pantalla información acerca de los sistemas de archivos que hay montados. 
#Para ello, puedes utilizar el comando df. La información que se mostrará será la siguiente:
#1. Sistema de ficheros.
#2. Punto de montaje.
#3. Tamaño.
#4. Espacio libre.
#5. Porcentaje de uso.
#La salida deberá estar en un formato similar al que se muestra en el siguiente ejemplo:


#!/bin/bash

df | sed -r -n -e '2,$s/^([a-Z0-9/:\\]+)[ ]+([0-9]+)[ ]+([0-9]+)[ ]+([0-9]+)[ ]+([0-9]+%)[ ]+([a-Z0-9/]+)/El sistema de ficheros \1, esta montado en \6, tiene un tamaño de \2 de los cuales se estan usando \3 que representan un porcentaje de \5 del total. Quedan libres \4.\n/p'



# df nos muestra informacion a cerca del sistema de archivos que hay montado en el equipo

# "df" output ---->>

## S.ficheros     bloques de 1K   Usados Disponibles Uso% Montado en
## udev                 4026372        0     4026372   0% /dev
## tmpfs                 811188     2100      809088   1% /run
## /dev/sda4           15415240 13219792     1392684  91% /
## tmpfs                4055924   536192     3519732  14% /dev/shm
## tmpfs                   5120        4        5116   1% /run/lock
## tmpfs                4055924        0     4055924   0% /sys/fs/cgroup
## /dev/loop2            101632   101632           0 100% /snap/core/10958
## /dev/loop1            204288   204288           0 100% /snap/code/61
## /dev/loop3             56832    56832           0 100% /snap/core18/1988
## /dev/loop4             56832    56832           0 100% /snap/core18/1997
## /dev/loop5             77568    77568           0 100% /snap/discord/121
## /dev/loop7             52352    52352           0 100% /snap/snap-store/518
## /dev/loop6             78592    78592           0 100% /snap/discord/122
## /dev/loop8             33152    33152           0 100% /snap/snapd/11588
## /dev/loop0            204288   204288           0 100% /snap/code/60
## /dev/loop11            51968    51968           0 100% /snap/snap-store/481
## /dev/loop9            224256   224256           0 100% /snap/gnome-3-34-1804/66
## /dev/loop13           101632   101632           0 100% /snap/core/10908
## /dev/loop10            63616    63616           0 100% /snap/gtk-common-themes/1506
## /dev/loop12           462208   462208           0 100% /snap/phpstorm/212
## /dev/loop14            33152    33152           0 100% /snap/snapd/11402
## /dev/loop15           165376   165376           0 100% /snap/gnome-3-28-1804/128
## /dev/loop16           223232   223232           0 100% /snap/gnome-3-34-1804/60
## /dev/loop17           415744   415744           0 100% /snap/phpstorm/208
## /dev/loop18           166784   166784           0 100% /snap/gnome-3-28-1804/145
## /dev/loop19            66432    66432           0 100% /snap/gtk-common-themes/1514
## /dev/sdb1          491102112 25244208   440841584   6% /home
## tmpfs                 811184       60      811124   1% /run/user/1000


##############################################################################################
##############################################################################################

# sed toma un stream de texto y lo edita como queramos
	
	# sed -r usa el formato ERE (mas completo) 
	# sed -n elimina el printeo automatico del "space pattern"
	# sed -e "add the script to the commands to be executed"

##############################################################################################
##############################################################################################

# '2,$s/^ ([a-Z0-9/:\\]+)[ ] + ([0-9]+)[ ] + ([0-9]+)[ ] + ([0-9]+)[ ] + ([0-9]+%)[ ] + 
# ([a-Z0-9/]+) /El sistema de ficheros \1, esta montado en \6 ..........  \4.\n/p'

# ([a-Z0-9/:\\]+)[ ]  ----->> texto con numeros y barras mas un espacio, se asigna a \1