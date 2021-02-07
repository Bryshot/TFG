#pragma once
#include <string>
#include <set>
#include <sstream>
#include <filesystem>
#include <obs-frontend-api.h>
#include <util/platform.h>

using namespace std;

const int fullscreenWidth = 1920;
const int fullscreenHeight = 1080;
const int camHeight = 400;
const int camWidth = 600;

/// <summary>
/// Función encargada de obtener los parametros necesarios para crear el JSon de configuración.
/// </summary>
void crearConfiguracion(string contestName);

/// <summary>
/// Función de utilidad encargada de crear un nombre de sceneCollection único.
/// </summary>
/// <param name="name">Nombre original</param>
/// <returns>Nombre original, o en caso de no ser único, una versión modificada de este</returns>
string makeUniqueName(string name);

/// <summary>
/// Función de utilidad encargada de crear un nombre de fichero para la sceneCollection único.
/// </summary>
/// <param name="name">Nombre original</param>
/// <param name="path">Ruta donde se guardan por defecto las sceneCollection</param>
/// <returns>Nombre original, o en caso de no ser único, una versión modificada de este</returns>
string makeUniqueFileName(string name,string path);


/// <summary>
/// Función de utilidad que convierte el nombre de una sceneCollection a su equivalente en fichero.
/// </summary>
/// <param name="name">Nombre de la sceneCollection</param>
/// <returns>Nombre del fichero</returns>
string makeFileName(string name);


/// <summary>
/// Función encargada de añadir las settings a una fuente
/// </summary>
/// <param name="data">, donde se añade</param>
/// <param name="url"> de la fuente</param>
/// <param name="height"> de la fuente</param>
/// <param name="width"> de la fuente</param>
void make_source_settings(obs_data_t *data, string url, int height, int width);
