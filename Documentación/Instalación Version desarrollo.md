# Gui de instalación del plugin

## Pasos previos a la instalación

 1. Descargar el código fuente del obs-studio del [repositorio en GitHub](https://github.com/obsproject/obs-studio)
 2.	Descargar el código fuente del plugin
 3. Descargar los archivos de dependencias
  1.	Dependencias de VS2017/2019
  2.	Versión de desarrollo de FFmpeg
  3.	Versión de desarrollo de x264 
 4.	Versión de desarrollo de mbedTLS
 5.	Versión de desarrollo de CEF
 6.	Versión de desarrollo de VLC 
 7.	Descargar el CMake (3.16 o superior, preferiblemente la última versión)
 8.	Tener instalado VS2019 (Recomendado) con la última versión de SDK
 
 Pasos en caso de querer usar Browser Source:
 1.	Construir el proyecto del CEF con el CMake, rellenando los campos del siguiente modo:
  a.  En el campo de “where is the source code”, introducir la dirección de la carpeta fuente (Por ejemplo D:/Dependencias/CEF). 
  b.  En el campo de "where to build the binaries", introducir la dirección de un subdirectorio “build”  que debemos crear dentro de la carpeta fuente(Por ejemplo D:/ Dependencias/CEF/Build)
 2.	Configurar y generar dicho proyecto(Seleccionando la versión del Visual Studio adecuada y la versión de sistema operativo).
 3. Compilar dicho proyecto en modo Release y en modo Debug (Al menos el proyecto de libcef_dll_wrapper).
