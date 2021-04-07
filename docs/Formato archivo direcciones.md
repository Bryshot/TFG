#Formato del archivo para importar la información de los equipos.

El formato de fichero necesario para la correcta importación de toda la información relativa a las fuentes de los equipos y la de la clasificación, es la siguiente :

* Cada línea contendrá únicamente un cadena de información.
* La primera línea contendrá la cantidad de equipos presentes en el torneo, continuado por la dirección web de la clasificación.
* Tras esto, para cada equipo se debe seguir el siguiente patrón:
  * En primer lugar, el nombre del equipo tal y como se refleje en el torneo (necesario para la vinculación).
  * A continuación, la dirección IP de la retransmisión del escritorio del equipo, seguido por la dirección IP de la retransmisión de la cámara del equipo (con el formato adecuado para las fuentes del VLC (Ejemplo: http://192.168.0.1::8000)).
  * En último lugar, la ruta donde se halla el icono del equipo (Los iconos deben tener una resolución de 110x110 px, de ser otra, seria necesario redimensionar la fuente en OBS, pero es necesario que todas tengan al menos la misma resolución)
