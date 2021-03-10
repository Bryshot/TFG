#include "headers/advanced-scene-switcher.hpp"

void SceneSwitcher::setupTransitionsTab()
{
	/*populateSceneSelection(ui->transitionsScene1, false);
	populateSceneSelection(ui->transitionsScene2, false);
	populateSceneSelection(ui->defaultTransitionsScene, false);
	populateTransitionSelection(ui->transitionsTransitions);
	populateTransitionSelection(ui->defaultTransitionsTransitions);

	for (auto &s : switcher->sceneTransitions) {
		std::string sceneName1 = GetWeakSourceName(s.scene);
		std::string sceneName2 = GetWeakSourceName(s.scene2);
		std::string transitionName = GetWeakSourceName(s.transition);
		QString text = MakeSceneTransitionName(sceneName1.c_str(),
						       sceneName2.c_str(),
						       transitionName.c_str());

		QListWidgetItem *item =
			new QListWidgetItem(text, ui->sceneTransitions);
		item->setData(Qt::UserRole, text);
	}

	for (auto &s : switcher->defaultSceneTransitions) {
		std::string sceneName = GetWeakSourceName(s.scene);
		std::string transitionName = GetWeakSourceName(s.transition);
		QString text = MakeDefaultSceneTransitionName(
			sceneName.c_str(), transitionName.c_str());

		QListWidgetItem *item =
			new QListWidgetItem(text, ui->defaultTransitions);
		item->setData(Qt::UserRole, text);
	}

	ui->transitionOverridecheckBox->setChecked(
		switcher->tansitionOverrideOverride);*/
}
