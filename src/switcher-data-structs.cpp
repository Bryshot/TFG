#include "headers/switcher-data-structs.hpp"
#include "headers/utility.hpp"

/// <summary>
/// Función encargada de reiniciar a cero todas las estructuras del plugin que pudieran contener algun elemento no valido.
/// </summary>
void SwitcherData::Prune()
{

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
