#Desarrolla un script que reciba como argumentos obligatorios un directorio y un numero en- ´
#tero N. El script debera dividir los ficheros que se encuentren dentro de dicho directorio en ´ N
#partes iguales por orden alfabetico. Los subdirectorios no se tomar ´ an en cuenta, ni tampoco los ´
#ficheros que se encuentren dentro de los subdirectorios. Cada grupo de ficheros se copiara en ´
#una carpeta diferente cuyo nombre incluira el n ´ umero del grupo correspondiente. Si el n ´ umero ´
#de ficheros es demasiado pequeno como para hacer ˜ N grupos, el script debera devolver un error ´
#y no hacer nada.
#Nota: para dividir una lista de ficheros en grupos de tamano˜ N puedes utilizar el comando
#xargs -n $N. Si lo necesitas, busca algo mas de informaci ´ on acerca de este comando

re='[0-9]+$'

if [ -d $1 ]; then
	if [[ "$2" =~ $re ]]; then
		
		i=0

		for x in $(find $1 -maxdepth 1 -type d)
		do
			if [[ "$x" != "$1" ]]; then
				let i=i+1
			fi
		done

		let resto=$i%$2
		let ncarpetas=$i/$2

		if [[ $resto == 0 ]]; then

			for (( it = 1; it <= $i; it++ )); do
				mkdir "group"$it
			done

		fi

	fi
fi

