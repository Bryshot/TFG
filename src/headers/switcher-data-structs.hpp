#pragma once
#include <condition_variable>
#include <string>
#include <queue>
#include <mutex>
#include <QDateTime>
#include <QThread>
#include <curl/curl.h>
#include <obs.h>
#include <obs-frontend-api.h>
#include "importIPs.h"

/*Constantes*/
const int default_interval = 3000;
const int default_delayIp = 5000;
const int default_delayJugdement = 1000;
const int default_sizeRotativeText = 48;
const int default_speedRotativeText = 100;
const int default_textStaticWidth = 350;
const int default_textStaticHeight = 28;
const int default_textSubmissionWidth = 350;
const int default_textSubmissionHeight = 600;
const int default_textTeamWidth = 880;
const int default_textTeamHeight = 110;
const int default_weightOfRank = 1;
const int default_weightOfTime = 1;
const int default_weightOfPending = 1;
const int default_numberOfCycle = 20;

const string default_contestName = "Contest";
const string defautl_staticText = "TEAM | PROBLEM | STATUS ";
const string default_url = "www.youtube.com/embed/2Fd8DeIvuU8?autoplay=1";
const string default_contestServer = "https://www.domjudge.org/demoweb";

const int MAX_VISIBLE_TEAMS_SUBMISSION = 22;
const int MAX_VISIBLE_CHARACTERS_SUBMISSION_TEXT = 20;

class SwitcherThread;
class SwitcherThreadSubmissions;

/********************************************************************************
 * SwitcherData
 ********************************************************************************/
struct SwitcherData {

	SwitcherThread *th = nullptr; //QThread principal del plugin
	SwitcherThreadSubmissions *thSub =
		nullptr; //QThread encargado de la actualización de la lista de entregas y correcciones
	std::condition_variable cv;
	std::mutex m;
	CURL *curl =
		nullptr; //Variable CURL que mantiene la configuración de las consultas en el servidor

	const string tokenIdClassification =
		"-1"; //Constante del id de equipo que representa la clasificación

	string contestName = default_contestName; //Nombre del concurso
	IpsContest
		ipsContestData; //Estructura encargada de almacenar las direcciones IP del torneo

	/*Delays y Timers para el control de los QThreads*/
	int interval = default_interval; //Intervalo de tiempo entre switchs
	int delayIp =
		default_delayIp; //Retraso previo ha hacer efectivo un cambio de equipo (Debido al delay interno del cambio de VLC)
	int delayJugdment =
		default_delayJugdement; //Retraso entre actualizaciones de la cola de envios

	/*Elementos necesarios para el control del plugin(Flags internos)*/
	bool stop = true; //Si se ha detenido el plugin
	bool verbose =
		false; //Si se debe escribir las acciones por la salida estandar
	bool importedIPs = false; //Si se han cargado las IPs del torneo
	bool usingDummy =
		false; //Si se esta usando las camTeam y screenTeam o la version dummy
	bool created = false; //Si se ha creado la configuración
	bool updatedSubmissions =
		false; //Si hay cambios en la información de las entregas
	bool swapScene = false; //Si hay que cambiar de escena
	bool swapIp = false;    //Si hay que cambiar las fuentes de TeamViewer
	bool loading = true;    //Si se esta realizando una carga
	bool preCreated =
		false; //Si se permite usar scene Collections no creados directamente por el plugin.(O por alguna ejecución previa del mismo)
	bool followVisible =
		true; //Si los sceneItems dummies tienen que tener la misma forma(tamaño,escala y rotación) que las principales

	/*Elementos necesarios para el texto rotativo */
	string textRotativeContent; //Contenido del texto rotativo
	int sizeRotativeText =
		default_sizeRotativeText; //Tamaño  del texto rotativo
	int speedRotativeText =
		default_speedRotativeText; //Velocidad del texto rotativo

	/*Elementos necesarios para el texto estático*/
	string textStaticContent =
		defautl_staticText; //Contenido del texto rotativo
	int textStaticWidth =
		default_textStaticWidth; //Ancho del texto estático
	int textStaticHeight =
		default_textStaticHeight; //Alto del texto estático

	/*Elementos necesarios para el texto estático*/
	string textSubmissionContent = ""; //Contenido del texto rotativo
	int textSubmissionWidth =
		default_textSubmissionWidth; //Ancho del texto estático
	int textSubmissionHeight =
		default_textSubmissionHeight; //Ancho del texto estático

	/*Elementos necesarios para el texto estático*/
	string textTeamContent =
		"Team:\nClassification:\nNumber of Problem Resolved:"; //Contenido del texto rotativo
	int textTeamWidth =
		default_textTeamWidth; //Ancho del texto del equipo en stream
	int textTeamHeight =
		default_textTeamHeight; //Ancho del texto del equipo en stream
	string textTeamImageFile =
		""; //Variable que almacena la dirección del icono el eqipo en el Stream

	/*Elementos necesarios para obtener la info del concurso*/
	string contestServerWebsite =
		default_contestServer; //Url base del servidor de DomJudge
	string passwordContestServer = "admin"; //Contraseña del usuario admin
	string userContestServer =
		"admin"; //Usuario del concurso con permisos de administrador (Necesario logear para acceder a ciertas consultas)
	string curlContest;     //Url del concurso dentro del DomJugde
	string curlScoreboard;  //Url de la clasificación dentro del DomJugde
	string curlSubmissions; //Url de las entregas dentro del DomJugde
	string curlJudgements;  //Url de las valoraciones dentro del DomJugde
	string curlState;       //Url del estado del torneo dentro del DomJugde
	string curlTeams;       //Url de los equipos dentro del DomJugde

