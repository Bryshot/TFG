#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <obs-frontend-api.h>
#include <set>
#include <sstream>
#include <util/platform.h>
#include <filesystem>
#include "../headers/advanced-scene-switcher.hpp"
//#include "../importers/importers.hpp"
//#include "../obs-app.hpp"

using namespace std;

const int fullscreenWidth = 1920;
const int fullscreenHeight = 1080;
const int camHeight = 400;
const int camWidth = 600;

/// <summary>
/// Función encargada de crear la sección correspondiente a los itemsScene en Json
/// </summary>
/// <param name="numItemsScene">, número de itemsScenes</param>
/// <param name="nameSources">, lista de nombres de los itemsScene</param>
void escribirItemScenes(int numItemsScene, string nameItemsScenes[]);

/// <summary>
///  Función encargada de crear la sección correspondiente a las escenas en el Json
/// </summary>
/// <param name="numSceneItems">, número de itemsScenes</param>
/// <param name="namesSources">, la lista de nombres de los itemsScene</param>
/// <param name="nameScene">, nombre de la escena</param>
void escribirEscena(int numSceneItems, string namesSources[], string nameScene);

/// <summary>
/// Función encargada de crear las fuentes (de tipo URL) en el Json
/// </summary>
/// <param name="name">, nombre de la fuente</param>
/// <param name="url">, direccion url a la que apunta</param>
/// <param name="height">, alto de la fuente</param>
/// <param name="width">, ancho de la fuente</param>
/// <param name="ultimo">, si es la ultima fuente del Json</param>
void escribirFuenteURL(string name, string url, int height, int width,
                       bool ultimo);

/// <summary>
/// Función encargada de escribir la seccion de sceneOrden en el Json
/// </summary>
void escribeEscenasOrden();

/// <summary>
/// Funcion general encargada de escribir la sección de sources del Json
/// </summary>
void escribeSources();

/// <summary>
/// Función que adelanta una linea del fichero de entrada ignorando la entrada
/// </summary>
void saltarLinea();

/// <summary>
///  Función encargada de avanzar en el fichero de entrada hasta encontrar el token, que si escribe lo indica, se escribe en el fichero de salida
/// </summary>
/// <param name="token">, cadena hasta la que avanzar</param>
/// <param name="escribe">, si se debe escribir o no lo que se avance en el fichero de entrada</param>
/// <returns></returns>
string continuarHastaToken(string token, bool escribe);


/// <summary>
/// Función general que se encarga de escribir un Json adecuado para la realización del torneo
/// </summary>
/// <param name="ContestName">, nombre de la colección de escenas dentro del OBS</param>
/// <param name="path">, dirección del Json que representa a la colección previa de escenas</param>
/// <param name="nameJson">, nombre del archivo Json que se va a crear</param>
void escribeFichero(string ContestName, string path, string nameJson);


/// <summary>
/// Función encargada de obtener los parametros necesarios para crear el JSon de configuración.
/// </summary>
void crearConfiguracion(string contestName);


/// <summary>
/// Función encargada de comprobar si existe una Scene 
/// </summary>
/// <param name="collectionName">Nombre de la colección</param>
/// <returns>True si existe la coleccion, False en otro caso</returns>
bool existeSceneCollection(string collectionName);



string constestNameUnique(string contestName);
string adaptaNombreFichero(string name);
string nameJsonUnique(string name, string collection_path);
