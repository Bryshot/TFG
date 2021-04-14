#pragma once

#include <QDialog>
#include <memory>
#include <vector>
#include <string>
#ifdef BUILD_OUT_OF_TREE
#include "../../forms/ui_autoProducer.h"
#else
#include "ui_autoProducer.h"
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
	SceneSwitcher(QWidget *parent);

	//Función encargada de la lógica de cerrar el plugin
	void closeEvent(QCloseEvent *event) override;

	//Función que modifica el gui para reflejar que se ha iniciado el programa
	void SetStarted();

	//Función encargada de modificar el gui para reflejar que se ha detenido el programa
	void SetStopped();

	//Función encargada de establecer el gui
	void loadUI();

	//Función encargada de establecer los valores y contenidos del gui
	void setupGeneralTab();

	//Función encargada de establecer el orden de las tabs del gui
	void setTabOrder();

public slots:

	//Función encargada de la lógica de modificar la variable interval por medio del gui
	void on_checkInterval_valueChanged(int value);

	//Función encargada de la lógica de modificar la variable speedRotation por medio del gui
	void on_SpeedRotation_valueChanged(int value);

	//Función encargada de la lógica de modificar la variable threadPriority por medio del gui
	void on_threadPriority_currentIndexChanged(int index);

	//Función encargada de la lógica de modificar la variable delayJudgments por medio del gui
	void on_delayJudgments_valueChanged(int value);

	//Función encargada de la lógica de modificar la variable preCreated por medio del gui
	void on_preCreatedCheckBox_stateChanged(int state);

	//Función encargada de la lógica de modificar la variable followVisible por medio del gui
	void on_followVisible_stateChanged(int state);

	//Función encargada de la lógica de modificar la variable delayIp por medio del gui
	void on_delayIp_valueChanged(int value);

	//Función encargada de la lógica de modificar la variable rankWeight por medio del gui
	void on_weightOfRank_valueChanged(double value);

	//Función encargada de la lógica de modificar la variable timeInStreamWeight por medio del gui
	void on_weightOfTime_valueChanged(double value);

	//Función encargada de la lógica de modificar la variable numPendingWeight por medio del gui
	void on_weightOfPending_valueChanged(double value);

	//Función encargada de la lógica de modificar la variable numPendingWeight por medio del gui
	void on_numberOfCycle_valueChanged(int value);

	//Función encargada de la lógica de iniciar el programa por medio del gui
	void on_toggleStartButton_clicked();

	//Función encargada de registrar un cambio de orden en las tabs del gui
	void on_tabMoved(int from, int to);

	//Función encargada de registrar un cambio en la checkbox de verbose
	void on_verboseLogging_stateChanged(int state);

	//Función encargada de la lógica de exportar la configuración del plugin
	void on_exportSettings_clicked();

	//Función encargada de la lógica de importar la configuración del plugin
	void on_importSettings_clicked();

	//Función encargada de la lógica de importar la información respectiva a las IPs del torneo
	void on_importIPs_clicked();

	//Función encargada de la lógica de crear una nueva configuración de escenas por medio del gui
	void on_createSetup_clicked();

	//Función encargada de la lógica de modificar la variable constestName por medio del gui
	void on_contestName_textChanged(const QString &text);

	//Función encargada de la lógica de modificar la variable contestServer por medio del gui
	void on_contestServer_textChanged(const QString &text);

	//Función encargada de la lógica de modificar la variable userContestServer por medio del gui
	void on_userContestServer_textChanged(const QString &text);

	//Función encargada de la lógica de modificar la variable passwordContestServer por medio del gui
	void on_passwordContestServer_textChanged(const QString &text);

	//Función encargada de la lógica de cerrar el programa por medio del boton del gui
	void on_close_clicked();

private:
};

/********************************************************************************
 * Sceneswitch helper
 ********************************************************************************/

//Función encargada de modificar textTeamImage
void modificaImage();

///Función encargada de modificar obs_source_t de tipo texto, insertando text como nuevo texto
void modificaText(obs_source_t *source, string text);

//Función encargada de insertar numSpaces espacios vacios en tmp
void insertSpaces(string &tmp, int numSpaces);

//Función encargada de replicar los cambios realizados en la vlc sources visibles en las vlc sources no visibles
void modificaPos(obs_sceneitem_t *main, obs_sceneitem_t *dummy,
		 bool usingDummy);

//Función encargada de comprobar que la escena actual es teamViewer o classificationView
bool checkCurrentScene();

//Función encargada de eliminar los i elementos más antiguos del vector LastTeamInStream
void popLastTeamInStream(int i);

//Función encargada de obtener las fuentes y escenas necesarias para el funcionamiento del plugin, cuando la scene collection actual esta pre-creada.
void get_sources();

//Función encargada de poner a false la variable created cuando se cambia de scene collection
void set_created_false(SwitcherData *switcher);

/********************************************************************************
 * Main SwitcherData
 ********************************************************************************/
struct SwitcherData;
extern SwitcherData *switcher;
