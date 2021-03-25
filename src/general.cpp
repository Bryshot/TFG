#include <obs-module.h>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>

#include "headers/advanced-scene-switcher.hpp"
#include "headers/importIPs.h"
#include "headers/curl-helper.hpp"

void SceneSwitcher::on_close_clicked()
{
	done(0);
}

void SceneSwitcher::on_threadPriority_currentIndexChanged(int index) {

	if (switcher->loading)
		return;

	//std::lock_guard<std::mutex> lock(switcher->m);
	switcher->threadPriority = switcher->threadPriorities[index].value;
}

void SceneSwitcher::on_checkInterval_valueChanged(int value)
{
	if (switcher->loading)
		return;

	switcher->interval = value;
}

void SceneSwitcher::on_delayJudgments_valueChanged(int value)
{
	if (switcher->loading)
		return;

	switcher->delayJugdment = value;
}

void SceneSwitcher::on_delayIp_valueChanged(int value)
{
	if (switcher->loading)
		return;

	switcher->delayIp = value;
}

void updateSpeedRotativeText(SwitcherData *switcher)
{
	obs_data_t *data = obs_data_create();
	obs_data_set_int(data, "speed_x", switcher->speedRotativeText);
	obs_source_update(switcher->filter, data);
	obs_data_release(data);
}

void SceneSwitcher::on_SpeedRotation_valueChanged(int value)
{
	if (switcher->loading)
		return;

	switcher->speedRotativeText = value;
	//Actualizar la velocidad
	if (switcher->created)
		updateSpeedRotativeText(switcher);
}

void SceneSwitcher::on_weightOfRank_valueChanged(double value)
{
	if (switcher->loading)
		return;

	switcher->rankWeight = value;
}

void SceneSwitcher::on_weightOfTime_valueChanged(double value)
{
	if (switcher->loading)
		return;

	switcher->timeInStreamWeight = value;
}

void SceneSwitcher::on_weightOfPending_valueChanged(double value)
{
	if (switcher->loading)
		return;

	switcher->numPendingWeight = value;
}

void SceneSwitcher::on_numberOfCycle_valueChanged(int value)
{
	if (switcher->loading)
		return;

	switcher->cycleSize = value;
}

void SceneSwitcher::SetStarted()
{
	ui->toggleStartButton->setText("Stop");
	ui->pluginRunningText->setText("Active");
}

void SceneSwitcher::SetStopped()
{
	ui->toggleStartButton->setText("Start");
	ui->pluginRunningText->setText("Inactive");
}

void SceneSwitcher::on_toggleStartButton_clicked()
{
	if (switcher->th && switcher->th->isRunning()) {
		switcher->Stop();
		SetStopped();
	} else if(switcher->importedIPs){
		switcher->Start();
		SetStarted();
	}
	else{
		QMessageBox Msgbox;
		Msgbox.setText("Import the IPs for the contest before start the plugin");
		Msgbox.exec();
		return;
	}
}

void SceneSwitcher::closeEvent(QCloseEvent *)
{
	done(0);
}

void SceneSwitcher::on_verboseLogging_stateChanged(int state)
{
	if (switcher->loading)
		return;

	switcher->verbose = state;
}

void SceneSwitcher::on_exportSettings_clicked()
{
	QString directory = QFileDialog::getSaveFileName(
		this, tr("Export AutoProducer settings to file ..."),
		QDir::currentPath(), tr("Text files (*.txt)"));
	if (directory.isEmpty())
		return;

	QFile file(directory);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;

	obs_data_t *obj = obs_data_create();

	switcher->saveGeneralSettings(obj);
	switcher->saveHotkeys(obj);

	obs_data_save_json(obj, file.fileName().toUtf8().constData());

	obs_data_release(obj);
}

void SceneSwitcher::on_importSettings_clicked()
{
	std::lock_guard<std::mutex> lock(switcher->m);

	QString directory = QFileDialog::getOpenFileName(
		this,
		tr("Import AutoProducer settings from file ..."),
		QDir::currentPath(), tr("Text files (*.txt)"));
	if (directory.isEmpty())
		return;

	QFile file(directory);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	obs_data_t *obj = obs_data_create_from_json_file(
		file.fileName().toUtf8().constData());

	if (!obj) {
		QMessageBox Msgbox;
		Msgbox.setText(
			"AutoProducer failed to import settings");
		Msgbox.exec();
		return;
	}

	switcher->loadGeneralSettings(obj);
	switcher->loadHotkeys(obj);

	obs_data_release(obj);

	QMessageBox Msgbox;
	Msgbox.setText(
		"AutoProducer settings imported successfully");
	Msgbox.exec();
	setupGeneralTab();
}

