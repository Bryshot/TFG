#include "headers/setup.h"

#include <obs-frontend-api.h>

void crearConfiguracion(string contestName, SwitcherData *switcher)
{
	//Declaración de variables
	string path, nameJson;
	char dst[512];
	struct vec2 pos = vec2();
	vec2_set(&pos, 0.0, fullscreenHeight - 48); //Cambiar por size

	//Ruta absoluta
	os_get_config_path(dst, 512, "obs-studio/basic/scenes/");

	//Nombre unico de fichero y de colección
	string name = makeUniqueName(contestName);
	string fileName = makeUniqueFileName(name, dst);
	obs_frontend_add_scene_collection(name.c_str());

	//Cambio de nombre a la escena creada por default
	struct obs_frontend_source_list scenes = {};
	obs_frontend_get_scenes(&scenes);

	for (size_t i = 0; i < scenes.sources.num; i++) {
		obs_source_t *source = scenes.sources.array[i];
		string temp = obs_source_get_name(source);
		if (temp == "Escena") {
			obs_source_set_name(source, "ProblemResolutionView");
			break;
		}
	}

	obs_frontend_source_list_free(&scenes);

	//Creacion de las escenas y fuentes necesarias para la configuración
	obs_scene *teamViewer = obs_scene_create("TeamView");
	obs_scene *classificationView = obs_scene_create("ClassificationView");

	obs_data_t *settingsCam = obs_data_create();
	make_source_settings(settingsCam, switcher->urlCam, camHeight, camWidth);
	switcher->camTeam = obs_source_create("browser_source","camTeam",settingsCam,NULL);

	obs_data_t *settingsScreen = obs_data_create();
	make_source_settings(settingsScreen, switcher->urlScreen, fullscreenHeight, fullscreenWidth);
	switcher->screenTeam = obs_source_create("browser_source", "screenTeam",settingsScreen, NULL);

	obs_data_t *settingsCamDummy = obs_data_create();
	make_source_settings(settingsCamDummy, "", camHeight, camWidth);
	switcher->camTeamDummy = obs_source_create("browser_source", "camTeamDummy", settingsCamDummy, NULL);

	obs_data_t *settingsScreenDummy = obs_data_create();
	make_source_settings(settingsScreenDummy, "", fullscreenHeight,fullscreenWidth);
	switcher->screenTeamDummy = obs_source_create("browser_source", "screenTeamDummy", settingsScreenDummy, NULL);

	obs_data_t *settingsClassification = obs_data_create();
	make_source_settings(settingsClassification, switcher->urlClassification, fullscreenHeight, fullscreenWidth);
	switcher->screenClassification = obs_source_create("browser_source", "screenClassification", settingsClassification, NULL);

	obs_data_t *settingsText = obs_data_create();
	make_text_settings(settingsText, "Hola soy un texto rotativo", 48,"bottom");//Sustituir por texto y tamaño por parametros;
	switcher->textRotative = obs_source_create("text_gdiplus", "textRotative", settingsText,NULL);

	obs_data_t *settingsFilter = obs_data_create();
	make_filter_settings(settingsFilter, 100);//Sustituir por parametro
	switcher->filter = obs_source_create("scroll_filter", "filter", settingsFilter, NULL);

	obs_source_filter_add(switcher->textRotative, switcher->filter);

	//Adicción de las escenas y fuentes a la configuración

	switcher->screenTeamDummyItem = obs_scene_add(teamViewer, switcher->screenTeamDummy);
	switcher->camTeamDummyItem = obs_scene_add(teamViewer, switcher->camTeamDummy);
	switcher->screenTeamItem = obs_scene_add(teamViewer, switcher->screenTeam);
	switcher->camTeamItem = obs_scene_add(teamViewer, switcher->camTeam);
	obs_sceneitem_t *item = obs_scene_add(teamViewer, switcher->textRotative);
	obs_sceneitem_set_pos(item, &pos);

	obs_scene_add(classificationView, switcher->screenClassification);
	item = obs_scene_add(classificationView, switcher->textRotative);
	obs_sceneitem_set_pos(item, &pos);

	//Parametros adicionales de la configuración
	obs_source_set_muted(switcher->screenClassification, true);
	obs_sceneitem_set_visible(switcher->screenTeamDummyItem, false);
	obs_sceneitem_set_visible(switcher->camTeamDummyItem, false);

	//Establecimiento de escena inicial
	obs_frontend_set_current_preview_scene(obs_scene_get_source(teamViewer));
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

string makeUniqueFileName(string name,string path)
{
	string file_name = makeFileName(name);
	filesystem::path file_path = std::filesystem::path(path)
			    .append(file_name)
			    .concat(".json");

	if (std::filesystem::exists(file_path)) {
		std::stringstream sstr;

		// Name already exists, make it unique.
		for (size_t idx = 1; true; idx++) {
			sstr.str(std::string());
			sstr << idx << ".json";
			file_path = std::filesystem::path(path)
					    .append(file_name)
					    .concat(sstr.str());
			if (!std::filesystem::exists(file_path)) {
				break;
			}
		}
	}
	return file_path.string();
}

string makeFileName(string name)
{
	size_t base_len = name.length();
	size_t len = os_utf8_to_wcs(name.data(), base_len, nullptr, 0);
	std::wstring wfile;

	if (!len)
		return name;

	wfile.resize(len);
	os_utf8_to_wcs(name.data(), base_len, &wfile[0], len + 1);

	for (size_t i = wfile.size(); i > 0; i--) {
		size_t im1 = i - 1;

		if (iswspace(wfile[im1])) {
			wfile[im1] = '_';
		} else if (wfile[im1] != '_' && !iswalnum(wfile[im1])) {
			wfile.erase(im1, 1);
		}
	}

	if (wfile.size() == 0)
		wfile = L"characters_only";

	len = os_wcs_to_utf8(wfile.c_str(), wfile.size(), nullptr, 0);
	if (!len)
		return name;

	name.resize(len);
	os_wcs_to_utf8(wfile.c_str(), wfile.size(), name.data(), len + 1);

	return name;
}

void make_source_settings(obs_data_t* data ,string url, int height, int width) {
	obs_data_set_int(data, "height", height);
	obs_data_set_string(data, "url", url.c_str());
	obs_data_set_int(data, "width", width);
	obs_data_set_bool(data, "reroute_audio", true);
}


void make_text_settings(obs_data_t* data, string text, int size,string valign)
{
	obs_data_set_int(data, "bk_opacity",100);
	obs_data_set_bool(data, "extents", true);
	obs_data_set_int(data, "extents_cx", fullscreenWidth);
	obs_data_set_int(data, "extents_cy",size);

	obs_data_t *font = obs_data_create();
	obs_data_set_string(font, "face", fuente.c_str()); 
	obs_data_set_int(font, "flags", 0);		
	obs_data_set_int(font, "size", size);
	obs_data_set_string(font, "style", "Normal"); 

	obs_data_set_obj(data, "font", font);
	obs_data_set_string(data, "text", text.c_str());
	obs_data_set_string(data, "valign", valign.c_str());
}


void make_filter_settings(obs_data_t* data, int rotationSpeed) {

	obs_data_set_int(data, "speed_x", rotationSpeed);
}
