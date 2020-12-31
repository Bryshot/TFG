#include "headers/switcher-data-structs.hpp"
#include "headers/utility.hpp"

void SwitcherData::Prune()
{
	

	for (size_t i = 0; i < sceneSequenceSwitches.size(); i++) {
		SceneSequenceSwitch &s = sceneSequenceSwitches[i];
		if (!s.valid())
			sceneSequenceSwitches.erase(
				sceneSequenceSwitches.begin() + i--);
	}

	if (!WeakSourceValid(autoStopScene)) {
		autoStopScene = nullptr;
		autoStopEnable = false;
	}

	for (size_t i = 0; i < sceneTransitions.size(); i++) {
		SceneTransition &s = sceneTransitions[i];
		if (!s.valid())
			sceneTransitions.erase(sceneTransitions.begin() + i--);
	}

	for (size_t i = 0; i < defaultSceneTransitions.size(); i++) {
		DefaultSceneTransition &s = defaultSceneTransitions[i];
		if (!s.valid())
			defaultSceneTransitions.erase(
				defaultSceneTransitions.begin() + i--);
	}
}
