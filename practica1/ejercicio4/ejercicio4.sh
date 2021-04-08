#Crea un script que te permita abrir una terminal con un prompt personalizado modificando
#la variable PS1. La personalizacion del prompt se hara de forma interactiva mediante un men ´ u´
#de opciones. Se recomienda el uso de funciones para organizar el codigo. El men ´ u permitir ´ a ir ´
#anadiendo las diferentes variables que existen para el ˜ prompt, tales como:
#Nombre de usuario.
#Nombre de host.
#Ruta completa.
#Ultima parte de la ruta. ´
#Hora.
#Fecha.
#Ademas, tambi ´ en deber ´ a existir la opci ´ on de a ´ nadir una cadena literal. Cada vez que se a ˜ nada ˜
#un elemento, se mostrara por pantalla el estado actual del ´ prompt que estamos creando. El menu´
#debe tener una opcion para confirmar y abrir ´ bash con el prompt personalizado y otra para salir
#sin hacer nada. Cuando cerremos la sesion personalizada de ´ bash, se terminara el ´ script tambien´
#y el prompt permanecera como estaba antes de ejecutar el ´ script. Para abrir una instancia de bash
#modificando temporalmente la variable PS1, puedes utilizar:

#	PROMPT_COMMAND="PS1='MiPromptPersonalizado';unset PROMPT_COMMAND" bash

#Se trata de una variable de entorno que proporciona bash y que se ejecuta justo antes de mostrar
#el prompt. Si intentamos modificar directamente la variable PS1 sin utilizar la variable PROMPT
#COMMAND, es posible que el prompt no se modifique debido a que al iniciar una sesion de ´ bash, el
#archivo .bashrc se carga y se anula la modificacion que hayamos hecho. ´
#A continuacion se muestra un ejemplo de ejecuci ´ on: 