void SceneSwitcher::on_importIPs_clicked() {

	QString directory = QFileDialog::getOpenFileName(
		this, tr("Import AutoProducer settings from file ..."),
		QDir::currentPath(), tr("Text files (*.txt)"));
	if (directory.isEmpty())
		return;
	
	switcher->ipsContestData = importIpContest(directory.toStdString());
	if (switcher->ipsContestData.numTeams == -1)
	{
		QMessageBox Msgbox;
		Msgbox.setText("AutoProducer failed to import urls");
		Msgbox.exec();
		return;
	}
	QMessageBox Msgbox;
	Msgbox.setText("AutoProducer urls imported successfully");
	Msgbox.exec();

	map<string, IpsTeam>::iterator it =	switcher->ipsContestData.ipsTeams.begin();
	switcher->ipScreen = it->second.ipScreen;
	switcher->ipCam = it->second.ipCam;
	switcher->textTeamImageFile = it->second.urlLogo;
	switcher->urlClassification = switcher->ipsContestData.ipClassification;

	switcher->importedIPs = true;

	if (switcher->created) {
		obs_data_t *dataClassification = obs_source_get_settings(switcher->screenClassification);
		obs_data_t *dataTextTeam = obs_source_get_settings(switcher->textTeam);
		obs_data_t *dataTextTeamImage = obs_source_get_settings(switcher->textTeamImage);

		switcher->modificaVLC(switcher->screenTeam, switcher->ipScreen);
		switcher->modificaVLC(switcher->camTeam, switcher->ipCam);

		obs_data_set_string(dataTextTeamImage, "file", switcher->textTeamImageFile.c_str());
		string tmp = "Team: " + it->first + "\n" +"Classification:\nNumber of Problem Resolved: ";
		obs_data_set_string(dataTextTeam, "text", tmp.c_str());
		obs_data_set_string(dataClassification, "url",switcher->urlClassification.c_str());

		obs_source_update(switcher->screenClassification, dataClassification);
		obs_source_update(switcher->textTeam, dataTextTeam);
		obs_source_update(switcher->textTeamImage, dataTextTeamImage);

		obs_data_release(dataClassification);
		obs_data_release(dataTextTeamImage);
		obs_data_release(dataTextTeam);
	}
	else switcher->textTeamContent ="Team: " + it->first + "\n" +"Classification:\nNumber of Problem Resolved: ";

}

void SceneSwitcher::on_createSetup_clicked() {
	crearConfiguracion(switcher);
}

void SceneSwitcher::on_contestName_textChanged(const QString &text) {

	if (switcher->loading)
		return;

	switcher->contestName = text.toStdString();
}

void SceneSwitcher::on_contestServer_textChanged(const QString &text)
{

	if (switcher->loading)
		return;

	switcher->contestServerWebsite = text.toStdString();
}

void SceneSwitcher::on_userContestServer_textChanged(const QString &text)
{

	if (switcher->loading)
		return;

	switcher->userContestServer = text.toStdString();
}

void SceneSwitcher::on_passwordContestServer_textChanged(const QString &text)
{

	if (switcher->loading)
		return;

	switcher->passwordContestServer = text.toStdString();
}

int findTabIndex(QTabBar *bar, int pos)//
{
	int at = -1;

	QString tabName = "";
	switch (pos) {
	case 0:
		tabName = "General";
		break;
	case 1:
		tabName = "Heuristic";
		break;
	case 2:
		tabName = "Timers";
		break;
	}

	for (int i = 0; i < bar->count(); ++i) {
		if (bar->tabText(i).compare(tabName) == 0) {
			at = i;
			break;
		}
	}
	if (at == -1)
		blog(LOG_INFO, "AutoProducer failed to find tab %s",
		     tabName.toUtf8().constData());

	return at;
}

//Se encarga de establecer el orden de las tabs
void SceneSwitcher::setTabOrder()
{
	QTabBar *bar = ui->tabWidget->tabBar();
	for (int i = 0; i < bar->count(); ++i) {
		int curPos = findTabIndex(bar, switcher->tabOrder[i]);

		if (i != curPos && curPos != -1)
			bar->moveTab(curPos, i);
	}

	connect(bar, &QTabBar::tabMoved, this, &SceneSwitcher::on_tabMoved);
}

void SceneSwitcher::on_tabMoved(int from, int to)
{
	if (switcher->loading)
		return;
	std::swap(switcher->tabOrder[from], switcher->tabOrder[to]);
}

