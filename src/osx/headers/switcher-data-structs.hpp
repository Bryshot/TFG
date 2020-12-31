#pragma once
#include <condition_variable>
#include <string>
#include <vector>
#include <mutex>
#include <QDateTime>
#include <QThread>
#include <curl/curl.h>

#include "switch-file.hpp"
#include "switch-media.hpp"
#include "switch-random.hpp"
#include "switch-transitions.hpp"
#include "swtich-sequence.hpp"


//Tiempo mínimo entre cambio y cambio de escena
constexpr auto default_interval = 300;

//Nombre default de la escena anterior
constexpr auto previous_scene_name = "Previous Scene";

//Tipo enumerado que se encarga de la lógica que se debe realizar cuando no hay ningun switch match
typedef enum { NO_SWITCH = 0, SWITCH = 1, RANDOM_SWITCH = 2 } NoMatch;

//Tipo enumerado que se encarga del comportamiento que se utiliza al iniciar el plugin
typedef enum { PERSIST = 0, START = 1, STOP = 2 } StartupBehavior;

//Tipo enumerado que se encarga del comportamiento al comenzar a funcionar el plugin
typedef enum { RECORDING = 0, STREAMING = 1, RECORINDGSTREAMING = 2 } AutoStartType;

//Definición de la clase del thread encargado del programa
class SwitcherThread;

/********************************************************************************
 * SwitcherData
 ********************************************************************************/
///Estructura de datos encargada de almacenar toda la información necesaria del plugin
struct SwitcherData {
	//Thread encargado del plugin
	SwitcherThread *th = nullptr;
	std::condition_variable cv;
	std::mutex m;

	/// Si hay una transicion activa
	bool transitionActive = false;
	/// Si se esta esperando por una 
	bool waitForTransition = false;
	std::condition_variable transitionCv;

	/// Si el plugin esta parado
	bool stop = false;
	/// Si se debe escribir por consola o no la salida
	bool verbose = false;
	///(?)
	bool transitionOverrideOverride = false;

	///Intervalo de tiempo mínimo entre cambio y cambio de escena
	int interval = default_interval;

	obs_source_t *waitScene = NULL;
	///Anterior escena
	OBSWeakSource previousScene = NULL;
	OBSWeakSource PreviousScene2 = NULL;
	///Última escena escogida aleatoriamente
	OBSWeakSource lastRandomScene;
	///Escena que se pone cuando no se realiza ningún match y la lógica de notMatch lo indica
	OBSWeakSource nonMatchingScene;

	///Enumerado que se encarga de la lógica que se debe realizar cuando no hay ningun switch match
	NoMatch switchIfNotMatching = NO_SWITCH;
	///Enumerado que se encarga del comportamiento que se utiliza al iniciar el plugin
	StartupBehavior startupBehavior = PERSIST;

	///Vector de SceneSequenceSwitch
	std::vector<SceneSequenceSwitch> sceneSequenceSwitches;
	///Multiplicador para identificar si el tiempo entre cambio y cambio dentro de la secuencia esta en segundos, minutos o horas
	int sceneSequenceMultiplier = 1;

	///Vector de randomSwitch
	std::vector<RandomSwitch> randomSwitches;

	///Estructura que guarda el fichero del que se leen y se escriben los fileSwitches
	FileIOData fileIO;
	//Array de fileSwitches
	std::vector<FileSwitch> fileSwitches;
	//Direccion url del archivo si es que se obtiene por medio de internet
	CURL *curl = nullptr;

	//Si se debe detener de grabar y retransmitir solo (Cuando la escena actual sea autoStopScene)
	bool autoStopEnable = false;
	//Escena en la que se debe estar para que se pare automáticamente la grabación y la retransmición
	OBSWeakSource autoStopScene;

	//Si se debe empezar a grabar y retransmitir solo (Cuando la escena actual sea autoStartScene)
	bool autoStartEnable = false;
	//Que se debe iniciar cuando se autoinicia la grabación/retransmición
	AutoStartType autoStartType = RECORDING;
	//Escena en la que se debe estar para que se inicie automáticamente la grabación y la retransmición
	OBSWeakSource autoStartScene;
	//Si se ha iniciado hace poco la grabación y la retransmición (Para evitar reentradas en la función de inicio automático)
	bool autoStartedRecently = false;

	//Vector de transiciones
	std::vector<SceneTransition> sceneTransitions;
	//Vector de transiciones de escena por default
	std::vector<DefaultSceneTransition> defaultSceneTransitions;

	//Vector de mediaSwitches 
	std::vector<MediaSwitch> mediaSwitches;
	//Fecha actual
	QDateTime liveTime;

	//Vector de prioridades por nombre
	std::vector<int> functionNamesByPriority = std::vector<int>{
		default_priority_0, default_priority_1,
		default_priority_2};

	///Estructura de datos para guardar la prioridad de cada thread, asociandolo a su nombre y una descripción
	struct ThreadPrio {
		std::string name;
		std::string description;
		uint32_t value;
	};

	//Vector de ThreadPrio; que representa la prioridad para cada thread
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

	//La prioridad del thread que dirige el plugin.
	uint32_t threadPriority = QThread::NormalPriority;

	//Vector con el orden de las tabs en la configuración
	std::vector<int> tabOrder;

	//Si hay algún hotkey relacionado con el plugin
	bool hotkeysRegistered = false;
	//Hotkey de inicio del plugin
	obs_hotkey_id startHotkey;
	//Hotkey de parada del plugin
	obs_hotkey_id stopHotkey;
	//Hotkey de alternar el estado del plugin
	obs_hotkey_id toggleHotkey;

	void Thread();
	void Start();
	void Stop();

	bool sceneChangedDuringWait();
	bool prioFuncsValid();
	void writeSceneInfoToFile();
	void setDefaultSceneTransitions();
	void autoStopStreamAndRecording();
	void autoStartStreamRecording();
	void checkSceneSequence(bool &match, OBSWeakSource &scene,
				 OBSWeakSource &transition,
				 std::unique_lock<std::mutex> &lock);
	void checkSwitchInfoFromFile(bool &match, OBSWeakSource &scene,
				     OBSWeakSource &transition);
	void checkFileContent(bool &match, OBSWeakSource &scene,
			      OBSWeakSource &transition);
	void checkRandom(bool &match, OBSWeakSource &scene,
			 OBSWeakSource &transition, int &delay);
	void checkMediaSwitch(bool &match, OBSWeakSource &scene,
			      OBSWeakSource &transition);

	/// <summary>
	/// Funcion encargada de guardar la configuración de las
	/// </summary>
	void saveSceneSequenceSwitches(obs_data_t *obj);
	void saveSceneTransitions(obs_data_t *obj);
	void saveRandomSwitches(obs_data_t *obj);
	void saveFileSwitches(obs_data_t *obj);
	void saveMediaSwitches(obs_data_t *obj);
	void saveGeneralSettings(obs_data_t *obj);
	void saveHotkeys(obs_data_t *obj);

	void loadSceneSequenceSwitches(obs_data_t *obj);
	void loadSceneTransitions(obs_data_t *obj);
	void loadRandomSwitches(obs_data_t *obj);
	void loadFileSwitches(obs_data_t *obj);
	void loadMediaSwitches(obs_data_t *obj);
	void loadGeneralSettings(obs_data_t *obj);
	void loadHotkeys(obs_data_t *obj);

	void Prune();
	inline ~SwitcherData() { Stop(); }
};

extern SwitcherData *switcher;

class SwitcherThread : public QThread {
public:
	explicit SwitcherThread(){};

	//Función encargada de arrancar el thread
	void run() { switcher->Thread(); };
};
