#pragma once
#include "headers/curl-helper.hpp"
#include "headers/advanced-scene-switcher.hpp"


/*Estructura que alberga la información estática de un problema*/
struct problemInfo {
	string id;
	string label;
	int timeLimit;
	string name;
};

/*Estructura que alberga la información personal de un equipo */
struct idenInfo {
	string affiliation;
	string nationality;
	string name;
};

/*Estructura que alberga la información de un problema respecto a un equipo*/
struct problemTeamInfo {
	string problemId;
	int num_judged;
	bool solved;
	bool first_to_solve;
	int time;
};

/*Estructura que alberga la puntuación de un equipo*/
struct score_t {
	int num_solved;
	int total_time;
	int num_pending; //Puede ser interesante para el sistema heuristico
	vector<string> problem_pending_ids;
};

/*Estructura que alberga toda la información relativa a un equipo */
struct teamInfo {
	int rank;
	score_t score;
	idenInfo identificationInfo;
	vector<problemTeamInfo> problems;
};

/*Estructura que alberga toda la información del torneo*/
struct contestInfo {
	string name;
	int penalty_time;//
	string start_time;
	string end_time;
	map<string,teamInfo> scoreBoard;
	vector<problemInfo> problems;
};

/// <summary>
/// Función auxiliar que se encarga de obtener la respuesta relacionada con la url aportada (Consulta en servidor por medio de cUrl)
/// </summary>
/// <param name="url">  de la consulta que se desea realizar</param>
/// <returns>json en formato string correspondiente a la consulta realizada</returns>
string getRemoteData(std::string &url);

/// <summary>
/// Función encargada de obtener la informacion inicial del contest
/// </summary>
/// <param name="contestName">Nombre del contest</param>
/// <returns>Un contestInfo con toda la información del torneo</returns>
contestInfo getContestRealTimeInfo(string contestName);
