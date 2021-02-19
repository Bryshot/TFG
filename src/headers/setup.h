#pragma once
#include <string>
#include <set>
#include <sstream>
#include <filesystem>
#include <obs-frontend-api.h>
#include <util/platform.h>
#include <src/headers/switcher-data-structs.hpp>

using namespace std;

const int fullscreenWidth = 1920;
const int fullscreenHeight = 1080;
const int camHeight = 400;
const int camWidth = 600;
const string fuente = "Arial";

/// <summary>
/// Función encargada de obtener los parametros necesarios para crear el JSon de configuración.
/// </summary>
/// <param name="switcher">Core del plugin</param>
void crearConfiguracion(SwitcherData *switcher);

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
/// Función encargada de crear las settings de una browser source
/// </summary>
/// <param name="data">, donde se añade</param>
/// <param name="url"> de la fuente</param>
/// <param name="height"> de la fuente</param>
/// <param name="width"> de la fuente</param>
void make_source_settings(obs_data_t *data, string url, int height, int width);

/// <summary>
/// Función encargada de añadir las settings de una text source
/// </summary>
/// <param name="data">, donde se añade</param>
/// <param name="text"> de la fuente</param>
/// <param name="size">  altura de la fuente</param>
/// <param name="valign"> a donde esta alinineada la fuente</param>
void make_text_settings(obs_data_t *data, string text, int size, string valign);

/// <summary>
///  Función encargada de añadir las settings de un scroll filter
/// </summary>
/// <param name="data">, donde se añade</param>
/// <param name="rotationSpeed">, la velocidad de rotación en el eje X</param>
void make_filter_settings(obs_data_t *data, int rotationSpeed);
