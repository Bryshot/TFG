# Guia de instalación del plugin

## Pasos previos a la instalación

- Descargar el código fuente del obs-studio del [repositorio en GitHub](https://github.com/obsproject/obs-studio)
-	Descargar el [código fuente del plugin](https://github.com/Bryshot/TFG)
- Descargar los archivos de dependencias:
  - Dependencias de [VS2017/2019](https://obsproject.com/downloads/dependencies2017.zip)
  - Dependencias de [Qt5(5.10.1)](https://cdn-fastly.obsproject.com/downloads/Qt_5.10.1.7z)
  - Versión de desarrollo de [FFmpeg](https://www.gyan.dev/ffmpeg/builds/ffmpeg-git-essentials.7z)
  -	Versión de desarrollo de [x264](https://code.videolan.org/videolan/x264/)
  - Versión de desarrollo de [mbedTLS](https://github.com/ARMmbed/mbedtls)
  - Versión de desarrollo de [CEF](https://cef-builds.spotifycdn.com/index.html)
  - Versión de desarrollo de [VLC](https://cdn-fastly.obsproject.com/downloads/vlc.zip)
 -	[Descargar el CMake](https://cmake.org/download/) (3.16 o superior, preferiblemente la última versión)
 -	Tener instalado VS2019 (Recomendado) con la [última versión de SDK](https://developer.microsoft.com/en-us/windows/downloads/windows-10-sdk/)
 
 Pasos en caso de querer usar Browser Source:
 -	Construir el proyecto del CEF con el CMake, rellenando los campos del siguiente modo:
   - En el campo de “where is the source code”, introducir la dirección de la carpeta fuente (Por ejemplo D:/Dependencias/CEF). 
   - En el campo de "where to build the binaries", introducir la dirección de un subdirectorio “build”  que debemos crear dentro de la carpeta fuente(Por ejemplo D:/ Dependencias/CEF/Build)
 -	Configurar y generar dicho proyecto(Seleccionando la versión del Visual Studio adecuada y la versión de sistema operativo).
 - Compilar dicho proyecto en modo Release y en modo Debug (Al menos el proyecto de libcef_dll_wrapper).

## Instalación

Existen 2 modos de instalación para el plugin en versión desarrollo; la version in-tree, la cual va integrada dentro de un proyecto del obs en version de desarrollo; y la version out-of-tree, la cual esta aislada del proyecto del obs.

### Instalación in-tree

* Crear dentro de la carpeta de código del obs-studio, una carpeta con nombre build.
* Copiar el código fuente del plugin, insertarlo dentro de la carpeta del obs/UI/frontend-plugins, y añadir el plugin al CMake (Escribir dentro del CMakeList.txt “add_subdirectory(SceneSwitcher)” )
* (Opcional) Crear una carpeta con todos los archivos de dependencias, creando dos subdirectorios en dicha carpeta:
  *	DependenciasGenerales, donde se debe insertar todas las versiones de desarrollo y las dependencias de VS2017/2019(Incluir solo la versión adecuada para el sistema operativo)
  *	Qt5, donde se debe insertar las dependencias de Qt5 
* Abrir el CMake, y rellenar los siguientes campos:
  *	En el campo de “where is the source code”, introducir la dirección del repositorio (Por ejemplo D:/obs-studio). 
  *	En el campo de "where to build the binaries", introducir la dirección del subdirectorio “build” creado previamente (Por ejemplo D:/obs-studio/build)
* Establecer las siguientes variables dentro de CMake (o establecerlas como variables de entorno de Windows, para evitar tener que volver a ponerlo si se desea copilar de nuevo el código fuente):
  *	DepsPath, variable de tipo path, que debe apuntar a la carpeta de dependenciasGeneral. (Por ejemplo, DepsPath= D:\Dependencies\ dependenciasGeneral)
  *	QTDIR, variable de tipo path, que debe apuntar a la carpeta de dependencias de Qt5 (Por ejemplo, QTDIR= D:\Dependencies\Qt\ 5.10.1\msvc2017_64)
  * (En caso de querer usar Browser Source) CEF_ROOT_DIR, variable de tipo path, que debe apuntar a la carpeta de dependencias de CEF (Por ejemplo, CEF_ROOT_DIR = D:\Dependencies\CEF
  * (En caso de querer usar Browser Source) BUILD_BROWSER, variable de tipo booleana, debe estar a true.
  * VLCPath, variable del tipo path, que debe apuntar a la carpeta de dependencias de VLC (Por ejemplo, VLCPath = D:\Dependencies\VLC).
*	En CMake-gui, presionar el botón de configuración, establecer la versión de Visual Studio deseada (Y en caso de que el sistema sea Win32, establecerlo en el segundo campo)
*	En CMake-gui, presionar el botón de generación, lo cual creara el proyecto.

El proyecto se puede abrir desde el propio CMake o desde el explorador de archivos como cualquier otro.

Imagen de la carpeta dependencias generales

Imagen de la configuración en el cMake


### Instalación out-of-tree (no operativo)
* Crear dentro de la carpeta de código del plugin, una carpeta con nombre build.
* (Opcional) Crear una carpeta con todos los archivos de dependencias, creando dos subdirectorios en dicha carpeta:
  *	DependenciasGeneral, donde se debe insertar todas las versiones de desarrollo y las dependencias de VS2017/2019
  *	Qt5, donde se debe insertar las dependencias de Qt5 
* Abrir el CMake, y rellenar los siguientes campos:
  *	En el campo de “where is the source code”, introducir la dirección del repositorio del plugin (Por ejemplo D:/AutoProducer). 
  *	En el campo de "where to build the binaries", introducir la dirección del subdirectorio “build” creado previamente (Por ejemplo D:/AutoProducer/build)
* Establecer las siguientes variables dentro de CMake (o establecerlas como variables de entorno de Windows, para evitar tener que volver a ponerlo si se desea copilar de nuevo el código fuente):
  *	DepsPath, variable de tipo path, que debe apuntar a la carpeta de dependenciasGeneral. (Por ejemplo, DepsPath= D:\Dependencies\ dependenciasGeneral)
  *	QTDIR, variable de tipo path, que debe apuntar a la carpeta de dependencias de Qt5 (Por ejemplo, QTDIR= D:\Dependencies\Qt\ 5.10.1\msvc2017_64)
  *	(En caso de querer usar Browser Source) CEF_ROOT_DIR, variable de tipo path, que debe apuntar a la carpeta de dependencias de CEF (Por ejemplo, CEF_ROOT_DIR = D:\Dependencies\CEF
  *	(En caso de querer usar Browser Source) BUILD_BROWSER, variable de tipo booleana, debe estar a true.
  *	VLCPath, variable del tipo path, que debe apuntar a la carpeta de dependencias de VLC (Por ejemplo, VLCPath = D:\Dependencies\VLC).
  *	BUILD_OUT_OF_TREE, variable de tipo bool, seleccionándola como true
  *	LIBOBS_INCLUDE_DIR, variable de tipo path, que debe apuntar a la localización de la subcarpeta del obs-studio llamada libobs (Por ejemplo D:\obs-studio\libobs)
  *	LIBOBS_FRONTEND_INCLUDE_DIR, variable de tipo path, que debe apuntar a la subcarpeta del obs-studio llamada obs-frontend-api (Por ejemplo D:\obs-studio\UI\obs-frontend-api)
  *	W32 ,variable de tipo path, que debe apuntar a la subcarpeta del obs-studio llamada w32-pthreads (Por ejemplo D:/obs-studio/Build/deps/w32-pthreads)
  *	LIBOBS_LIB, variable de tipo filepath, que debe apuntar a obs.dll (Por defecto se encuentra en C:\Program Files\obs-studio\bin\64bit\obs.dll)
  *	LIBOBS_FRONTEND_API_LIB, variable de tipo filepath, que debe apuntar al obs-frontend-api.dll (Por defecto se encuentra en C:\Program Files\obs-studio\bin\64bit\ obs-frontend-api.dll)
  *	W32_PTHREADS_LIB, variable de tipo filepath, que debe apuntar al w32-pthreads.dll (Por defecto se encuentra en C:/Program Files/obs-studio/bin/64bit/w32-pthreads.dll)
* En CMake-gui, presionar el botón de configuración, establecer la versión de Visual Studio deseada (Y en caso de que el sistema sea Win32, establecerlo en el segundo campo)
* En CMake-gui, presionar el botón de generación, lo cual creara el proyecto.

El proyecto se puede abrir desde el propio CMake o desde el explorador de archivos como cualquier otro.

