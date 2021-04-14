#include "headers/setup.h"

void crearConfiguracion(SwitcherData *switcher)
{
	//Declaración de variables
	obs_sceneitem_t *item;
	struct vec2 pos = vec2();

	vec2_set(&pos, 0.0, fullscreenHeight - switcher->sizeRotativeText);

	//Nombre unico de fichero y de colección
	string name = makeUniqueName(switcher->contestName);
	obs_frontend_add_scene_collection(name.c_str());

	//Cambio de nombre a la escena creada por default
	struct obs_frontend_source_list scenes = {};
	obs_frontend_get_scenes(&scenes);
	obs_source_set_name(scenes.sources.array[0], "ProblemResolutionView");
	obs_frontend_source_list_free(&scenes);

	//Creacion de las escenas y fuentes necesarias para la configuración
	switcher->teamViewerScene = obs_scene_create("TeamView");
	switcher->classificationScene = obs_scene_create("ClassificationView");

	//Camara principal
	obs_data_t *settingsCam = obs_data_create();
	make_source_settings(settingsCam, switcher->ipCam);
	switcher->camTeam =
		obs_source_create("vlc_source", "camTeam", settingsCam, NULL);
	obs_data_release(settingsCam);

	//Escritorio principal
	obs_data_t *settingsScreen = obs_data_create();
	make_source_settings(settingsScreen, switcher->ipScreen);
	switcher->screenTeam = obs_source_create("vlc_source", "screenTeam",
						 settingsScreen, NULL);
	obs_data_release(settingsScreen);

	//Camara secundaria
	obs_data_t *settingsCamDummy = obs_data_create();
	make_source_settings(settingsCamDummy, "");
	switcher->camTeamDummy = obs_source_create("vlc_source", "camTeamDummy",
						   settingsCamDummy, NULL);
	obs_data_release(settingsCamDummy);

	//Escritorio secundario
	obs_data_t *settingsScreenDummy = obs_data_create();
	make_source_settings(settingsScreenDummy, "");
	switcher->screenTeamDummy = obs_source_create(
		"vlc_source", "screenTeamDummy", settingsScreenDummy, NULL);
	obs_data_release(settingsScreenDummy);

	//Clasificación
	obs_data_t *settingsClassification = obs_data_create();
	make_source_settings(settingsClassification,
			     switcher->urlClassification,
			     fullscreenHeight - switcher->sizeRotativeText,
			     fullscreenWidth);
	switcher->screenClassification =
		obs_source_create("browser_source", "screenClassification",
				  settingsClassification, NULL);
	obs_data_release(settingsClassification);

	//Texto rotativo
	obs_data_t *settingsText = obs_data_create();
	make_text_settings(settingsText, switcher->textRotativeContent,
			   switcher->sizeRotativeText, 3500,
			   switcher->sizeRotativeText, 100, 0, "bottom");
	switcher->textRotative = obs_source_create(
		"text_gdiplus", "textRotative", settingsText, NULL);
	obs_data_release(settingsText);

	//Texto estático
	obs_data_t *settingsStaticText = obs_data_create();
	make_text_settings(settingsStaticText, switcher->textStaticContent,
			   switcher->textStaticHeight,
			   switcher->textStaticWidth, 28, 0, 4, "top");
	switcher->staticText = obs_source_create("text_gdiplus", "textStatic",
						 settingsStaticText, NULL);
	obs_data_release(settingsStaticText);

	//Cola de envios
	obs_data_t *settingsTextSubmission = obs_data_create();
	make_text_settings(settingsTextSubmission,
			   switcher->textSubmissionContent,
			   switcher->textSubmissionHeight,
			   switcher->textSubmissionWidth, 28, 0, 0, "top");
	switcher->textSubmission = obs_source_create(
		"text_gdiplus", "textSubmission", settingsTextSubmission, NULL);
	obs_data_release(settingsTextSubmission);

	//Texto información de los equipos
	obs_data_t *settingsTextTeam = obs_data_create();
	make_text_settings(settingsTextTeam, switcher->textTeamContent,
			   switcher->textTeamHeight, switcher->textTeamWidth,
			   36, 50, 0, "top");
	switcher->textTeam = obs_source_create("text_gdiplus", "textTeam",
					       settingsTextTeam, NULL);
	obs_data_release(settingsTextTeam);

	//Icono de los equipos
	obs_data_t *settingsImageTeam = obs_data_create();
	make_image_settings(settingsImageTeam, switcher->textTeamImageFile);
	switcher->textTeamImage = obs_source_create("image_source", "imageTeam",
						    settingsImageTeam, NULL);
	obs_data_release(settingsImageTeam);

	//Filtro rotativo
	obs_data_t *settingsFilter = obs_data_create();
	make_filter_settings(settingsFilter, switcher->speedRotativeText);
	switcher->filter = obs_source_create("scroll_filter", "filter",
					     settingsFilter, NULL);
	obs_source_filter_add(switcher->textRotative, switcher->filter);
	obs_data_release(settingsFilter);

	//Adicción de las escenas y fuentes a la configuración
	switcher->screenTeamDummyItem = obs_scene_add(
		switcher->teamViewerScene, switcher->screenTeamDummy);
	switcher->camTeamDummyItem = obs_scene_add(switcher->teamViewerScene,
						   switcher->camTeamDummy);
	switcher->screenTeamItem =
		obs_scene_add(switcher->teamViewerScene, switcher->screenTeam);
	switcher->camTeamItem =
		obs_scene_add(switcher->teamViewerScene, switcher->camTeam);

	item = obs_scene_add(switcher->teamViewerScene, switcher->textRotative);
	obs_sceneitem_set_pos(item, &pos);

	obs_scene_add(switcher->classificationScene,
		      switcher->screenClassification);
	item = obs_scene_add(switcher->classificationScene,
			     switcher->textRotative);
	obs_sceneitem_set_pos(item, &pos);

	vec2_set(&pos, 0.0, 331.0);
	item = obs_scene_add(switcher->teamViewerScene, switcher->staticText);
	obs_sceneitem_set_pos(item, &pos);

	vec2_set(&pos, 0.0, 365.0);
	item = obs_scene_add(switcher->teamViewerScene,
			     switcher->textSubmission);
	obs_sceneitem_set_pos(item, &pos);

	vec2_set(&pos, 659, 878);
	item = obs_scene_add(switcher->teamViewerScene, switcher->textTeam);
	obs_sceneitem_set_pos(item, &pos);

	vec2_set(&pos, 1432.4312744140625, 879);
	item = obs_scene_add(switcher->teamViewerScene,
			     switcher->textTeamImage);
	obs_sceneitem_set_pos(item, &pos);

	//Parametros adicionales de la configuración
	obs_source_set_muted(switcher->screenClassification, true);
	obs_sceneitem_set_visible(switcher->screenTeamDummyItem, false);
	obs_sceneitem_set_visible(switcher->camTeamDummyItem, false);

	//Establecimiento de escena inicial
	obs_frontend_set_current_preview_scene(
		obs_scene_get_source(switcher->classificationScene));
	obs_frontend_set_current_scene(
		obs_scene_get_source(switcher->teamViewerScene));

	//Marcamos el final de la creación
	switcher->created = true;
}

