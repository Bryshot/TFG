#pragma once
#include <string>
#include <map>
#include <vector>
#include "headers/switcher-data-structs.hpp"
#include "headers/curl-helper.hpp"
#include "headers/autoProducerContest.hpp"
#include <obs-frontend-api.h>

using namespace std;

const string pendingJugment = "PJ";

enum class typesInfo {
	Submissions
}; //Simplificar en un futuro si no se añaden nuevos types

/*Estructura que alberga la información personal de un equipo */
struct idenInfo {
	string affiliation;
	string nationality;
	string name;
};

/*Estructura que alberga la puntuación de un equipo*/
struct score_t {
	int num_solved;
	//int total_time; //Puede ser interesante para el sistema heuristico (En desuso)
	int num_pending = 0;
};

/*Estructura que alberga toda la información relativa a un equipo */
struct teamInfo {
	int rank;
	score_t score;
	int timeInStream = 0;
	idenInfo identificationInfo;
};

/*Estructuea que alberga la información de una entrega*/
struct submissionInfo {
	string idProblem;
	string idTeam;
	string result = pendingJugment;
};

/*Estructura que alberga toda la información del torneo*/
struct contestInfo {
	string start_time;
	string end_time;
	map<string, teamInfo> scoreBoard;
	map<string, submissionInfo> submissionPendings;
};

/// <summary>
/// Función auxiliar que se encarga de obtener la respuesta relacionada con la url aportada (Consulta en servidor por medio de cUrl)
/// </summary>
/// <param name="url">  de la consulta que se desea realizar</param>
/// <param name="admin"> si el mensaje debe enviarse como admin</param>
/// <returns>json en formato string correspondiente a la consulta realizada</returns>
string getRemoteData(std::string &url, bool admin);

/// <summary>
/// Función encargada de obtener la informacion inicial del contest
/// </summary>
/// <param name="contestName">Nombre del contest</param>
/// <returns>Un contestInfo con toda la información del torneo</returns>
contestInfo getContestRealTimeInfo();

/// <summary>
/// Función encargada de actualizar la información del torneo en tiempo real
/// </summary>
/// <param name="data"> donde se almacena toda la información</param>
void updateContestRealTimeInfo(contestInfo &data);

/// <summary>
/// Configura las variables del curl del torneo
/// </summary>
void setCurls();

/// <summary>
/// Obtiene la información general del torneo
/// </summary>
/// <param name="contest">, donde se almacena la información</param>
void getGeneralContestInfo(contestInfo &contest);

/// <summary>
/// Obtiene la información de los equipos
/// </summary>
/// <param name="contest">, donde se almacena la información</param>
/// <param name="startingInfo">, si se debe obtener la información estática de los equipos</param>
void getTeamsContestInfo(contestInfo &contest, bool startingInfo);

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
void getIdentificationTeamInfo(string id, teamInfo &info);

/// <summary>
/// Realiza los calculos heuristicos
/// </summary>
/// <param name="bestHeuristic">, el valor heuristico del mejor equipo</param>
/// <param name="best">, la información del mejor equipo</param>
/// <param name="bestId">, id del mejor equipo</param>
/// <param name="contest">, donde se almacena la información dinámica</param>
void heuristic(double bestHeuristic, teamInfo &best, string bestId,
	       contestInfo &contest);

/// <summary>
/// Comprueba si el torneo se encuentra congelado
/// </summary>
/// <returns>true si el torneo esta congelado, o false en otro caso</returns>
bool isFrozen();
