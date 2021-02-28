#include <QMainWindow>
#include <QDir>
#include <QAction>

#include <condition_variable>
#include <chrono>
#include <vector>
#include <mutex>
#include <thread>

#include <obs-frontend-api.h>
#include <obs-module.h>
#include <obs.hpp>
#include <obs.h>
#include <util/util.hpp>

#include "headers/switcher-data-structs.hpp"
#include "headers/advanced-scene-switcher.hpp"
#include "headers/utility.hpp"
#include "headers/curl-helper.hpp"
#include "headers/curlCore.h"

SwitcherData *switcher = nullptr;

/// <summary>
/// Función encargada de inicializar el plugin
/// </summary>
/// <param name="parent"></param>
SceneSwitcher::SceneSwitcher(QWidget *parent)
	: QDialog(parent), ui(new Ui_SceneSwitcher)
{
	ui->setupUi(this);

	std::lock_guard<std::mutex> lock(switcher->m);

	switcher->Prune();
	loadUI();
}

/// <summary>
/// Añade todas las escenas en sel; si addPrevious es true, tambien se añade la última escena
/// </summary>
/// <param name="sel">ComboBox donde estaran todas las escenas</param>
/// <param name="addPrevious">Booleano de selección</param>
void SceneSwitcher::populateSceneSelection(QComboBox *sel, bool addPrevious)
{
	BPtr<char *> scenes = obs_frontend_get_scene_names();
	char **temp = scenes;
	while (*temp) {
		const char *name = *temp;
		sel->addItem(name);
		temp++;
	}

	if (addPrevious)
		sel->addItem(previous_scene_name);
}

/// <summary>
/// Añade todas las transiciones en sel
/// </summary>
/// <param name="sel">ComboBox donde estaran todas las transiciones</param>
void SceneSwitcher::populateTransitionSelection(QComboBox *sel)
{
	obs_frontend_source_list *transitions = new obs_frontend_source_list();
	obs_frontend_get_transitions(transitions);

	for (size_t i = 0; i < transitions->sources.num; i++) {
		const char *name =
			obs_source_get_name(transitions->sources.array[i]);
		sel->addItem(name);
	}

	obs_frontend_source_list_free(transitions);
}

/// <summary>
/// Función que llama a las funciones encargadas de la lógica de cada Tab de la config del Plugin
/// </summary>
void SceneSwitcher::loadUI()
{
#if __APPLE__
	setMinimumHeight(700);
#endif
	setupGeneralTab();
	setupTransitionsTab();
	setTabOrder();

	loading = false;
}


/********************************************************************************
 * Saving and loading the settings
 ********************************************************************************/
static void SaveSceneSwitcher(obs_data_t *save_data, bool saving, void *)
{
	if (saving) {
		switcher->Stop();
		std::lock_guard<std::mutex> lock(switcher->m);

		switcher->Prune();

		obs_data_t *obj = obs_data_create();

		switcher->saveSceneTransitions(obj);
		switcher->saveGeneralSettings(obj);
		switcher->saveHotkeys(obj);

		obs_data_set_obj(save_data, "advanced-scene-switcher", obj);

		obs_data_release(obj);
	} else {
		switcher->m.lock();

		obs_data_t *obj = obs_data_get_obj(save_data, "advanced-scene-switcher");

		if (!obj)
			obj = obs_data_create();

		switcher->loadSceneTransitions(obj);
		switcher->loadGeneralSettings(obj);
		switcher->loadHotkeys(obj);

		obs_data_release(obj);

		switcher->m.unlock();

		// stop the scene switcher at least once
		// to avoid issues with scene collection changes
		bool start = !switcher->stop;
		switcher->Stop();
		if (start)
			switcher->Start();
	}
}
/********************************************************************************
 * Main switcher thread
 ********************************************************************************/
