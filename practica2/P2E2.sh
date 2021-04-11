#!/bin/bash

## Desarrolla un script que consulte el fichero /etc/passwd y utilice los comandos grep y sed
## para:

## 1. Mostrar aquellos usuarios que tengan asignado un interprete de comandos valido.

## 2. Mostrar aquellos usuarios que tengan un directo home asignado.

## 3. Mostrar los usuarios y el nombre de su grupo principal en dos columnas, como se muestra
## 		en el ejemplo. Si un usuario no tiene ningun grupo asignado, deber ´ a mostrar un guion en
## 		la posicion correspondiente.


echo -e "\n1) Usuarios con un interprete de comandos valido:\n"
grep -w 'bash\|sh' /etc/passwd # grep -w -> Select only those lines containing matches that form whole words

echo -e "\n2) Usuarios con un home valido:\n"
grep -v nonexistent /etc/passwd # grep -v -> Invert the sense of matching, to select non-matching lines

echo -e "\n3) Usuarios y su grupo:\n"

echo "Usuario                              Grupo"
echo "__________________________________________"
echo "                                          "

while IFS=: read -r f1 f2 f3 f4 f5 f6 f7
do 

	printf "%20s %20s\n" $f1 $f5
done < /etc/passwd
