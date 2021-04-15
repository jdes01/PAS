#! /bin/bash

## Crear un script que reciba como unico argumento obligatorio un fichero de texto que con- ´
## tendra una serie de posibles contrase ´ nas, una por cada linea. El programa debera evaluar todas ´
## las contrasenas para determinar su seguridad. 
## Esta seguridad se indicara con un numero de puntos

## Los factores a tener en cuenta para su evaluacion son los siguientes: ´

## 1. Si la longitud de la contrasena es menor de 5, la contrasena no es segura y debera tener ´
## siempre 0 puntos.
## 2. A partir de longitud 8, se sumaran 5 puntos. ´
## 3. A partir de longitud 15, en lugar de 5, se sumaran 20 puntos. ´
## 4. Si contiene al menos 1 letra minuscula, se suman 10 puntos. 
## 5. Si contiene al menos 1 letra mayuscula, se suman 10 puntos. ´
## 6. Si contiene al menos 1 numero, se ºan 5 puntos. ´
## 7. Si contiene algun car ´ acter especial, se suman 15 puntos. ´

## 8. Adicionalmente, se sumara 1 punto por cada numero, 2 puntos por cada letra y 3 puntos ´
## por cada simbolo que contenga.

## La salida del programa debe estar en formato tabla y debe contener las columnas:

## Contrasena. ˜
## Numero de letras min ´ usculas. ´
## Numero de letras may ´ usculas. ´
## Cantidad de nuneros. ´
## Numero de caracteres especiales. ´
## Puntuacion total.


if [ $# -ne 1 ]
then

	echo "Falta 1 argumento."
	exit 1
fi

echo -e "\t  contraseña\t\tminusculas\tmayusculas\tnúmeros\t\tsímbolos\tpuntos"
echo -e "\t===================================================================================================== "

for x in $(cat $1)
do	
	# echo a la contraseña, seleccionamos lo que queremos evaluar, eliminamos \n y contamos palabras

	a=$(echo  "$x" | tr -d "\n") # tr ---> translate or delete characters; -d -> delete

	b=$(echo  "$x" | grep --color -o -E '[a-z]+' | tr -d "\n" | wc -m )  # grep -o -> only matching characters; -E -> usa ERE
	
	c=$(echo  "$x" | grep --color -o -E '[A-Z]+' | tr -d "\n" | wc -m )

	d=$(echo  "$x" | grep --color -o -E '[0-9]+' | tr -d "\n" | wc -m )

	e=$(echo  "$x" | grep --color -o -E '[[:punct:]]+' | tr -d "\n" | wc -m  )

	let "suma = $b + $c + $d + $e "

	let total=0


	if [ $suma-gt 8 ]
	then

		total=5
	fi

	if [ $suma-gt 15 ]
	then

		total=20
	fi


	if [ $b -gt 0 ] ##minusculas
	then

		let "total = $total + 10 "
		let "total = $total + $b*2 "
	fi
	
	if [ $c -gt 0 ] ##mayusculas
	then

		let "total = $total + 10 "
		let "total = $total + $c*2 "
	fi

	if [ $d -gt 0 ] ##numeros
	then

		let "total = $total + 5 "
		let "total = $total + $d "
	fi	
	
	if [ $e -gt 0 ] ##simbolos
	then

		let "total = $total + 15 "
		let "total = $total + $e*3 "
	fi



	if [ $suma -lt 5 ]
	then

		total=0
	fi
	
	
	printf " %23s \t    %3d \t %3d \t\t %3d \t\t %3d \t\t %3d\n" $a $b $c $d $e $total

done | sort -k 6 -n -r
