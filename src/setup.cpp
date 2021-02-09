#include "headers/setup.h"

#include <obs-frontend-api.h>

void crearConfiguracion(string contestName, SwitcherData *switcher)
{
	string path, nameJson;
	char dst[512];

	os_get_config_path(dst, 512, "obs-studio/basic/scenes/");

	string name = makeUniqueName(contestName);
	string fileName = makeUniqueFileName(name, dst);
	obs_frontend_add_scene_collection(name.c_str());

	obs_data_t *contest = obs_data_create_from_json_file(fileName.c_str());
	name = makeUniqueName(contestName);
	fileName = makeUniqueFileName(name, dst);
	obs_frontend_set_current_scene_collection(name.c_str());

	struct obs_frontend_source_list scenes = {};
	obs_frontend_get_scenes(&scenes);

	for (size_t i = 0; i < scenes.sources.num; i++) {
		obs_source_t *source = scenes.sources.array[i];
		string temp = obs_source_get_name(source);
		if (temp == "Escena") {
			obs_source_set_name(source, "ProblemResolutionView");
		}
	}

	obs_frontend_source_list_free(&scenes);

	obs_scene *teamViewer = obs_scene_create("TeamView");
	obs_scene *classificationView = obs_scene_create("ClassificationView");

	obs_data_t *settingsCam = obs_data_create();
	make_source_settings(settingsCam, switcher->urlCam, camHeight, camWidth);
	switcher->camTeam = obs_source_create("browser_source","camTeam",settingsCam,NULL);

	obs_data_t *settingsScreen = obs_data_create();
	make_source_settings(settingsScreen, switcher->urlScreen, fullscreenHeight, fullscreenWidth);
	switcher->screenTeam = obs_source_create("browser_source", "screenTeam",settingsScreen, NULL);

	obs_data_t *settingsClassification = obs_data_create();
	make_source_settings(settingsClassification, switcher->urlClassification, fullscreenHeight, fullscreenWidth);
	switcher->screenClassification = obs_source_create("browser_source", "screenClassification", settingsClassification, NULL);

	obs_scene_add(teamViewer, switcher->screenTeam);
	obs_scene_add(teamViewer, switcher->camTeam);
	
	obs_scene_add(classificationView, switcher->screenClassification);
	
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

	/*obs_data_t *setting = obs_data_create();

	obs_data_set_int(setting, "height", height);
	obs_data_set_string(setting, "url", url.c_str());
	obs_data_set_int(setting, "width", width);*/

	obs_data_set_int(data, "height", height);
	obs_data_set_string(data, "url", url.c_str());
	obs_data_set_int(data, "width", width);

	//obs_data_set_obj(data, "settings", setting);
}
