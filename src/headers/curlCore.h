#pragma once
#include "headers/curl-helper.hpp"
#include "headers/advanced-scene-switcher.hpp"

enum class typesInfo {Submissions,Problems};


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
};

/*Estructura que alberga toda la información relativa a un equipo */
struct teamInfo {
	int rank;
	score_t score;
	idenInfo identificationInfo;
	map<string,problemTeamInfo> problems;
};

/*Estructuea que alberga la información de una entrega*/
struct submissionInfo {
	string idProblem;
	string idTeam;
	string result = "pendingJudgment";
};

/*Estructura que alberga toda la información del torneo*/
struct contestInfo {
	string name;
	int penalty_time;//
	string start_time;
	string end_time;
	map<string,teamInfo> scoreBoard;
	vector<problemInfo> problems;
	map<string, submissionInfo> submissionPendings;
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
contestInfo getContestRealTimeInfo();

/// <summary>
/// Configura las variables del curl del torneo
/// </summary>
void setCurls();

/// <summary>
/// Obtiene la información general del torneo
/// </summary>
/// <param name="contest">, donde se almacena la información</param>
void getGeneralContestInfo(contestInfo& contest);

///<summary>
/// Obtiene la información general del torneo
/// </summary>
/// <param name="contest">, donde se almacena la información</param>
/// <param name="data">, la información</param>
void getProblemContestInfo(contestInfo &contest, obs_data_t *data);

/// <summary>
/// Obtiene la información de los equipos
/// </summary>
/// <param name="contest">, donde se almacena la información</param>
/// <param name="startingInfo">, si se debe obtener la información estática de los equipos</param>
void getTeamsContestInfo(contestInfo& contest, bool startingInfo);

/// <summary>
/// Obtiene la información de las entregas realizadas
/// </summary>
/// <param name="contest">, donde se almacena la información</param>
/// <param name="data">, la información</param>
void getSubmissionsInfo(contestInfo &contest, obs_data_t *data);

/// <summary>
/// Obtiene la información de los veredictos de las entregas
/// </summary>
/// <param name="contest">, donde se almacena la información</param>
void getJudgementsInfo(contestInfo &contest);

/// <summary>
/// Función auxiliar para eliminar codigo rebundante en la obtención de información en vectores
/// </summary>
/// <param name="contest">, donde se almacena la información</param>
/// <param name="curl">, de donde obtener la información</param>
/// <param name="type">, el tipo de información que deseamos obtener</param>
void getArrayInfo(contestInfo &contest, string curl, typesInfo type);

/// <summary>
/// Obtiene la información estática de los equipos
/// </summary>
/// <param name="id">, del equipo</param>
/// <param name="info">, donde se almacena la información</param>
void getIdentificationTeamInfo(string id, teamInfo&info);
