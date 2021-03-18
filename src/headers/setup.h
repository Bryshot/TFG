#pragma once
#include <string>
#include <set>
#include <sstream>
#include <filesystem>
#include <obs-frontend-api.h>
#include <util/platform.h>
#include "switcher-data-structs.hpp"

using namespace std;

const int fullscreenWidth = 1920;	//Tamaño de la pantalla completa (Ancho)
const int fullscreenHeight = 1080;	//Tamaño de la pantalla completa (Alto)
const int camHeight = 400;		//Tamaño de la camara (Alto)
const int camWidth = 600;		//Tamaño de la camara (Ancho)
const string fuente = "Arial";		//Fuente de los texto

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
/// Función encargada de crear las settings de una VLC source
/// </summary>
/// <param name="data">, donde se añade</param>
/// <param name="ip"> de la fuente</param>
void make_source_settings(obs_data_t *data, string ip);

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
/// <param name="width">, del texto</param>
/// <param name="height"> del texto</param>
/// <param name="sizeFont"> tamaño de la fuente</param>
/// <param name="opacity"> de la fuente</param>
/// <param name="valign"> a donde esta alinineada la fuente</param>
void make_text_settings(obs_data_t *data, string text, int width, int height,int sizeFont, int opacity,int flag,string valign);

/// <summary>
///  Función encargada de añadir las settings de un scroll filter
/// </summary>
/// <param name="data">, donde se añade</param>
/// <param name="rotationSpeed">, la velocidad de rotación en el eje X</param>
void make_filter_settings(obs_data_t *data, int rotationSpeed);

/// <summary>
/// Función encarga de añadir las settings de una imagen
/// </summary>
/// <param name="data"></param>
/// <param name="file"></param>
void make_image_settings(obs_data_t *data, string file);
