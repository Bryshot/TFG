# AutoProducer
AutoProducer es un autorealizador de camaras para torneos de programación para el OBS Studio, desarrollado por Eduardo Renovales como TFG de la carrera de Ingienería Informática en la Universidad complutense de Madrid. 

![ImagenFDI](https://github.com/Bryshot/TFG/blob/master/docs/Images/fdi.png)

## Requisitos
 Para que el correcto y completo funcionamiento del plugin, el torneo que se deseé retransmitir debe de tener las siguientes caracteristicas:
 - El torneo debe de llevarse a cabo por medio de la api de DomJudge. Además, para el uso completo del plugin es necesario disponer de un usuario con permisos de administrador.
 - Se debe facilitar al plugin un archivo con la información completa de los equipos, tal y como se expone en el [archivo de instalación]()

## Configuración
El plugin permite realizar diversas configuraciones, las cuales se pueden realizar desde la propia interfaz del plugin o importandola, en cuyo caso, se debe respetar el [formato requerido](https://github.com/Bryshot/TFG/blob/master/docs/Formato%20archivo%20configuración.md).

## Instalación
 **El plugin debe de instalarse en la version de usuario salvo en caso de querer modificar el contenido del plugin.**

### Modo usuario
 Teniendo instalado el obs studio, es necesario instalar [el dll](https://github.com/Bryshot/TFG/blob/master/desktop%20version/advanced-scene-switcher.dll) y [el pdb](https://github.com/Bryshot/TFG/blob/master/desktop%20version/advanced-scene-switcher.pdb) del plugin en la carpeta "C:\Program Files\obs-studio\obs-plugins\64bit"

 Imagen de ejemplo de la carpeta obs-plugins:
 ![Imagen de ejemplo](https://github.com/Bryshot/TFG/blob/master/docs/Images/obs-plugins.PNG)
 
### Modo desarrollador
 Para instalar el plugin en modo desarrollador es necesario seguir un procedimiento complejo que se detalla en profundidad en este [documento](https://github.com/Bryshot/TFG/blob/master/docs/Instalación%20Version%20desarrollo.md).

