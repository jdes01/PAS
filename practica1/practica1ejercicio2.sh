#Desarrolla un script que reciba como argumentos obligatorios un directorio y un numero en- ´
#tero N. El script debera dividir los ficheros que se encuentren dentro de dicho directorio en ´ N
#partes iguales por orden alfabetico. Los subdirectorios no se tomar ´ an en cuenta, ni tampoco los ´
#ficheros que se encuentren dentro de los subdirectorios. Cada grupo de ficheros se copiara en ´
#una carpeta diferente cuyo nombre incluira el n ´ umero del grupo correspondiente. Si el n ´ umero ´
#de ficheros es demasiado pequeno como para hacer ˜ N grupos, el script debera devolver un error ´
#y no hacer nada.
#Nota: para dividir una lista de ficheros en grupos de tamano˜ N puedes utilizar el comando
#xargs -n $N. Si lo necesitas, busca algo mas de informaci ´ on acerca de este comando


if [ -d $1 ] && [ $2 -gt 0 ]
then

	echo "correcto"

	for a in $(ls $1)
	do

		let "x = $x + 1"
	done

	echo "Hay $x ficheros y directorios en ${1}"

	if [ $2 -gt $x ] # -gt greater than
	then

		echo "Error, el numero de archivos es menor que el numero de grupos"
		exit
	fi

	if [ $[x%$2] -ne 0 ] # -ne not equal
	then

		echo "No se puede dividir en partes iguales"
		exit
	fi

else

	echo "fallaste"
	exit
fi

let size=$[x/${2}]
echo "el tamaño de grupo es $size"

mkdir groups

for var in $( seq $2 ) #seq print a sequence of numbers "seq 5" -> 1 2 3 4 5
do

	mkdir "groups/group$var"
done

a=0

for b in $(ls $1)
do

	if [ -f $b ]
	then

		echo $b
		cp $b groups/group$x #Copiamos el fichero a su correspondiente carpeta

		if [ $(find groups/group$x -maxdepth 1 -type f | wc -l) -eq $size ] #Si llenamos una carpeta, modificamos x para seguir añadiendo ficheros a otra carpeta
		then

			let "a = $a + 1"
		fi
	fi
	
done
