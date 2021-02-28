#pragma once
#include <condition_variable>
#include <string>
#include <vector>
#include <mutex>
#include <QDateTime>
#include <QThread>
#include <curl/curl.h>
#include "switch-transitions.hpp"
#include "importIPs.h"

constexpr auto default_interval = 300;
constexpr auto previous_scene_name = "Previous Scene";
constexpr auto default_sizeRotativeText = 48;
constexpr auto default_speedRotativeText = 100;
constexpr auto default_contestName = "Contest";
constexpr auto default_rotatingText = "Default rotating text";
constexpr auto default_url = "www.youtube.com/embed/4sJ1YMkXm28?autoplay=1";
const string default_contestServer = "https://www.domjudge.org/demoweb";

class SwitcherThread;
class SwitcherThreadSubmissions;

/********************************************************************************
 * SwitcherData
 ********************************************************************************/
struct SwitcherData {
	SwitcherThread *th = nullptr;
	SwitcherThreadSubmissions *thSub = nullptr;
	std::condition_variable cv;
	std::mutex m;
	CURL *curl = nullptr;

	bool transitionActive = false;
	bool waitForTransition = false;
	std::condition_variable transitionCv;
	bool stop = false;		//Si se ha detenido el plugin
	bool verbose = false;		//Si se debe escribir las acciones por la salida estandar
	bool tansitionOverrideOverride = false;
	bool importedIPs = false;	//Si se han cargado las IPs del torneo
	bool usingDummy = false;	//Si se esta usando las camTeam y screenTeam o la version dummy
	bool created = false;		//Si se ha creado la configuración 

	string contestName = default_contestName;	//Nombre del concurso
	IpsContest ipsContestData;			//Estructura encargada de almacenar las direcciones IP del torneo
	int interval = default_interval;		//Intervalo de tiempo entre switchs

	/*Elementos necesarios para el texto rotativo */
	string textRotativeContent = default_rotatingText;	//Contenido del texto rotativo
	int sizeRotativeText = default_sizeRotativeText;	//Tamaño  del texto rotativo
	int speedRotativeText = default_speedRotativeText;	//Velocidad del texto rotativo

	/*Elementos necesarios para obtener la info del concurso*/

	string contestServerWebsite = default_contestServer; //Url base del servidor de DomJudge
	string curlContest;				     //Url del concurso dentro del DomJugde
	string curlProblems;				     //Url de los problemas dentro del DomJugde
	string curlScoreboard;				     //Url de la clasificación dentro del DomJugde
	string curlSubmissions;				     //Url de las entregas dentro del DomJugde
	string curlJudgements;				     //Url de las valoraciones dentro del DomJugde
	string curlTeams;				     //Url de los equipos dentro del DomJugde
	

	obs_source_t *waitScene = NULL;			     //Scene que almacena la escena presente antes de un cv.wait
	OBSWeakSource previousScene = NULL;		///REVISAR
	OBSWeakSource PreviousScene2 = NULL;		///REVISAR

	/*Url de las fuentes actuales*/
	string ipCam = "";				//Variable que almacena el valor inicial de camTeam
	string ipScreen = "";				//Variable que almacena el valor inicial de screenTeam
	string urlClassification = default_url;		//Variable que almacena el valor inicial de screenClassification

	/*Fuentes necesarias para la realización del torneo*/
	OBSSource screenTeam;				//Source que almacena la screenTeam
	OBSSource camTeam;				//Source que almacena la camTeam
	OBSSource screenTeamDummy;			//Source que almacena la screenTeamDummy
	OBSSource camTeamDummy;				//Source que almacena la camTeamDummy
	OBSSource screenClassification;			//Source que almacena la screenClassification
	OBSSource textRotative;				//Source que almacena el textRotative
	OBSSource filter;				//Source que almacena el filtro de rotación del textRotative

