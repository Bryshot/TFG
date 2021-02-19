#include <obs-module.h>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>

#include "headers/advanced-scene-switcher.hpp"
#include "headers/importUrl.h"

void SceneSwitcher::on_close_clicked()
{
	done(0);
}

void SceneSwitcher::on_checkInterval_valueChanged(int value)
{
	if (loading)
		return;

	std::lock_guard<std::mutex> lock(switcher->m);
	switcher->interval = value;
}

void updateSizeRotativeText(SwitcherData *switcher)
{
	obs_data_t *data = obs_source_get_settings(switcher->textRotative);
	obs_data_t *font = obs_data_get_obj(data, "font");
	obs_data_set_int(font, "size", switcher->sizeRotativeText);
	obs_source_update(switcher->textRotative, data);
}

void SceneSwitcher::on_sizeRotativeText_valueChanged(int value)
{
	if (loading)
		return;

	std::lock_guard<std::mutex> lock(switcher->m);
	switcher->sizeRotativeText = value;
	//Actualizar el tamaño
	if (switcher->created)
		updateSizeRotativeText(switcher);
}

void updateSpeedRotativeText(SwitcherData *switcher)
{
	obs_data_t *data = obs_data_create();
	obs_data_set_int(data, "speed_x", switcher->speedRotativeText);
	obs_source_update(switcher->filter, data);
}

void SceneSwitcher::on_SpeedRotation_valueChanged(int value)
{
	if (loading)
		return;

	std::lock_guard<std::mutex> lock(switcher->m);
	switcher->speedRotativeText = value;
	//Actualizar la velocidad
	if (switcher->created)
		updateSpeedRotativeText(switcher);
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
	} else if(switcher->importedUrls){
		switcher->Start();
		SetStarted();
	}
	else{
		QMessageBox Msgbox;
		Msgbox.setText("Import the url before start the plugin");
		Msgbox.exec();
		return;
	}
}

void SceneSwitcher::closeEvent(QCloseEvent *)
{
	obs_frontend_save();
}

void SceneSwitcher::on_verboseLogging_stateChanged(int state)
{
	if (loading)
		return;

	std::lock_guard<std::mutex> lock(switcher->m);
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

	switcher->saveSceneTransitions(obj);
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

	switcher->loadSceneTransitions(obj);
	switcher->loadGeneralSettings(obj);
	switcher->loadHotkeys(obj);

	obs_data_release(obj);

	QMessageBox Msgbox;
	Msgbox.setText(
		"AutoProducer settings imported successfully");
	Msgbox.exec();
	close();
}

void SceneSwitcher::on_importUrls_clicked() {

	QString directory = QFileDialog::getOpenFileName(
		this, tr("Import AutoProducer settings from file ..."),
		QDir::currentPath(), tr("Text files (*.txt)"));
	if (directory.isEmpty())
		return;
	
	switcher->urlsContest =
			importUrlContest(directory.toStdString());
	if (switcher->urlsContest.numTeams == -1)
	{
		QMessageBox Msgbox;
		Msgbox.setText("AutoProducer failed to import urls");
		Msgbox.exec();
		return;
	}
	QMessageBox Msgbox;
	Msgbox.setText("AutoProducer urls imported successfully");
	Msgbox.exec();

	switcher->urlScreen = switcher->urlsContest.urlsTeams[0].urlScreen;
	switcher->urlCam = switcher->urlsContest.urlsTeams[0].urlCamara;
	switcher->urlClassification = switcher->urlsContest.urlClassification;

	switcher->importedUrls = true;

	obs_data_t *dataScreen = obs_source_get_settings(switcher->screenTeam);
	obs_data_t *dataCam = obs_source_get_settings(switcher->camTeam);
	obs_data_t *dataClassification = obs_source_get_settings(switcher->screenClassification);

	obs_data_set_string(dataScreen, "url", switcher->urlScreen.c_str());
	obs_data_set_string(dataCam, "url", switcher->urlCam.c_str());
	obs_data_set_string(dataClassification, "url",switcher->urlClassification.c_str());

	obs_source_update(switcher->screenTeam, dataScreen);
	obs_source_update(switcher->camTeam, dataCam);
	obs_source_update(switcher->screenClassification, dataCam);
	close();
}

void SceneSwitcher::on_createSetup_clicked() {
	crearConfiguracion(switcher);
}

void SceneSwitcher::on_contestName_textChanged(const QString &text) {
	switcher->contestName = text.toStdString();
}


void updateRotativeTextContent(SwitcherData* switcher) {
	obs_data_t *data = obs_source_get_settings(switcher->textRotative);
	obs_data_set_string(data, "text", switcher->textRotativeContent.c_str());
	obs_source_update(switcher->textRotative, data);
	
}

void SceneSwitcher::on_rotativeText_textChanged(const QString &text) {
	switcher->textRotativeContent = text.toStdString();
	//Update texto
	if (switcher->created)
		updateRotativeTextContent(switcher);
}


int findTabIndex(QTabBar *bar, int pos)
{
	int at = -1;

	QString tabName = "";
	switch (pos) {
	case 0:
		tabName = "General";
		break;
	case 1:
		tabName = "Transition";
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
	if (loading)
		return;
	std::swap(switcher->tabOrder[from], switcher->tabOrder[to]);
}

void SwitcherData::saveGeneralSettings(obs_data_t *obj)
{
	obs_data_set_int(obj, "interval", switcher->interval);

	obs_data_set_bool(obj, "active", !switcher->stop);

	obs_data_set_bool(obj, "verbose", switcher->verbose);

	obs_data_set_int(obj, "threadPriority", switcher->threadPriority);

	obs_data_set_int(obj, "generalTabPos", switcher->tabOrder[0]);
	obs_data_set_int(obj, "transitionTabPos", switcher->tabOrder[1]);
}

void SwitcherData::loadGeneralSettings(obs_data_t *obj)
{
	obs_data_set_default_int(obj, "interval", default_interval);
	switcher->interval = obs_data_get_int(obj, "interval");

	switcher->stop = !obs_data_get_bool(obj, "active");
	
	switcher->verbose = obs_data_get_bool(obj, "verbose");

	obs_data_set_default_int(obj, "threadPriority",
				 QThread::NormalPriority);
	switcher->threadPriority = obs_data_get_int(obj, "threadPriority");

	obs_data_set_default_int(obj, "generalTabPos", 0);
	obs_data_set_default_int(obj, "transitionTabPos", 1);

	switcher->tabOrder.emplace_back(
		(int)(obs_data_get_int(obj, "generalTabPos")));
	switcher->tabOrder.emplace_back(
		(int)(obs_data_get_int(obj, "transitionTabPos")));
}

void SceneSwitcher::setupGeneralTab()
{
	ui->verboseLogging->setChecked(switcher->verbose);

	ui->contestName->setText(switcher->contestName.c_str());
	ui->sizeRotativeText->setValue(switcher->sizeRotativeText);
	ui->rotativeText->setText(switcher->textRotativeContent.c_str());
	ui->SpeedRotation->setValue(switcher->speedRotativeText);

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
}
