#pragma once
#include <condition_variable>
#include <string>
#include <vector>
#include <mutex>
#include <QDateTime>
#include <QThread>
#include <curl/curl.h>
#include "switch-transitions.hpp"
#include "importUrl.h"

constexpr auto default_interval = 300;
constexpr auto previous_scene_name = "Previous Scene";
constexpr auto default_sizeRotativeText = 48;
constexpr auto default_speedRotativeText = 100;
constexpr auto default_contestName = "Contest";
constexpr auto default_rotatingText = "Default rotating text";
constexpr auto default_url = "www.youtube.com/embed/4sJ1YMkXm28?autoplay=1";
const string default_contestServer = "https://www.domjudge.org/demoweb";

class SwitcherThread;

/********************************************************************************
 * SwitcherData
 ********************************************************************************/
struct SwitcherData {
	SwitcherThread *th = nullptr;

	std::condition_variable cv;
	std::mutex m;
	CURL *curl = nullptr;

	bool transitionActive = false;
	bool waitForTransition = false;
	std::condition_variable transitionCv;
	bool stop = false;
	bool verbose = false;
	bool tansitionOverrideOverride = false;
	bool importedUrls = false;
	bool usingDummy = false;
	bool created = false;

	string contestName = default_contestName;
	UrlsContest urlsContestData;
	int interval = default_interval;

	/*Elementos necesarios para el texto rotativo */
	string textRotativeContent = default_rotatingText;
	int sizeRotativeText = default_sizeRotativeText;
	int speedRotativeText = default_speedRotativeText;

	/*Elementos necesarios para obtener la info del concurso*/

	string contestServerWebsite = default_contestServer;
	string curlContest;
	string curlProblems;
	string curlScoreboard;
	string curlTeams;
	

	obs_source_t *waitScene = NULL;
	OBSWeakSource previousScene = NULL;
	OBSWeakSource PreviousScene2 = NULL;

	/*Url de las fuentes actuales*/
	string ipCam = "";
	string ipScreen = "";
	string urlClassification = default_url;

	/*Fuentes necesarias para la realización del torneo*/
	OBSSource screenTeam;
	OBSSource camTeam;
	OBSSource screenTeamDummy;
	OBSSource camTeamDummy;
	OBSSource screenClassification;
	OBSSource textRotative;
	OBSSource filter;

	/*Scene items necesarios para la realización del torneo */
	obs_sceneitem_t *camTeamDummyItem;
	obs_sceneitem_t *screenTeamDummyItem;
	obs_sceneitem_t *camTeamItem;
	obs_sceneitem_t *screenTeamItem;

	std::vector<SceneTransition> sceneTransitions;
	std::vector<DefaultSceneTransition> defaultSceneTransitions;
	
	QDateTime liveTime;

	struct ThreadPrio {
		std::string name;
		std::string description;
		uint32_t value;
	};

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

	uint32_t threadPriority = QThread::NormalPriority;

	std::vector<int> tabOrder;

	bool hotkeysRegistered = false;
	obs_hotkey_id startHotkey;
	obs_hotkey_id stopHotkey;
	obs_hotkey_id toggleHotkey;

	void Thread();
	void Start();
	void Stop();

	void switchIP(string ipScreen, string ipCam,
		       unique_lock<mutex> &lock);

	void modificaVLC(obs_source_t *source, string ip);

	bool sceneChangedDuringWait();
	
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
class SwitcherThread : public QThread {
public:
	explicit SwitcherThread(){};
	void run() { switcher->Thread(); };
};
