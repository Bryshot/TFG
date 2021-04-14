#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <cctype>
#include <map>
using namespace std;

typedef struct {
	string ipCam;    //Dirección ip y puerto donde se encuentra la camara
	string ipScreen; //Dirección ip y puerto donde se encuentra la pantalla
	string urlLogo;  //Dirección url donde esta el logo del equipo
} IpsTeam;

typedef struct {
	map<string, IpsTeam> ipsTeams; //Mapa con todos los equipos del torneo
	int numTeams;                  //Numero de equipos en el torneo
	string ipClassification;       //Dirección  web de la clasificación
} IpsContest;

/// <summary>
/// Función encargada de asignar una linea del fichero a una variable
/// </summary>
/// <param name="s"></param>
void asignaString(string &s);

/// <summary>
/// Función encargada de obtener la información del torneo y los equipos del fichero seleccionado
/// </summary>
/// <param name="path">, del fichero que se obtiene la información</param>
/// <returns></returns>
IpsContest importIpContest(string path);
