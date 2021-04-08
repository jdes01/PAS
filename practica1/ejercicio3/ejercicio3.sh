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

re='[0-9]+$'

if [ -d $1 ]; then #arg 1 dir
	if [ -d $2 ]; then #arg 2 dir
		if [[ "$3" =~ $re ]]; then #arg 3 dir
		fi
	fi
fi