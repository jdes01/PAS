#!/bin/bash

who | awk '{printf "Usuario " $1 " conectado " $4 " horas y ausente " $5 "\n" }'

# w imprime:  javier   :0           2021-04-11 17:20 (:0)

# imprimiria mas users (en mi pc solo hay uno)

# awk uso: awk options program file

# 	opciones: -F fs     Para especificar un separador de archivos.
#  			  -f file     Para especificar un archivo que contenga un script awk.
#             -v var=value     Para declarar una variable.

# Para definir un script awk utilizamos llaves rodeadas por unas comillas simples 
# como se muestra a continuación:
	
#	  awk '{print "Welcome to awk command tutorial "}'


# con $n usamos como variables elementos del texto ( $0 representa toda la cadena )

# texto: "Hola soy Javi"

#	awk '{print $0}' texto  --->> "Hola soy Javi"  
#	awk '{print $1}' texto  --->> "Hola"
#	awk '{print $3}' texto  --->> "Javi" 

# echo "Hello Tom" | awk '{$2="Adam"; print $0}'   --->> Hello Adam