#!/bin/bash

# Crea un script que proporcione informacion acerca de los usuarios que se encuentran conec- ´
# tados. Para ello, puedes utilizar la salida del comando w, y grep y sed para obtener los datos
# siguientes:
# 1. Nombre del usuario.
# 2. Hora a la que se ha conectado. Debera estar formateada como HH:mm (p.ej.: 09:35). ´
# 3. Tiempo que lleva ausente. Debera estar formateado como HH:mm:ii (p.ej.: 01:03:15). ´
# A continuacion se muestra un ejemplo de la salida del ´ script que debes desarrollar:


echo "El usuario $(w -h | grep -Eo '^[^ ]+') lleva conectado desde las " # w devuelve info de los usuarios conectados, -h elimina la cabecera
