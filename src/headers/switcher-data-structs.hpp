#pragma once
#include <condition_variable>
#include <string>
#include <vector>
#include <mutex>
#include <QDateTime>
#include <QThread>

#include "switch-transitions.hpp"
#include "importUrl.h"

constexpr auto default_interval = 300;
constexpr auto previous_scene_name = "Previous Scene";

typedef enum { NO_SWITCH = 0, SWITCH = 1} NoMatch;
typedef enum { PERSIST = 0, START = 1, STOP = 2 } StartupBehavior;
typedef enum {
	RECORDING = 0,
	STREAMING = 1,
	RECORINDGSTREAMING = 2
} AutoStartType;

class SwitcherThread;

/********************************************************************************
 * SwitcherData
 ********************************************************************************/
struct SwitcherData {
	SwitcherThread *th = nullptr;

	std::condition_variable cv;
	std::mutex m;
	bool transitionActive = false;
	bool waitForTransition = false;
	std::condition_variable transitionCv;
	bool stop = false;
	bool verbose = false;
	bool tansitionOverrideOverride = false;
	bool importedUrls = false;

	string urlCam = "www.youtube.com/embed/4sJ1YMkXm28?autoplay=1";
	string urlScreen = "www.youtube.com/embed/4sJ1YMkXm28?autoplay=1";
	string urlClassification = "www.youtube.com/embed/4sJ1YMkXm28?autoplay=1";

	string contestName = "Contest";
	int interval = default_interval;

	obs_source_t *waitScene = NULL;
	OBSWeakSource previousScene = NULL;
	OBSWeakSource PreviousScene2 = NULL;
	OBSWeakSource nonMatchingScene;
	NoMatch switchIfNotMatching = NO_SWITCH;
	StartupBehavior startupBehavior = PERSIST;

	UrlsContest urlsContest;
	OBSSource screenTeam;
	OBSSource camTeam;
	OBSSource screenClassification;
	OBSSource ClassificationView;

	bool autoStopEnable = false;
	OBSWeakSource autoStopScene;

	bool autoStartEnable = false;
	AutoStartType autoStartType = RECORDING;
	OBSWeakSource autoStartScene;
	bool autoStartedRecently = false;

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

	bool sceneChangedDuringWait();
	
	void setDefaultSceneTransitions();
	void autoStopStreamAndRecording();
	void autoStartStreamRecording();
	
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
