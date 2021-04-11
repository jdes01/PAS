#!/bin/bash

#Crear un script que permita automatizar un sistema de copias de seguridad. El sistema debera´
#funcionar de la siguiente forma:
#Recibira´ 3 par´ametros obligatorios: carpeta de origen, carpeta de destino y un numero ´
#entero N.
#Se guardaran un ´ m ´aximo de N copias, de manera que cuando haya N y se quiera hacer
#una nueva, se debera borrar la m ´ as antigua. ´
#Las copias se almacenaran en subdirectorios dentro del directorio de destino siguiendo la ´
#numeracion´ 1, 2, 3, ..., N. La copia m ´as antigua siempre estar´a en el subdirectorio 1.
#Cuando una copia antigua se borre, se deberan renombrar el resto de directorios para seguir ´
#manteniendo la copia mas antigua en el 1. ´
#A continuacion se muestra un ejemplo en el que se ejecuta el ´ script varias veces, partiendo de
#un directorio de backups vac´ıo.

if [ -d $1 ]; then #arg 1 dir
    if [ -d $2 ]; then #arg 2 dir
        if [ $3 -gt 0 ]; then #arg 3 dir

            N=$3

            mkdir "$2/backup"

            if [ -z "$(ls -A "$2/backup")" ]; then

            	echo "hola"
                mkdir -p "$2/backup/1" #no salta error si ya existe
                cp -r $1 "$2/backup/1" #copia recursiva (el dir y todo lo que haya dentro)
                exit
            fi

            i=$(find "$2/backup" -maxdepth 1 -type d -print| wc -l)
            echo "$i"

            if [ $N -gt  $i ]; then

            	let x=$N-$i  #PARA HACER OPERACIONES MATEMATICAS SIEMPRE USAMOS LET
            	let x=$N-$i+1

                mkdir "$2/backup/$x"
                cp -r $1 "$2/backups/$x"
                exit
            fi
            
        fi
    fi
fi