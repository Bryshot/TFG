#pragma once

#include <QDialog>
#include <memory>
#include <vector>
#include <string>
#ifdef BUILD_OUT_OF_TREE
#include "../../forms/ui_advanced-scene-switcher.h"
#else
#include "ui_advanced-scene-switcher.h"
#endif
#include "switcher-data-structs.hpp"
#include "setup.h"

class QCloseEvent;

/*******************************************************************************
 * Advanced Scene Switcher window
 *******************************************************************************/
class SceneSwitcher : public QDialog {
	Q_OBJECT

public:
	std::unique_ptr<Ui_SceneSwitcher> ui;
	bool loading = true;
	SceneSwitcher(QWidget *parent);

	void closeEvent(QCloseEvent *event) override;

	void SetStarted();
	void SetStopped();

	int FindByData(const QString &window);//Revisar
	
	void loadUI();
	void setupGeneralTab();
	void setTabOrder();

public slots:

	void on_checkInterval_valueChanged(int value);
	void on_SpeedRotation_valueChanged(int value);
	void on_threadPriority_currentIndexChanged(int index);
	void on_delayJudgments_valueChanged(int value);
	void on_delayIp_valueChanged(int value);
	void on_weightOfRank_valueChanged(double value);
	void on_weightOfTime_valueChanged(double value);
	void on_weightOfPending_valueChanged(double value);
	void on_numberOfCycle_valueChanged(int value);

	void on_toggleStartButton_clicked();
	void on_tabMoved(int from, int to);


	void on_verboseLogging_stateChanged(int state);

	void on_exportSettings_clicked();
	void on_importSettings_clicked();

	void on_importIPs_clicked();

	void on_createSetup_clicked();

	void on_contestName_textChanged(const QString &text);
	void on_contestServer_textChanged(const QString &text);
	void on_userContestServer_textChanged(const QString &text);
	void on_passwordContestServer_textChanged(const QString &text);

	void on_close_clicked();

private:
	
};

/********************************************************************************
 * Sceneswitch helper
 ********************************************************************************/

void switchScene(obs_source_t *transition, std::unique_lock<std::mutex> &lock);

void modificaTextTeam(obs_source_t *source, string text);

void insertSpaces(string &tmp, int numSpaces);

void updateTextRotative(obs_source_t * source);

/********************************************************************************
 * Main SwitcherData
 ********************************************************************************/
struct SwitcherData;
extern SwitcherData *switcher;