void SwitcherData::saveGeneralSettings(obs_data_t *obj)
{
	obs_data_set_string(obj, "contestName", switcher->contestName.c_str());
	obs_data_set_string(obj, "contestServerWebsite", switcher->contestServerWebsite.c_str());
	obs_data_set_string(obj, "userContestServer", switcher->userContestServer.c_str());
	obs_data_set_string(obj, "passwordContestServer", switcher->passwordContestServer.c_str());
	obs_data_set_int(obj, "speedRotativeText", switcher->speedRotativeText);
	obs_data_set_int(obj, "threadPriority", switcher->threadPriority);
	obs_data_set_bool(obj, "verbose", switcher->verbose);
	obs_data_set_int(obj, "generalTabPos", switcher->tabOrder[0]);
	obs_data_set_int(obj, "heuristicTabPos", switcher->tabOrder[1]);
	obs_data_set_int(obj, "timerTabPos", switcher->tabOrder[2]);

	/*Save Heuristic settings*/
	obs_data_set_double(obj, "rankWeight", switcher->rankWeight);
	obs_data_set_double(obj, "numPendingWeight", switcher->numPendingWeight);
	obs_data_set_double(obj, "timeInStreamWeight", switcher->timeInStreamWeight);
	obs_data_set_int(obj, "cycleSize", switcher->cycleSize);

	/*Save Timers settings*/
	obs_data_set_int(obj, "interval", switcher->interval);
	obs_data_set_int(obj, "delayIp", switcher->delayIp);
	obs_data_set_int(obj, "delayJugdment", switcher->delayJugdment);
}

void SwitcherData::loadGeneralSettings(obs_data_t *obj)
{
	

	switcher->contestName = obs_data_get_string(obj, "contestName");
	switcher->contestServerWebsite = obs_data_get_string(obj, "contestServerWebsite");
	switcher->userContestServer = obs_data_get_string(obj, "userContestServer");
	switcher->passwordContestServer = obs_data_get_string(obj, "passwordContestServer");
	switcher->speedRotativeText = obs_data_get_int(obj, "speedRotativeText");
	switcher->threadPriority = obs_data_get_int(obj, "threadPriority");
	switcher->verbose = obs_data_get_bool(obj, "verbose");

	if (!switcher->loading)
	{
		switcher->tabOrder.clear();
		switcher->tabOrder.emplace_back((int)(obs_data_get_int(obj, "generalTabPos")));
		switcher->tabOrder.emplace_back((int)(obs_data_get_int(obj, "heuristicTabPos")));
		switcher->tabOrder.emplace_back((int)(obs_data_get_int(obj, "timerTabPos")));
	}
	

	/*Load Heuristic settings*/
	switcher->rankWeight = obs_data_get_double(obj, "rankWeight");
	switcher->numPendingWeight = obs_data_get_double(obj, "numPendingWeight");
	switcher->timeInStreamWeight = obs_data_get_double(obj, "timeInStreamWeight");
	switcher->cycleSize = obs_data_get_int(obj, "cycleSize");

	/*Load Timers settings*/
	switcher->interval = obs_data_get_int(obj, "interval");
	switcher->delayIp = obs_data_get_int(obj, "delayIp");
	switcher->delayJugdment = obs_data_get_int(obj, "delayJugdment");

}


//Se encarga de cargar los valores de las variables del gui
void SceneSwitcher::setupGeneralTab()
{
	ui->verboseLogging->setChecked(switcher->verbose);

	ui->contestName->setText(switcher->contestName.c_str());
	ui->passwordContestServer->setText(switcher->passwordContestServer.c_str());
	ui->userContestServer->setText(switcher->userContestServer.c_str());
	ui->SpeedRotation->setValue(switcher->speedRotativeText);
	ui->contestServer->setText(switcher->contestServerWebsite.c_str());


	for (int i = 0; i < (int)switcher->threadPriorities.size(); ++i) {
		ui->threadPriority->addItem(
			switcher->threadPriorities[i].name.c_str());
		ui->threadPriority->setItemData(
			i, switcher->threadPriorities[i].description.c_str(),
			Qt::ToolTipRole);
		if (switcher->threadPriority ==
		    switcher->threadPriorities[i].value) {
			ui->threadPriority->setCurrentText(
				switcher->threadPriorities[i].name.c_str());
		}
	}

	if (switcher->th && switcher->th->isRunning())
		SetStarted();
	else
		SetStopped();

	/*Heuristic tab*/
	ui->weightOfRank->setValue(switcher->rankWeight);
	ui->weightOfPending->setValue(switcher->numPendingWeight);
	ui->weightOfTime->setValue(switcher->timeInStreamWeight);
	ui->numberOfCycle->setValue(switcher->cycleSize);


	/*Timers tab*/
	ui->delayJudgments->setValue(switcher->delayJugdment);
	ui->delayIp->setValue(switcher->delayIp);
	ui->checkInterval->setValue(switcher->interval);
}
