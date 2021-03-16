#include <QMainWindow>
#include <QDir>
#include <QAction>
#include <condition_variable>
#include <chrono>
#include <vector>
#include <mutex>
#include <stdlib.h> 
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
contestInfo contestRealData;

/// <summary>
/// Función encargada de inicializar el plugin
/// </summary>
/// <param name="parent"></param>
SceneSwitcher::SceneSwitcher(QWidget *parent)
	: QDialog(parent), ui(new Ui_SceneSwitcher)
{
	ui->setupUi(this);

	std::lock_guard<std::mutex> lock(switcher->m);

	//switcher->Prune();

	loadUI();
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
	//setupTransitionsTab();
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

		//switcher->Prune();

		obs_data_t *obj = obs_data_create();

		switcher->saveGeneralSettings(obj);
		switcher->saveHotkeys(obj);

		obs_data_set_obj(save_data, "advanced-switcher", obj);

		obs_data_release(obj);
	} else {
		switcher->m.lock();

		obs_data_t *obj = obs_data_get_obj(save_data, "advanced-switcher");

		if (!obj)
			obj = obs_data_create();

		
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
	blog(LOG_INFO, "Advanced Switcher started");

	std::unique_lock<std::mutex> lock(m);

	//Obtenemos la información inicial del concurso
	contestRealData = getContestRealTimeInfo();

	updateTextRotative(switcher->textRotative);

	while (true) {

		/*Declaración de las variables*/
		std::chrono::milliseconds duration = std::chrono::milliseconds(interval);
		obs_frontend_get_transitions(&switcher->transitions);

		if (verbose)
			blog(LOG_INFO, "AutoProducer sleep for %d", interval);

		//Se recargan las variables de control para el wait (Revisar)
		switcher->Prune();
		waitScene = obs_frontend_get_current_scene();

		//Tiempo  de espera entre switch y switch
		cv.wait_for(lock, duration);

		//Comprueba si se ha detenido el programa durante el sleep.
		if (switcher->stop) break;
		//else if (sceneChangedDuringWait()) //scene might have changed during the sleep
		//	goto startLoop;

		obs_source_t * transition = selectRandomTransition();
		//setDefaultSceneTransitions();//Revisar

		//Actualización de la información del torneo y realiza el calculo heurístico
		updateContestRealTimeInfo(contestRealData);
		
		/*Si durante el proceso de  decisión se para el plugin, se cancela el cambio*/
		if (switcher->stop) {
			goto endLoop;
		}

		if (switcher->swapIp) //Realiza el cambio de las vlcSources
			switchIP(lock);
		if (switcher->swapScene) //Realiza el cambio de escenas
			switchScene(transition, lock);
		
		

	}
endLoop:
	blog(LOG_INFO, "AutoProducer stopped");
}

/********************************************************************************
 * Thread submissions
 ********************************************************************************/

void SwitcherData::ThreadSubmissions() {

	while (true) {
		//Variables del thread
		int i = 0;
		string tmp = "";

		/*Establecer un lock simple en el mapa submissionPendings*/
		while (!switcher->updatedSubmissions) {
			std::this_thread::sleep_for(std::chrono::milliseconds(switcher->delayJugdment));
		}

		//Codigo para actualizar barra de submissions
		//getJudgementsInfo(contestRealData);

		map<string, submissionInfo>::iterator it = contestRealData.submissionPendings.begin();

		/*Bucle para cada envio*/
		while (it != contestRealData.submissionPendings.end() && i < MAX_VISIBLE_TEAMS_SUBMISSION) {

			int slot1 = 11;
			int slot2 = 23;
			map<string, teamInfo>::iterator it2 = contestRealData.scoreBoard.find(it->second.idTeam);
			string tmp2 = "";

			//string tmp3 = it2->second.identificationInfo.name;
			//std::for_each(tmp3.begin(),tmp3.end(),[](char &c) { c = ::tolower(c); });
			tmp2 += it2->second.identificationInfo.name.substr(0,7);
			insertSpaces(tmp2, slot1 - (int)tmp2.size());

			//tmp3 = it->second.idProblem;
			//std::for_each(tmp3.begin(),tmp3.end(),[](char &c) { c = ::tolower(c); });
			tmp2 += it->second.idProblem; /*Quiza crear una lista de pair con id y nombre*/


			if (slot2 - tmp2.size() <= 0) {
				insertSpaces(tmp2, 1);
			}
			else
				insertSpaces(tmp2, slot2 - (int)tmp2.size());
			

			/*Sumar espacios para maquetación*/

			tmp2 += it->second.result + "\n";
			
			i++;
			if (it->second.result != pendingJugment) 
			{
				string temp = it->first;
				it++;
				contestRealData.submissionPendings.erase(contestRealData.submissionPendings.find(temp));
			}
			else 
				it++;
			tmp += tmp2;
		}

		switcher->updatedSubmissions = false;
		updateTextSubmissionContent(tmp);
		//Espera temporal entre refrescos

		//Si se para el programa tambien se para este thread
		if (switcher->stop) {
			return;
		}
	}

}