	/*Scene items necesarios para la realización del torneo */
	obs_sceneitem_t *camTeamDummyItem;		//Scene_Item que almacena la camTeamDummy	
	obs_sceneitem_t *screenTeamDummyItem;		//Scene_Item que almacena la screenTeamDummy	
	obs_sceneitem_t *camTeamItem;			//Scene_Item que almacena la camTeam	
	obs_sceneitem_t *screenTeamItem;		//Scene_Item que almacena la screenTeam	

	/*Variables para la cola de entregas*/
	int numPendingSubmissions =0; //Solo sirve para saber de cuantas submissions no tienen resultado todavía
	int numSubmissionsPrevious = 0; //Sirve para no tener que comprobar las entregas ya comprobadas, pues el cUrl devuelve todas las del torneo.

	std::vector<SceneTransition> sceneTransitions;
	std::vector<DefaultSceneTransition> defaultSceneTransitions;

	//Estructura para representar la prioridad de un QThread
	struct ThreadPrio {
		std::string name;
		std::string description;
		uint32_t value;
	};

	//Vector con los distintos tipos de prioridades disponibles para un QThread
	std::vector<ThreadPrio> threadPriorities{
		{"Idle",
		 "scheduled only when no other threads are running (lowest CPU load)",
		 QThread::IdlePriority},
		{"Lowest", "scheduled less often than LowPriority",
		 QThread::LowestPriority},
		{"Low", "scheduled less often than NormalPriority",
		 QThread::LowPriority},
		{"Normal", "the default priority of the operating system",
		 QThread::NormalPriority},
		{"High", "scheduled more often than NormalPriority",
		 QThread::HighPriority},
		{"Highest", "scheduled more often than HighPriority",
		 QThread::HighestPriority},
		{"Time critical",
		 "scheduled as often as possible (highest CPU load)",
		 QThread::TimeCriticalPriority},
	};

	//Prioridad utilizada en los QThreads del plugin
	uint32_t threadPriority = QThread::NormalPriority;

	//Orden del las tabs de la configuración del plugin
	std::vector<int> tabOrder;

	/*Elementos necesarios para los hotkeys del plugin*/
	bool hotkeysRegistered = false;			//Si se han registrado los hotkeys del plugin dentro de los del obs
	obs_hotkey_id startHotkey;			//Hotkey de inicio del plugin
	obs_hotkey_id stopHotkey;			//Hotkey de final del plugin
	obs_hotkey_id toggleHotkey;			//Hotkey para alternar el estado del plugin

	/*Funciones del plugin*/
	void Thread();			//Función principal del plugin, controlada por el thread principal.
	void ThreadSubmissions();	//Función principal del thSub, Thread encargado de actualizar la lista de envios y correcciones
	void Start();			//Función que inicia el funcionamiento del plugin, arranca los diversos QThreads
	void Stop();			//Funcíón que detiene el funcionamiento del plugin, para y elimina los diversos QThreads

	void switchIP(string ipScreen, string ipCam,unique_lock<mutex> &lock); //Función encargada de cambiar los scene item de una escena

	void modificaVLC(obs_source_t *source, string ip); //Función encargada de modificar el contenido de una VLCSource

	bool sceneChangedDuringWait();	//Función encargada de comprobar si se ha cambiado de escena durante una espera
	
	void setDefaultSceneTransitions(); 
	
	void saveSceneTransitions(obs_data_t *obj);
	void saveGeneralSettings(obs_data_t *obj);
	void saveHotkeys(obs_data_t *obj);
	
	void loadSceneTransitions(obs_data_t *obj);
	void loadGeneralSettings(obs_data_t *obj);
	void loadHotkeys(obs_data_t *obj);

	void Prune();
	inline ~SwitcherData() { Stop(); }
};

extern SwitcherData *switcher;

//Clase que representa el QThread que se encarga de la funcionalidad principal del plugin
class SwitcherThread : public QThread {
public:
	explicit SwitcherThread(){};
	void run() { switcher->Thread(); };
};

//Clase que representa el QThread que se encarga de actualizar la lista de envios y correcciones
class SwitcherThreadSubmissions : public QThread {
public:
	explicit SwitcherThreadSubmissions(){};
	void run() { switcher->ThreadSubmissions(); };
};
