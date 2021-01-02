#pragma once
#include <string>
#include "utility.hpp"
#include "switch-generic.hpp"

/// Quitar
constexpr auto round_trip_func = 0;
constexpr auto default_priority_0 = round_trip_func;

/// <summary>
///  Estructura que define los SceneSequenceSwitch
/// </summary>
struct SceneSequenceSwitch : SceneSwitcherEntry {
	//Escena con la que se empieza el trip
	OBSWeakSource startScene;
	//Tiempo en cada parada
	double delay;
	//String que define al switch
	std::string sceneSequenceStr;

	/// <summary>
	/// Funcion encargada de obtener el tipo de SceneSwitcherEntry
	/// </summary>
	/// <returns>sequence</returns>
	const char *getType() { return "sequence"; }

	/// <summary>
	/// Función encargada de comprobar si el SceneSequenceSwitch es valido
	/// </summary>
	/// <returns>True si es válido, false en otro caso</returns>
	bool valid()
	{
		return WeakSourceValid(startScene) &&
		       (usePreviousScene || WeakSourceValid(scene)) &&
		       WeakSourceValid(transition);
	}

	/// <summary>
	/// Log de cuando se detiene el trip
	/// </summary>
	/// <param name="duracion"> del sleep </param>
	void logSleep(int dur)
	{
		blog(LOG_INFO, "Advanced Scene Switcher sequence sleep %d",
		     dur);
	}

	/// <summary>
	/// Constructora del SceneSequenceSwitch
	/// </summary>
	/// <param name="startScene_"> Escena con la que se empieza el trip</param>
	/// <param name="scene_">con la que acaba la sequence</param>
	/// <param name="transition_">que se debe utilizar</param>
	/// <param name="delay_"> entre cada cambio</param>
	/// <param name="usePreviousScene_"> si se utiliza la ultima escena</param>
	/// <param name="string"> que define el switch</param>
	inline SceneSequenceSwitch(OBSWeakSource startScene_,
				    OBSWeakSource scene_,
				    OBSWeakSource transition_, double delay_,
				    bool usePreviousScene_, std::string str)
		: SceneSwitcherEntry(scene_, transition_, usePreviousScene_),
		  startScene(startScene_),
		  delay(delay_),
		  sceneSequenceStr(str)
	{
	}
};

//Función encargada de crear el log crea define a un SceneSequenceSwitch
static inline QString MakeSceneSequenceSwitchName(const QString &scene1,
						   const QString &scene2,
						   const QString &transition,
						   double delay);
