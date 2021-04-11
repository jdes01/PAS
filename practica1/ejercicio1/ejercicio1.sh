#script que recibe un argumento que sera un directirio; imprimira el nombre de los ficheros y directorios que contenga el dir especificado
#junto con la longitud de su nombre (sin incluir la ruta) tambien la de todos los subdirectorios; tambien poner caracter / caracteres dependiendo del caso

#!/bin/bash

directorio=$1
if [ -d $directorio ]
then

	for x in $(find $directorio) 
	do
		nombre_sin_ruta=$(basename $x) #basename devuelve el nombre del fichero sin la ruta donde se encuentra

		echo -n "$nombre_sin_ruta <- "	

		caracteres=$(echo -n $nombre_sin_ruta | wc -m )	#cuenta los caracteres

		if [ $caracteres -eq 1 ] #para poner 1 caracater // 2 caracterES
		then

			echo "$caracteres caracter" 
		else

			echo "$caracteres caracteres"
		fi
	done

	for x in $(find $directorio) #find busca archivos y directorios en el directorio especificado;
	do 		                     #si quisieras contar las carpetas en el directorio, sin contar archivos ni carpetas dentro de carpetas: -maxdepth 1 -type d -print| wc

		nombre_sin_ruta=$(basename $x)

		carpeta=$(dirname $x)	 #devuelve la carpeta donde se aloja el fichero

		ruta_absoluta=$(pwd $carpeta) #pwd para devolver la ruta absoluta de la carpeta	

		caracteres=$(echo -n $nombre_sin_ruta | wc -m )	

		#calculo modulo de caracteres para saber si es impar o par		
		let modulo=$caracteres%2

		if [ $modulo != 0 ]
		then	

			#mirando el man stat se pueden sacar todos.
			i_nodo=$(stat -c "i-nodo %i" $x)
			tamanio_bytes=$(stat -c "%s" $x)
			tamanio_kilobytes=$[$tamanio_bytes/1024]
			permiso=$(stat -c "permisos: %a" $x)
			usuario=$(stat -c "%U" $x)
			grupo_usuario=$(stat -c "%G" $x)
	
			echo "$nombre_sin_ruta, $ruta_absoluta, $i_nodo, $tamanio_bytes bytes ($tamanio_kilobytes K), $permiso, $usuario : $grupo_usuario "
		fi	
	done
fi 