	/*Elementos necesarios para el calculo de la heurística*/
	queue<string>
		lastTeamsInStream; //Vector de identificadores de los ultimos cycleSize equipos que han estado en el Stream (Cambiar por cola, solo se inserta por el final y se elimina el primero)
	int classificationTimeInStream =
		0; //Variable que controla la cantidad de veces que ClassificationView ha estado en Stream en los ultimos cycleSize ciclos.
	double rankWeight =
		default_weightOfRank; //Peso del ranking en el computo de la heurística
	double numPendingWeight =
		default_weightOfPending; //Peso de los envios pendientes en el computo de la heurística
	double timeInStreamWeight =
		default_weightOfTime; //Peso del tiempo en pantalla en el computo de la heurística
	int cycleSize =
		default_numberOfCycle; //Tamaño maximo de la cola lastTeamInStream (durante cuantos cambios se tienen en cuenta una aparición en el stream)

	obs_source_t *waitScene =
		NULL; //Scene que almacena la escena presente antes de un cv.wait

	//Url de las fuentes actuales
	string ipCam = ""; //Variable que almacena el valor futuro de camTeam
	string ipScreen =
		""; //Variable que almacena el valor futuro de screenTeam
	string urlClassification =
		default_url; //Variable que almacena el valor inicial de screenClassification

	//Fuentes necesarias para la realización del torneo
	obs_source_t *screenTeam;			 //Source que almacena la screenTeam
	obs_source_t *camTeam;				 //Source que almacena la camTeam
	obs_source_t *screenTeamDummy;			 //Source que almacena la screenTeamDummy
	obs_source_t *camTeamDummy;			 //Source que almacena la camTeamDummy
	obs_source_t *screenClassification;		 //Source que almacena la screenClassification
	obs_source_t *textRotative;			 //Source que almacena el textRotative
	obs_source_t *staticText;			 //Source que almacena el staticText
	obs_source_t *textSubmission;			 //Source que almacena el textSubmision
	obs_source_t *textTeam;				 //Source que almacena el textTeam
	obs_source_t *textTeamImage;			 //Source que almacena el textTeamImage
	obs_source_t *filter;				 //Source que almacena el filtro de rotación del textRotative
	obs_scene_t *teamViewerScene;			 //Scene  que almacena la escena TeamViewer
	obs_scene_t *classificationScene;		 //Scene  que almacena la escena Classification
	struct obs_frontend_source_list transitions ={}; //Lista de sources que almacena las transition

	/*Scene items necesarios para la realización del torneo */
	obs_sceneitem_t
		*camTeamDummyItem; //Scene_Item que almacena la camTeamDummy
	obs_sceneitem_t *
		screenTeamDummyItem; //Scene_Item que almacena la screenTeamDummy
	obs_sceneitem_t *camTeamItem;    //Scene_Item que almacena la camTeam
	obs_sceneitem_t *screenTeamItem; //Scene_Item que almacena la screenTeam

	/*Variables para la cola de entregas*/
	int numSubmissionsPrevious =
		0; //Sirve para no tener que comprobar las entregas ya comprobadas, pues el cUrl devuelve todas las del torneo.

	/*Variables para la prioridades de los QThread*/
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
	uint32_t threadPriority = QThread::
		NormalPriority; //Prioridad utilizada en los QThreads del plugin

	std::vector<int> tabOrder = {
		0, 1, 2}; //Orden del las tabs de la configuración del plugin

	/*Elementos necesarios para los hotkeys del plugin*/
	bool hotkeysRegistered =
		false; //Si se han registrado los hotkeys del plugin dentro de los del obs
	obs_hotkey_id startHotkey;  //Hotkey de inicio del plugin
	obs_hotkey_id stopHotkey;   //Hotkey de final del plugin
	obs_hotkey_id toggleHotkey; //Hotkey para alternar el estado del plugin

	/*Funciones de control del plugin*/
	void
	Thread(); //Función principal del plugin, controlada por el thread principal.
	void
	ThreadSubmissions(); //Función principal del thSub, Thread encargado de actualizar la lista de envios y correcciones
	void
	Start(); //Función que inicia el funcionamiento del plugin, arranca los diversos QThreads
	void
	Stop(); //Funcíón que detiene el funcionamiento del plugin, para y elimina los diversos QThreads

	/*Funciones encargadas del cambio en las escenas y fuentes principales*/
	void
	switchScene(obs_source_t *transition); //,unique_lock<std::mutex> & lock
	void
	switchIP(); //Función encargada de cambiar los scene item de una escena
	void modificaVLC(
		obs_source_t *source,
		string ip); //Función encargada de modificar el contenido de una VLCSource

	bool
	sceneChangedDuringWait(); //Función encargada de comprobar si se ha cambiado de escena durante una espera

	obs_source_t *
	selectRandomTransition(); //Función encargada de devolver una transición random.

	void saveGeneralSettings(obs_data_t *obj);
	void saveHotkeys(obs_data_t *obj);
	void loadGeneralSettings(obs_data_t *obj);
	void loadHotkeys(obs_data_t *obj);

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
