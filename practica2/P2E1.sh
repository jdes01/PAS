#!/bin/bash

## Desarrollar un script en el que utilices los comandos grep y sed para formatear 
## el fichero films.html que se proporciona. El fichero se encuentra en formato html, similar al que
## podr´ıamos encontrar en una pagina como IMDB. El ´ script debera realizar las siguientes tareas:

## 1. Mostrar los datos de las pel´ıculas en formato de texto plano similar al que se muestra en el
## ejemplo siguiente.

## 2. Mostrar el t´ıtulo, el ano y la valoraci ˜ on de las pel ´ ´ıculas de los anos 2000 (2000-2009).

## 3. Mostrar las pel´ıculas en el mismo formato que se indico en el punto 1 pero cambiando el 
## 		formato del numero de valoraciones para que muestre los miles como “k”. Por ejemplo, en 
## 		lugar de 658000, se debera mostrar 658k. 

## 4. Anadir un punto final a las l ˜ ´ıneas que no lo tengan.

## 5. Mostrar una version resumida en la que la sinopsis se limitar ´ a a 300 caracteres, mostrando 
## 		puntos suspensivos al final de la l´ınea.


echo -e "\n\t1) Mostrar pelis en el formato indicado"

##  sed -e --> Con esta opción indicamos que estamos pasando comandos por la línea de comandos y no a través de un archivo. 
sed -e 's/<p id=.director.>/Director: /g' -e 's/<p id=.genres.>/Generos: /g' -e 's/<. id="writers">/Guionistas: /g' -e 's/<.2>/Titulo: /g' -e 's/<p id="rating">/Valoracion: /g' -e 's/<p id=.length.>/Duracion: /g' -e 's/m<\/p>/ minutos/g' -e 's/<\/p>//g' -e 's/<p>/Sinopsis: /g' -e 's/<p>/Sinopsis: /g' -e '/<\/article>/d' -e '/<hr>/d' -e 's/<article>/\t==================================/g' -e 's/(20/\n\tAño: 20/g' -e 's/)<\/h2>/ /g' -e '/<\/body>/d' -e '/<\/html>/d' -e '/<!DOCTYPE html>/d' -e '/html/d' -e '/<head>/d' -e '/<title>Films<\/title>/d' -e '/<\/head>/d' -e '/<body>/d' -e '/<h1>Films<\/h1>/d' -e 's/[|]/, /g' -e 's/ratings/valoraciones/g' films.html

## sed -e 's/<pid=.director.>/Director:/g'
##.  opcion |    patron 1    | patron 2| global (todas las aparicones de patron 1, cambiarlo por patron 2)


echo -e "\n\t2) Mostrar el título, el año y la valoración de las películas de los años 2000 (2000-2009)\n"
sed -e 's/<p id=.director.>/Director: /g' -e 's/<p id=.genres.>/Generos: /g' -e 's/<. id="writers">/Guionistas: /g' -e 's/<.2>/Titulo: /g' -e 's/<p id="rating">/Valoracion: /g' -e 's/<p id=.length.>/Duracion: /g' -e 's/m<\/p>/ minutos/g' -e 's/<\/p>//g' -e 's/<p>/Sinopsis: /g' -e 's/<p>/Sinopsis: /g' -e '/<\/article>/d' -e '/<hr>/d' -e 's/<article>/\t==================================/g' -e 's/(20/\n\tAño: 20/g' -e 's/)<\/h2>/ /g' -e '/<\/body>/d' -e '/<\/html>/d' -e '/<!DOCTYPE html>/d' -e '/html/d' -e '/<head>/d' -e '/<title>Films<\/title>/d' -e '/<\/head>/d' -e '/<body>/d' -e '/<h1>Films<\/h1>/d' -e 's/[|]/, /g' -e 's/ratings/valoraciones/g' films.html | grep -B 1 -A 1 '200[0-9]'

echo -e "\n\n\t3)  Mostrar las pel ́ıculas formatadas como en 1) pero cambiando el formato del n ́umero de valoracionesde 658000 a 658k\n"
sed -e 's/<p id=.director.>/Director: /g' -e 's/<p id=.genres.>/Generos: /g' -e 's/<. id="writers">/Guionistas: /g' -e 's/<.2>/Titulo: /g' -e 's/<p id="rating">/Valoracion: /g' -e 's/<p id=.length.>/Duracion: /g' -e 's/m<\/p>/ minutos/g' -e 's/<\/p>//g' -e 's/<p>/Sinopsis: /g' -e 's/<p>/Sinopsis: /g' -e '/<\/article>/d' -e '/<hr>/d' -e 's/<article>/\t==================================/g' -e 's/(20/\n\tAño: 20/g' -e 's/)<\/h2>/ /g' -e '/<\/body>/d' -e '/<\/html>/d' -e '/<!DOCTYPE html>/d' -e '/html/d' -e '/<head>/d' -e '/<title>Films<\/title>/d' -e '/<\/head>/d' -e '/<body>/d' -e '/<h1>Films<\/h1>/d' -e 's/[|]/, /g' -e 's/ratings/valoraciones/g' -e 's/\([[:digit:]]\{3\}\)\([[:digit:]]\{3\}\)/\1\k/g' films.html

echo -e "\n\n\t4) Añadir un punto al final de las líneas que no lo tengan.\n"
sed -e 's/<p id=.director.>/Director: /g' -e 's/<p id=.genres.>/Generos: /g' -e 's/<. id="writers">/Guionistas: /g' -e 's/<.2>/Titulo: /g' -e 's/<p id="rating">/Valoracion: /g' -e 's/<p id=.length.>/Duracion: /g' -e 's/m<\/p>/ minutos/g' -e 's/<\/p>//g' -e 's/<p>/Sinopsis: /g' -e 's/<p>/Sinopsis: /g' -e '/<\/article>/d' -e '/<hr>/d' -e 's/<article>/\t==================================/g' -e 's/(20/\n\tAño: 20/g' -e 's/)<\/h2>/ /g' -e '/<\/body>/d' -e '/<\/html>/d' -e '/<!DOCTYPE html>/d' -e '/html/d' -e '/<head>/d' -e '/<title>Films<\/title>/d' -e '/<\/head>/d' -e '/<body>/d' -e '/<h1>Films<\/h1>/d' -e 's/[|]/, /g' -e 's/ratings/valoraciones/g' -e 's/$/\./g' films.html

echo -e "\n\n\t5) Mostrar una versión resumida en la que la sinopsis se limita a 300 caracteres\n"
sed -e 's/<p id=.director.>/Director: /g' -e 's/<p id=.genres.>/Generos: /g' -e 's/<. id="writers">/Guionistas: /g' -e 's/<.2>/Titulo: /g' -e 's/<p id="rating">/Valoracion: /g' -e 's/<p id=.length.>/Duracion: /g' -e 's/m<\/p>/ minutos/g' -e 's/<\/p>//g' -e 's/<p>/Sinopsis: /g' -e 's/<p>/Sinopsis: /g' -e '/<\/article>/d' -e '/<hr>/d' -e 's/<article>/\t==================================/g' -e 's/(20/\n\tAño: 20/g' -e 's/)<\/h2>/ /g' -e '/<\/body>/d' -e '/<\/html>/d' -e '/<!DOCTYPE html>/d' -e '/html/d' -e '/<head>/d' -e '/<title>Films<\/title>/d' -e '/<\/head>/d' -e '/<body>/d' -e '/<h1>Films<\/h1>/d' -e 's/[|]/, /g' -e 's/ratings/valoraciones/g' films.html | sed -e 's/^\(.\{318\}\).*$/\1.../'
