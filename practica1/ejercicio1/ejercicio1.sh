#script que recibe un argumento que sera un directirio; imprimira el nombre de los ficheros y directorios que contenga el dir especificado
#junto con la longitud de su nombre (sin incluir la ruta) tambien la de todos los subdirectorios; tambien poner caracter / caracteres dependiendo del caso

#!/bin/bash

for x in $(find $1)
do
	echo "$x -> $(echo "$x" | wc -m) caracteres"

done

for x in $(find $1)
do
	resto="$(echo "$x" | wc -m)%2"
	if [[ $resto -ne 0 ]]; then
		y="${x##*/}"
		let kbs=$(stat -c '%s' $y)/1024
		echo "$y " " ruta:" $(realpath $y) " i-nodo:" $(stat -c '%i' $y) $(stat -c '%s' $y) "bytes" "kb: $kbs" "permisos:" $(stat -c '%a' $y) "($(stat -c '%U' $y) : $(stat -c '%G' $y))"   
	fi

done