void switchScene(obs_source_t *transition,std::unique_lock<std::mutex> &lock)
{
	obs_source_t *currentSource = obs_frontend_get_current_scene();
	obs_source_t *sourceTeamviewer = obs_scene_get_source(switcher->teamViewerScene);
	obs_source_t *sourceClassification = obs_scene_get_source(switcher->classificationScene);
	obs_source_t *source = nullptr;

	if (currentSource == sourceTeamviewer)
		source = sourceClassification;
	else if (currentSource == sourceClassification)
		source = sourceTeamviewer;

	// this call might block when OBS_FRONTEND_EVENT_SCENE_CHANGED is active and mutex is held
	// thus unlock mutex to avoid deadlock
	lock.unlock();

	obs_frontend_set_current_transition(transition);
	obs_frontend_set_current_scene(source);
	lock.lock();

	if (switcher->verbose)
		blog(LOG_INFO,"Advanced Switcher switched scene");
	switcher->swapScene = false;
	obs_source_release(currentSource);
}

void insertSpaces(string & elem, int numSpaces) {
	for (int i = 0 ; i<numSpaces;i++) {
		elem+=" ";
	}
}

obs_source_t * SwitcherData::selectRandomTransition() {

	int random = rand() % switcher->transitions.sources.num;
	return switcher->transitions.sources.array[random];
}

void SwitcherData::switchIP(unique_lock<mutex> &lock)
{
	obs_source_t *screen;
	obs_source_t *cam;

	if (usingDummy) {
		screen = screenTeam;
		cam = camTeam;
	}
	else
	{
		screen = screenTeamDummy;
		cam = camTeamDummy;
	}

	modificaVLC(screen, ipScreen);
	modificaVLC(cam, ipCam);
	modificaTextTeam(switcher->textTeam, switcher->textTeamContent);

	cv.wait_for(lock, chrono::milliseconds(switcher->delayIp));

	if (usingDummy)
	{
		obs_sceneitem_set_visible(screenTeamDummyItem,false);
		obs_sceneitem_set_visible(camTeamDummyItem, false);
		obs_sceneitem_set_visible(screenTeamItem,true);
		obs_sceneitem_set_visible(camTeamItem, true);
	}
	else
	{
		obs_sceneitem_set_visible(screenTeamDummyItem,true);
		obs_sceneitem_set_visible(camTeamDummyItem, true);
		obs_sceneitem_set_visible(screenTeamItem, false);
		obs_sceneitem_set_visible(camTeamItem, false);
	}
	usingDummy = !usingDummy;
	swapIp = false;
}

void SwitcherData::modificaVLC(obs_source_t* source, string ip) {

	obs_data_t *data = obs_source_get_settings(source);
	obs_data_array_t *array = obs_data_get_array(data, "playlist");
	obs_data_t *d = obs_data_array_item(array, 0);

	obs_data_set_string(d, "value", ip.c_str());
	obs_data_release(d);
	obs_data_array_release(array);
	obs_source_update(source, data);
	obs_data_release(data);
}

void updateTextRotative(obs_source_t * source) {

	switcher->textRotativeContent += "| Name of the contest = "+ switcher->contestName + " | Start of the contest:"+ contestRealData.start_time + " | End of the contest:"+contestRealData.end_time+" |";
	obs_data_t *data = obs_source_get_settings(source);
	obs_data_set_string(data, "text", switcher->textRotativeContent.c_str());
	obs_source_update(source, data);
	obs_data_release(data);
}

void modificaTextTeam(obs_source_t *source, string text)
{

	obs_data_t *data = obs_source_get_settings(source);
	obs_data_set_string(data, "text", text.c_str());
	obs_source_update(source, data);
	obs_data_release(data);
	
}

void SwitcherData::updateTextSubmissionContent(string text)
{
	obs_data_t *data = obs_source_get_settings(switcher->textSubmission);
	obs_data_set_string(data, "text", text.c_str());
	obs_source_update(switcher->textSubmission, data);
	obs_data_release(data);
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
		switcher->thSub->start((QThread::Priority)switcher->threadPriority);
	}
}

void SwitcherData::Stop()
{
	if (th && th->isRunning() && thSub && thSub->isRunning())
	{
		switcher->stop = true;
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
		//switcher->curlAdmin = f_curl_init();
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
