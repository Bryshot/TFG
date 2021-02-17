#include <obs-module.h>

///La funcionalidad de este fichero es conectar/desconectar el módulo del  plugin al obs.

OBS_DECLARE_MODULE()

void InitSceneSwitcher();
void FreeSceneSwitcher();

/// <summary>
/// Función a la que llama obs para inicializar el módulo 
/// </summary>
/// <returns>Devuelve True siempre que se logra finalizar la inicialización</returns>
bool obs_module_load(void)
{
	InitSceneSwitcher();
	return true;
}

/// <summary>
/// Función a la que llama obs para desconectar el módulo
/// </summary>
void obs_module_unload(void)
{
	FreeSceneSwitcher();
}
