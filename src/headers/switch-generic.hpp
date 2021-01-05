#pragma once
#include <string>
#include <obs.hpp>

/// <summary>
/// Estructura que representa de manera genérica los switches del programa
/// </summary>
struct SceneSwitcherEntry {

	/// Escena a la que se realiza el switch
	OBSWeakSource scene;
	///Transición que se utiliza en el switch
	OBSWeakSource transition;
	/// Si se debe utilizar la escena anterior como scene
	bool usePreviousScene;

	/// <summary>
	/// Comprueba si un switch es válido o no
	/// </summary>
	/// <returns>True si el switch es válido, false en otro caso</returns>
	virtual bool valid()
	{
		return (usePreviousScene || WeakSourceValid(scene)) &&
		       WeakSourceValid(transition);
	}

        /// Devuelve el tipo del switch
	virtual const char *getType() = 0;

	/// <summary>
	/// Textualiza el resultado de un switch por consola
	/// </summary>
	virtual void logMatch()
	{
		obs_source_t *s = obs_weak_source_get_source(scene);
		const char *sceneName = obs_source_get_name(s);
		obs_source_release(s);
		blog(LOG_INFO,
		     "AutoProducer match for '%s' - switch to scene '%s'",
		     getType(), sceneName);
	}

	/// <summary>
	/// Constructora básica de SceneSwitcherEntry
	/// </summary>
	inline SceneSwitcherEntry() : usePreviousScene(false) {}

	/// <summary>
	/// Constructora 2 de SceneSwitcherEntry
	/// </summary>
	/// <param name="scene_">, a la que cambiar</param>
	/// <param name="transition_">, transición que se debe emplear</param>
	/// <param name="usePreviousScene_">, si se debe usar la anterior escena</param>
	inline SceneSwitcherEntry(OBSWeakSource scene_,
				  OBSWeakSource transition_,
				  bool usePreviousScene_)
		: scene(scene_),
		  transition(transition_),
		  usePreviousScene(usePreviousScene_)
	{
	}

	/// <summary>
	/// Constructora 3 de SceneSwitcherEntry
	/// </summary>
	/// <param name="scene_">, a la que cambiar</param>
	/// <param name="transition_">, transición que se debe emplear</param>
	inline SceneSwitcherEntry(OBSWeakSource scene_,
				  OBSWeakSource transition_)
		: scene(scene_),
		  transition(transition_),
		  usePreviousScene(false)
	{
	}
};
