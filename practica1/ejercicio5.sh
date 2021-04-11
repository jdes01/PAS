#5. ejercicio5.sh
#Haz un script que permita encontrar una contrasena a partir de su hash SHA-256. Se sabe que ˜
#la contrasena es un numero hexadecimal (0-9, A-F) y que su longitud no es superior a 5 caracteres.

#El script debera probar todas las combinaciones posibles hasta encontrar la clave.

#Para generar el hash de una cadena puedes utilizar:
# echo "cadena" | sha256sum

#Se recomienda utilizar el operador brace expansion para generar las diferentes combinaciones.
#Para expandir una expresion que se encuentre en una variable, puedes hacerlo de la siguiente ´
#forma:
# eval echo $string

#El hash proporcionado se encuentra dentro del fichero hash.txt que se proporciona junto
#con la practica. 

#Una ejecucion del ´ script debe producir una salida similar a:
# i42vayuv@NEWTS:˜/PAS$ ./ejercicio5.sh
# Probando 16 combinaciones de longitud 1
# Probando 256 combinaciones de longitud 2
# Probando 4096 combinaciones de longitud 3
# Probando 65536 combinaciones de longitud 4
# Se ha encontrado la contrasena: 54AD
#¿De que forma incrementa el numero de combinaciones al aumentar el largo de la contrasena?
#¿Y al incluir diferentes tipos de caracteres? 



#!/bin/bash
hash="$(cat "hash.txt" | cut -d " " -f 1)" #cut sirve para eliminar elementos de un texto. como en el txt es "" blablabla - " 
                                           #queremos eliminar espacios y guion para quedarnos con blablabla
                                           #asique decimos cut, -d que es el delimitador y le decimos que use el espacio,
                                           # y usando -f le decimos que queremos que retorne el elemento 1 (el texto)


#echo {{0..9},{A..F}}
comb=16

#########################

echo "Probando las $comb combinaciones de longitud 1"

for i in $(eval echo {{0..9},{A..F}}) #eval es un comando de posix que toma un argumento y crea un comando con el. Lo explico mas abajo 
do
    #echo "i = $i"
    try="$(echo $i | sha256sum)"

    if [ "$try" -eq "$hash" ] # -eq equal, igual
    then 

        echo "Encontrado! es $i"
        exit
    fi

done

let "comb = $comb * 16"

#########################

echo "Probando las $comb combinaciones de longitud 2"

for i in $(eval echo {{0..9},{A..F}}{{0..9},{A..F}})
do
    #echo "i = $i"
    try="$(echo $i | sha256sum)"

    if [ "$try" -eq "$hash" ]
    then 

        echo "Encontrado! es $i"
        exit
    fi

done

let "comb = $comb * 16"

#########################

echo "Probando las $comb combinaciones de longitud 3"

for i in $(eval echo {{0..9},{A..F}}{{0..9},{A..F}}{{0..9},{A..F}})
do
    #echo "i = $i"
    try="$(echo $i | sha256sum)"

    if [ "$try" -eq "$hash" ]
    then 

        echo "Encontrado! es $i"
        exit
    fi

done

let "comb = $comb * 16"

#########################

echo "Probando las $comb combinaciones de longitud 4"

for i in $(eval echo {{0..9},{A..F}}{{0..9},{A..F}}{{0..9},{A..F}}{{0..9},{A..F}})
do
    #echo "i = $i"
    try="$(echo $i | sha256sum)"

    if [ "$try" -eq "$hash" ]
    then 

        echo "Encontrado! es $i"
        exit
    fi

done


let "comb = $comb * 16"

#########################

echo "Probando las $comb combinaciones de longitud 5"

for i in $(eval echo {{0..9},{A..F}}{{0..9},{A..F}}{{0..9},{A..F}}{{0..9},{A..F}}{{0..9},{A..F}})
do
    #echo "i = $i"
    try="$(echo $i | sha256sum)"

    if [ "$try" -eq "$hash" ]
    then 

        echo "Encontrado! es $i"
        exit
    fi

done


#########################################3

# eval --> "crea un comando de algo concatenando expresiones"
#
#  Ejemplo:
#
##  1) foo=10 x=foo
##  2) y='$'$x
##  3) echo $y
##  4) $foo
##  5) eval y='$'$x
##  6) echo $y
##  7) 10