#pragma once
#include <string>
#include "utility.hpp"
#include "switch-generic.hpp"
/*
/// <summary>
/// Estructura que representa los SceneTransitions
/// </summary>
struct SceneTransition : SceneSwitcherEntry {

	//Escena de la que se debe acabar de reproducir la transición
	OBSWeakSource scene2;
	//String que representa la transición
	std::string sceneTransitionStr;

	/// <summary>
	/// Funcion encargada de obtener el tipo de SceneSwitcherEntry
	/// </summary>
	/// <returns>Transition</returns>
	const char *getType() { return "transition"; }

	/// <summary>
	/// Comprueba si un switch es válido o no
	/// </summary>
	/// <returns>True si el switch es válido, false en otro caso</returns>
	bool valid()
	{
		return (WeakSourceValid(scene) && WeakSourceValid(scene2)) &&
		       WeakSourceValid(transition);
	}

	/// <summary>
	/// Constructora de SceneTransition
	/// </summary>
	/// <param name="scene_"> en la que se empieza la transición</param>
	/// <param name="scene2_"> en la que se acaba la transición</param>
	/// <param name="transition_"> que se debe emplear</param>
	/// <param name="sceneTransitionStr_">, string que representa la transición</param>
	inline SceneTransition(OBSWeakSource scene_, OBSWeakSource scene2_,
			       OBSWeakSource transition_,
			       std::string sceneTransitionStr_)
		: SceneSwitcherEntry(scene_, transition_),
		  scene2(scene2_),
		  sceneTransitionStr(sceneTransitionStr_)
	{
	}
};

/// <summary>
/// Función encargada de crear el log correspondiente a la transición
/// </summary>
/// <param name="scene1"> de la que se empieza la transición</param>
/// <param name="scene2"> en la que se debe acabar la transición</param>
/// <param name="transition">, string que define la transición</param>
/// <returns></returns>
static inline QString MakeSceneTransitionName(const QString &scene1,
					      const QString &scene2,
					      const QString &transition);

/// <summary>
/// Estructura que representa los DefaultSceneTransition
/// </summary>
struct DefaultSceneTransition : SceneSwitcherEntry {
	//String que representa la transición
	std::string sceneTransitionStr;

	/// <summary>
	/// Funcion encargada de obtener el tipo de SceneSwitcherEntry
	/// </summary>
	/// <returns>def_transition</returns>
	const char *getType() { return "def_transition"; }

	/// <summary>
	/// Constructora de DefaultSceneTransition
	/// </summary>
	/// <param name="scene_">,con la que empieza la transición</param>
	/// <param name="transition_">, string que define la transición</param>
	/// <param name="sceneTransitionStr_"></param>
	inline DefaultSceneTransition(OBSWeakSource scene_,
				      OBSWeakSource transition_,
				      std::string sceneTransitionStr_)
		: SceneSwitcherEntry(scene_, transition_),
		  sceneTransitionStr(sceneTransitionStr_)
	{
	}
};

/// <summary>
/// Función encargada de crear el log correspondiente a la transición
/// </summary>
/// <param name="scene"> de la que se empieza la transición</param>
/// <param name="transition">, string que define la transición</param>
static inline QString MakeDefaultSceneTransitionName(const QString &scene,
						     const QString &transition);
						     */
