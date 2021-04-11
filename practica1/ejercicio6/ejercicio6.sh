#!/bin/bash

if [ $# -ne 1 ]
then

	echo "El script debe recibir un argumento obligatoriamente."
	exit 1
fi

while read direccion
do
	
	ip=$(resolveip $direccion | cut -d' ' -f6)
	echo -e "$ip\t\t\t$direccion"
done < $1