void SwitcherData::Thread()
{
	//Anunciamos el inicio del funcionamiento del plugin
	blog(LOG_INFO, "AutoProducer started");

	int sleep = 0;
	std::unique_lock<std::mutex> lock(m);//Inutil

	//Obtenemos la información inicial del concurso
	contestInfo contest = getContestRealTimeInfo();

	while (true) {
	startLoop:
		bool switchS;
		OBSWeakSource scene;
		OBSWeakSource transition;
		std::chrono::milliseconds duration = std::chrono::milliseconds(sleep);

		if (verbose)
			blog(LOG_INFO,"AutoProducer sleep for %d",sleep);

		//Se recargan las 
		switcher->Prune();
		waitScene = obs_frontend_get_current_scene();
		//Tiempom de espera entre switch y switch
		cv.wait_for(lock, duration);

		//Comprueba si se ha detenido el programa durante el sleep.
		if (switcher->stop) break;
		setDefaultSceneTransitions();
		
		/// <summary>
		/// Decisor
		/// </summary>

		if (sceneChangedDuringWait()) //scene might have changed during the sleep
		{
			goto startLoop;
		}

		if (switcher->stop) {
			goto endLoop;
		}	

		if (switchS) {
			switchScene(scene, transition, tansitionOverrideOverride, lock);
		} else
		{
		}

	}
endLoop:
	blog(LOG_INFO, "AutoProducer stopped");
}

/********************************************************************************
 * Thread submissions
 ********************************************************************************/

void SwitcherData::ThreadSubmissions() {

	while (!switcher->stop) {
		//Codigo para actualizar barra de submissions
	}
}

void switchScene(OBSWeakSource &scene, OBSWeakSource &transition,
		 bool &transitionOverrideOverride,
		 std::unique_lock<std::mutex> &lock)
{
	obs_source_t *source = obs_weak_source_get_source(scene);
	obs_source_t *currentSource = obs_frontend_get_current_scene();

	if (source && source != currentSource) {

		// this call might block when OBS_FRONTEND_EVENT_SCENE_CHANGED is active and mutex is held
		// thus unlock mutex to avoid deadlock
		//lock.unlock();

		transitionData td;
		setNextTransition(scene, currentSource, transition,
				  transitionOverrideOverride, td);
		obs_frontend_set_current_scene(source);
		if (transitionOverrideOverride)
			restoreTransitionOverride(source, td);
		lock.lock();

		if (switcher->verbose)
			blog(LOG_INFO,
			     "AutoProducer switched scene");
	}
	obs_source_release(currentSource);
	obs_source_release(source);
}

void SwitcherData::switchIP(string ipScreen, string ipCam,
	       unique_lock<mutex> &lock)
{
	obs_source_t *currentSource = obs_frontend_get_current_preview_scene();
	string nombreEscena = obs_source_get_name(currentSource);

	if (nombreEscena == "TeamView") {

		obs_source_t *screen;
		obs_source_t *cam;

		if (switcher->usingDummy) {
			screen = switcher->screenTeam;
			cam = switcher->camTeam;
		}
		else
		{
			screen = switcher->screenTeamDummy;
			cam = switcher->camTeamDummy;
		}

		switcher->modificaVLC(screen, ipScreen);
		switcher->modificaVLC(cam, ipCam);

		cv.wait_for(lock, chrono::milliseconds(15000));

		if (switcher->usingDummy)
		{
			obs_sceneitem_set_visible(switcher->screenTeamDummyItem,false);
			obs_sceneitem_set_visible(switcher->camTeamDummyItem, false);
			obs_sceneitem_set_visible(switcher->screenTeamItem,true);
			obs_sceneitem_set_visible(switcher->camTeamItem, true);
		}
		else
		{
			obs_sceneitem_set_visible(switcher->screenTeamDummyItem,true);
			obs_sceneitem_set_visible(switcher->camTeamDummyItem, true);
			obs_sceneitem_set_visible(switcher->screenTeamItem, false);
			obs_sceneitem_set_visible(switcher->camTeamItem, false);
		}
		switcher->usingDummy = !switcher->usingDummy;
	}
}

