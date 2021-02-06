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

	int FindByData(const QString &window);
	int SceneTransitionsFindByData(const QString &scene1,
				       const QString &scene2);
	int DefaultTransitionsFindByData(const QString &scene);

	void UpdateNonMatchingScene(const QString &name);
	void UpdateAutoStopScene(const QString &name);
	void UpdateAutoStartScene(const QString &name);
	
	void loadUI();
	void populateSceneSelection(QComboBox *sel, bool addPrevious);
	void populateTransitionSelection(QComboBox *sel);
	void setupGeneralTab();
	void setupTransitionsTab();
	void setTabOrder();

public slots:
	void on_noMatchDontSwitch_clicked();
	void on_noMatchSwitch_clicked();
	void on_noMatchRandomSwitch_clicked();
	void on_startupBehavior_currentIndexChanged(int index);
	void on_noMatchSwitchScene_currentTextChanged(const QString &text);
	void on_checkInterval_valueChanged(int value);
	void on_toggleStartButton_clicked();
	void on_tabMoved(int from, int to);

	void on_autoStopSceneCheckBox_stateChanged(int state);
	void on_autoStopScenes_currentTextChanged(const QString &text);

	void on_autoStartSceneCheckBox_stateChanged(int state);
	void on_autoStartType_currentIndexChanged(int index);
	void on_autoStartScenes_currentTextChanged(const QString &text);

	void on_verboseLogging_stateChanged(int state);

	void on_exportSettings_clicked();
	void on_importSettings_clicked();

	void on_importUrls_clicked();

	void on_createSetup_clicked();

	void on_contestName_textChanged(const QString &text);

	void on_sceneTransitions_currentRowChanged(int idx);
	void on_transitionsAdd_clicked();
	void on_transitionsRemove_clicked();
	void on_defaultTransitions_currentRowChanged(int idx);
	void on_defaultTransitionsAdd_clicked();
	void on_defaultTransitionsRemove_clicked();
	void on_transitionOverridecheckBox_stateChanged(int state);

	void on_close_clicked();

private:
};

/********************************************************************************
 * Windowtitle helper
 ********************************************************************************/
void GetWindowList(std::vector<std::string> &windows);
void GetWindowList(QStringList &windows); // Overloaded
void GetCurrentWindowTitle(std::string &title);
bool isFullscreen(std::string &title);
bool isMaximized(std::string &title);

/********************************************************************************
 * Screenregion helper
 ********************************************************************************/
std::pair<int, int> getCursorPos();

/********************************************************************************
 * Idle detection helper
 ********************************************************************************/
int secondsSinceLastInput();

/********************************************************************************
 * Executable helper
 ********************************************************************************/
void GetProcessList(QStringList &processes);
bool isInFocus(const QString &executable);

/********************************************************************************
 * Sceneswitch helper
 ********************************************************************************/
struct obs_weak_source;
typedef struct obs_weak_source obs_weak_source_t;

typedef struct transitionData {
	std::string name = "";
	int duration = 0;
} transitionData;

void setNextTransition(OBSWeakSource &targetScene, obs_source_t *currentSource,
		       OBSWeakSource &transition,
		       bool &transitionOverrideOverride, transitionData &td);
void overwriteTransitionOverride(obs_weak_source_t *sceneWs,
				 obs_source_t *transition, transitionData &td);
void restoreTransitionOverride(obs_source_t *scene, transitionData td);

void switchScene(OBSWeakSource &scene, OBSWeakSource &transition,
		 bool &transitionOverrideOverride,
		 std::unique_lock<std::mutex> &lock);

/********************************************************************************
 * Main SwitcherData
 ********************************************************************************/
struct SwitcherData;
extern SwitcherData *switcher;