string makeUniqueName(string name)
{
	std::set<std::string> names;
	char **names_raw = obs_frontend_get_scene_collections();

	for (char **ptr = names_raw; *ptr != nullptr; ptr++) {
		names.emplace(*ptr);
	}

	bfree(names_raw);

	if (names.find(name) != names.end()) {
		std::stringstream sstr;

		// Name already exists, make it unique.
		for (size_t idx = 1; true; idx++) {
			sstr.str(std::string());
			sstr << name;
			sstr << " (" << idx << ")";
			std::string test = sstr.str();
			if (names.find(test) == names.end())
				return test;
		}
	}
	return name;
}

void make_source_settings(obs_data_t *data, string ip)
{

	obs_data_set_string(data, "playback_behavior", "always_play");
	obs_data_array_t *array = obs_data_array_create();
	obs_data_t *obj = obs_data_create();
	obs_data_set_bool(obj, "hidden", false);
	obs_data_set_bool(obj, "selected", true);
	obs_data_set_string(obj, "value", ip.c_str());
	obs_data_array_push_back(array, obj);
	obs_data_set_array(data, "playlist", array);
	obs_data_release(obj);
	obs_data_array_release(array);
}

void make_source_settings(obs_data_t *data, string url, int height, int width)
{
	obs_data_set_string(
		data, "css",
		"body { background-color: rgba(255, 255, 255, 1); margin: 0px auto; overflow: hidden; }");
	obs_data_set_int(data, "height", height);
	obs_data_set_string(data, "url", url.c_str());
	obs_data_set_int(data, "width", width);
	obs_data_set_bool(data, "reroute_audio", true);
}

void make_text_settings(obs_data_t *data, string text, int height, int width,
			int sizeFont, int opacity, int flags, string valign)
{
	obs_data_set_int(data, "bk_opacity", opacity);
	obs_data_set_bool(data, "extents", true);
	obs_data_set_int(data, "extents_cx", width);
	obs_data_set_int(data, "extents_cy", height);

	obs_data_t *font = obs_data_create();
	obs_data_set_string(font, "face", fuente.c_str());
	obs_data_set_int(font, "flags", flags);
	obs_data_set_int(font, "size", sizeFont);
	obs_data_set_string(font, "style", "Normal");

	obs_data_set_obj(data, "font", font);
	obs_data_set_string(data, "text", text.c_str());
	obs_data_set_string(data, "valign", valign.c_str());

	obs_data_release(font);
}

void make_filter_settings(obs_data_t *data, int rotationSpeed)
{

	obs_data_set_int(data, "speed_x", rotationSpeed);
}

void make_image_settings(obs_data_t *data, string file)
{

	obs_data_set_string(data, "file", file.c_str());
}