void SwitcherData::modificaVLC(obs_source_t* source, string ip) {

	obs_data_t *data = obs_source_get_settings(source);
	obs_data_array_t *array = obs_data_get_array(data, "playlist");
	obs_data_t *d = obs_data_array_item(array, 0);

	obs_data_set_string(d, "value", ip.c_str());
	obs_data_release(d);
	obs_data_array_release(array);
	obs_source_update(source, data);
}

bool SwitcherData::sceneChangedDuringWait()
{
	bool r = false;
	obs_source_t *currentSource = obs_frontend_get_current_scene();
	if (!currentSource)
		return true;
	obs_source_release(currentSource);
	if (waitScene && currentSource != waitScene)
		r = true;
	return r;
}

void SwitcherData::Start()
{
	if (!(th && th->isRunning() && thSub && thSub->isRunning())) {
		stop = false;
		switcher->th = new SwitcherThread();
		switcher->th->start(
			(QThread::Priority)switcher->threadPriority);
		switcher->thSub = new SwitcherThreadSubmissions();
		switcher->thSub->start(
			(QThread::Priority)switcher->threadPriority);
	}
}

void SwitcherData::Stop()
{
	if (th && th->isRunning() && thSub && thSub->isRunning())
	{
		switcher->stop = true;
		transitionCv.notify_one();
		cv.notify_one();
		th->wait();
		thSub->wait();
		delete th;
		delete thSub;
		th = nullptr;
		thSub = nullptr;
	}
}

/********************************************************************************
 * OBS module setup
 ********************************************************************************/
extern "C" void FreeSceneSwitcher()
{
	if (loaded_curl_lib) {
		if (switcher->curl && f_curl_cleanup)
			f_curl_cleanup(switcher->curl);
		delete loaded_curl_lib;
		loaded_curl_lib = nullptr;
	}

	delete switcher;
	switcher = nullptr;
}

void handleSceneChange(SwitcherData *s)
{
	std::lock_guard<std::mutex> lock(s->m);
	//stop waiting if scene was manually changed
	if (s->sceneChangedDuringWait())
		s->cv.notify_one();

	//set previous scene
	obs_source_t *source = obs_frontend_get_current_scene();
	obs_weak_source_t *ws = obs_source_get_weak_source(source);
	obs_source_release(source);
	obs_weak_source_release(ws);
	if (source && s->PreviousScene2 != ws) {
		s->previousScene = s->PreviousScene2;
		s->PreviousScene2 = ws;
	}
}


/// <summary>
/// Funcion encargada de gestionar los eventos de obs que puedan ocurrir.
/// </summary>
/// <param name=""></param>
/// <param name="switcher"></param>
static void OBSEvent(enum obs_frontend_event event, void *switcher)
{
	switch (event) {
	case OBS_FRONTEND_EVENT_EXIT:
		FreeSceneSwitcher();
		break;
	case OBS_FRONTEND_EVENT_SCENE_CHANGED:
		handleSceneChange((SwitcherData *)switcher);
		break;
	default:
		break;
	}
}

extern "C" void InitSceneSwitcher()
{
	QAction *action = (QAction *)obs_frontend_add_tools_menu_qaction(
		"Advanced Scene Switcher");

	switcher = new SwitcherData;

	if (loadCurl() && f_curl_init) {
		switcher->curl = f_curl_init();
	}

	auto cb = []() {
		QMainWindow *window =
			(QMainWindow *)obs_frontend_get_main_window();

		SceneSwitcher ss(window);
		ss.exec();
	};

	obs_frontend_add_save_callback(SaveSceneSwitcher, nullptr);
	obs_frontend_add_event_callback(OBSEvent, switcher);

	action->connect(action, &QAction::triggered, cb);
}
