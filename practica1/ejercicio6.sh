#!/bin/bash

# Desarolla un script que reciba como argumento el nombre de un fichero de texto que contendra 
# diferentes nombres de dominio, uno por linea. El script debera resolver los diferentes
# nombres de dominio y mostrar por pantalla la IP obtenida seguida del nombre de dominio que
# se proporcionaba.
# Los resultados deberan estar formateados en dos columnas y ordenados seg ´ un la direcci ´ on´
# IP, de manera que se respete el orden de las mismas (primero se ordena por la primera cifra de
# la ip, despues por la segunda, por la tercera...). Por ejemplo, la IP 31.22.56.233 debe preceder a la ´
# 127.0.0.1.
# Puedes usar el comando resolveip para resolver un dominio. Echa un vistazo a sus opciones.

if [ $# -ne 1 ] # igual que $1 es el argumento 1, $# seria como el argc
then

	echo "El script debe recibir un argumento obligatoriamente."
	exit 1
fi

while read direccion #lee por terminal y lo guarda en la variable direccion
do
	
	ip=$(dig $direccion +short) #Comando para resolver la ip
	echo -e "$ip\t\t\t$direccion" #Imprimimos por pantalla la ip con su correspondiente dirección

done < $1 #cuando hacemos while para leer ficheros usamos esto
