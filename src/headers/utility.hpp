#pragma once
#include <obs.hpp>
#include <QString>
#include <obs-frontend-api.h>
//Archivo con funciones de utilidad destinadas a la obtencion de weakTransition y weakSource, y comprobar que estas ultimas son validas.

/// <summary>
/// Comprueba si valida una weakSource( Si se puede obtener su source)
/// </summary>
/// <param name="Weaksource"> que se quiere comprobar</param>
/// <returns>True si es valida, false en otro caso</returns>
static inline bool WeakSourceValid(obs_weak_source_t *ws)
{
	obs_source_t *source = obs_weak_source_get_source(ws);
	if (source)
		obs_source_release(source);
	return !!source;
}

/// <summary>
/// Recupera el nombre de la weakSource (que coincide con el de su Source)
/// </summary>
/// <param name="Weak_source">, de la que se desea obtener el nombre</param>
/// <returns>Nombre de la escena</returns>
static inline std::string GetWeakSourceName(obs_weak_source_t *weak_source)
{
	std::string name;

	obs_source_t *source = obs_weak_source_get_source(weak_source);
	if (source) {
		name = obs_source_get_name(source);
		obs_source_release(source);
	}

	return name;
}

/// <summary>
/// Obtiene la weakSource que coincide con el nombre dado
/// </summary>
/// <param name="Name"> de la weakSource que se busca</param>
/// <returns>La weakSource deseada</returns>
static inline OBSWeakSource GetWeakSourceByName(const char *name)
{
	OBSWeakSource weak;
	obs_source_t *source = obs_get_source_by_name(name);
	if (source) {
		weak = obs_source_get_weak_source(source);
		obs_weak_source_release(weak);
		obs_source_release(source);
	}

	return weak;
}

/// <summary>
/// Obtiene la weakSource que coincide con el nombre (en formato QString)
/// </summary>
/// <param name="Nombre"> de la weakSource en formato QString</param>
/// <returns>La weakSource deseada</returns>
static inline OBSWeakSource GetWeakSourceByQString(const QString &name)
{
	return GetWeakSourceByName(name.toUtf8().constData());
}

/// <summary>
/// Obtiene la weakTransition que coincide con el nombre
/// </summary>
/// <param name="Nombre de la transicion"> deseada</param>
/// <returns>La weakTransition deseada</returns>
static inline OBSWeakSource GetWeakTransitionByName(const char *transitionName)
{
	OBSWeakSource weak;
	obs_source_t *source = nullptr;

	if (strcmp(transitionName, "Default") == 0) {
		source = obs_frontend_get_current_transition();
		weak = obs_source_get_weak_source(source);
		obs_source_release(source);
		obs_weak_source_release(weak);
		return weak;
	}

	obs_frontend_source_list *transitions = new obs_frontend_source_list();
	obs_frontend_get_transitions(transitions);
	bool match = false;

	for (size_t i = 0; i < transitions->sources.num; i++) {
		const char *name =
			obs_source_get_name(transitions->sources.array[i]);
		if (strcmp(transitionName, name) == 0) {
			match = true;
			source = transitions->sources.array[i];
			break;
		}
	}

	if (match) {
		weak = obs_source_get_weak_source(source);
		obs_weak_source_release(weak);
	}
	obs_frontend_source_list_free(transitions);

	return weak;
}

/// <summary>
/// Obtiene la weakTransition que coincide con el nombre (en formato QString)
/// </summary>
/// <param name="Nombre"> de la weakTransition</param>
/// <returns>La weakTransition deseada</returns>
static inline OBSWeakSource GetWeakTransitionByQString(const QString &name)
{
	return GetWeakTransitionByName(name.toUtf8().constData());
}
