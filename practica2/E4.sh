#Crea un script que proporcione información acerca del uso de memoria del equipo. Puedes uti-
#lizar los comandos grep y sed para obtener la información necesaria del fichero /proc/meminfo.
#El script debe mostrar por pantalla la siguiente información:
#1. Memoria total.
#2. Memoria libre.
#3. Memoria disponible.
#4. Memoria en uso como caché de disco.
#5. Tamaño total de la memoria swap.
#6. Memoria swap libre.
#Además, el script recibirá un argumento númerico opcional. Este argumento, cuyo valor por
#defecto será 10, se utilizará para mostrar el Top-N procesos que más memoria están utilizando.
#Para ello, puedes utilizar la salida del comando ps aux.

#!/bin/bash
echo -n "Memoria total: " && cat /proc/meminfo | awk 'FNR == 1 {print $2 $3}'
echo -n "Memoria libre: " && cat /proc/meminfo | awk 'FNR == 2 {print $2 $3}'
echo -n "Memoria disponible: " && cat /proc/meminfo | awk 'FNR == 3 {print $2 $3}'
echo -n "Memoria en uso como caché de disco: " && cat /proc/meminfo | awk 'FNR == 5 {print $2 $3}'
echo -n "Tamaño total de la memoria swap: " && cat /proc/meminfo | awk 'FNR == 15 {print $2 $3}'
echo -n "Memoria swap libre: " && cat /proc/meminfo | awk 'FNR == 16 {print $2 $3}'

echo -e

aux=$1
if [ $aux -lt 10 ]
then
    aux=10
else
    aux=$1
fi

echo "Top-$aux  procesos en uso de memoria."
echo "=================================================================="
echo "Usuario                 %Uso                      Proceso"
echo "=================================================================="

ps aux --sort -rss | awk 'NR>1{print $1,"\t \t \t" $4, "\t \t \t   " $11}' | head -n $aux