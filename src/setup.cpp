#include "headers/setup.h"

 ifstream fin;
 ofstream fout;

void escribirItemScenes(int numItemsScene, string nameSources[]) {
  if (numItemsScene == 0) {
    fout << "                \"items\": []\n";
    return;
  }
  fout << "                \"items\": [\n";
  for (int i = 0; i < numItemsScene; i++) {
    fout << "                    {\n";  
    fout << "                        \"align\": 5,\n";  
    fout << "                        \"bounds\": {\n";  
    fout << "                            \"x\": 0.0,\n";  
    fout << "                            \"y\": 0.0\n";  
    fout << "                        },\n";  
    fout << "                        \"bounds_align\": 0,\n";  
    fout << "                        \"bounds_type\": 0,\n";  
    fout << "                        \"crop_bottom\": 0,\n";  
    fout << "                        \"crop_left\": 0,\n";
    fout << "                        \"crop_right\": 0,\n";
    fout << "                        \"crop_top\": 0,\n";
    fout << "                        \"group_item_backup\": false,\n";  
    fout << "                        \"id\": "<<i+1<<",\n";  //
    fout << "                        \"locked\": false,\n";  
    fout << "                        \"name\": \""<< nameSources[i] << "\",\n";  //
    fout << "                        \"pos\": {\n";  
    fout << "                            \"x\": 0.0,\n";  
    fout << "                            \"y\": 0.0\n";  
    fout << "                        },\n";  
    fout << "                        \"private_settings\": {},\n";  
    fout << "                        \"rot\": 0.0,\n";  
    fout << "                        \"scale\": {\n";  
    fout << "                            \"x\": 1.0,\n";  //
    fout << "                            \"y\": 1.0\n";  //
    fout << "                        },\n";  
    fout << "                        \"scale_filter\": \"disable\",\n";  
    fout << "                        \"visible\": true\n";
    if (i == numItemsScene-1)
      fout << "                    }\n";
    else
      fout << "                    },\n";
  }
  fout << "                ]\n";
}

void escribirEscena(int numSceneItems, string namesSources[],string nameScene) { 
    fout << "        {\n";
    fout << "            \"balance\": 0.5,\n";
    fout << "            \"deinterlace_field_order\": 0,\n";
    fout << "            \"deinterlace_mode\": 0,\n";
    fout << "            \"enabled\": true,\n";
    fout << "            \"flags\": 0,\n";
    fout << "            \"hotkeys\": {\n";
    if (numSceneItems > 0) {
      fout << "                \"OBSBasic.SelectScene\": [],\n";
      for (int i = 0; i < numSceneItems; i++)
        fout << "                \"libobs.hide_scene_item." << namesSources[i] << "\": [],\n";
      for (int i = 0; i < numSceneItems - 1; i++)
        fout << "                \"libobs.show_scene_item." << namesSources[i] << "\": [],\n";
        fout << "                \"libobs.show_scene_item."<< namesSources[numSceneItems - 1] << "\": []\n";
    } 
    else fout << "                \"OBSBasic.SelectScene\": []\n";
    fout << "            },\n";
    fout << "            \"id\": \"scene\",\n";
    fout << "            \"mixers\": 0,\n";
    fout << "            \"monitoring_type\": 0,\n";
    fout << "            \"muted\": false,\n";
    fout << "            \"name\": \"" << nameScene << "\",\n";
    fout << "            \"prev_ver\": 402653187,\n";
    fout << "            \"private_settings\": {},\n";
    fout << "            \"push-to-mute\": false,\n";
    fout << "            \"push-to-mute-delay\": 0,\n";
    fout << "            \"push-to-talk\": false,\n";
    fout << "            \"push-to-talk-delay\": 0,\n";
    fout << "            \"settings\": {\n";
    fout << "                \"custom_size\": false,\n";
    fout << "                \"id_counter\": "<<numSceneItems<<",\n";
    escribirItemScenes(numSceneItems, namesSources);
    fout << "            },\n";
    fout << "            \"sync\": 0,\n";
    fout << "            \"volume\": 1.0\n";
    fout << "        },\n";
}

void escribirFuenteURL(string name, string url,int height,int width, bool ultimo) 
{ 
    fout << "        {\n";
    fout << "            \"balance\": 0.5,\n";
    fout << "            \"deinterlace_field_order\": 0,\n";
    fout << "            \"deinterlace_mode\": 0,\n";
    fout << "            \"enabled\": true,\n";
    fout << "            \"flags\": 0,\n";
    fout << "            \"hotkeys\": {\n";
    fout << "                \"libobs.mute\": [],\n";
    fout << "                \"libobs.push-to-mute\": [],\n";
    fout << "                \"libobs.push-to-talk\": [],\n";
    fout << "                \"libobs.unmute\": []\n";
    fout << "            },\n";
    fout << "            \"id\": \"browser_source\",\n";
    fout << "            \"mixers\": 255,\n";///
    fout << "            \"monitoring_type\": 0,\n";
    fout << "            \"muted\": false,\n";
    fout << "            \"name\": \"" << name <<"\",\n";
    fout << "            \"prev_ver\": 402653187,\n";
    fout << "            \"private_settings\": {},\n";
    fout << "            \"push-to-mute\": false,\n";
    fout << "            \"push-to-mute-delay\": 0,\n";
    fout << "            \"push-to-talk\": false,\n";
    fout << "            \"push-to-talk-delay\": 0,\n";
    fout << "            \"settings\": {\n";
    fout << "                \"height\": "<<height<<",\n";
    fout << "                \"url\": \""<< url <<"\",\n";
    fout << "                \"width\": "<<width<<"\n";
    fout << "            },\n";
    fout << "            \"sync\": 0,\n";
    fout << "            \"volume\": 1.0\n";
    if (ultimo)
      fout << "        }\n";
    else
      fout << "        },\n";
}

void escribeEscenasOrden() {
    fout << "    \"scene_order\": [\n";
    fout << "        {\n"; 
    fout << "            \"name\": \"TeamView\"\n";
    fout << "        },\n";
    fout << "        {\n";
    fout << "            \"name\": \"ProblemResolutionView\"\n";
    fout << "        },\n";
    fout << "        {\n";
    fout << "            \"name\": \"ClassificationView\"\n";
    fout << "        }\n";
    fout << "    ],\n";
    
}

void escribeSources() {
  string listNamesSources[2];
  fout << "    \"sources\": [\n";
  listNamesSources[0] = "camTeam";
  listNamesSources[1] = "screenTeam";
  escribirEscena(2, listNamesSources, "TeamView");
  escribirFuenteURL("screenTeam",
                    "https://player.twitch.tv/"
                    "?channel=snoodyboo&enableExtensions=true&muted=false&"
                    "parent=twitch.tv&player=popfout&volume=0.46000000834465027",
                    fullscreenHeight, fullscreenWidth,false);
  escribirFuenteURL("camTeam",
                    "https://player.twitch.tv/"
                    "?channel=snoodyboo&enableExtensions=true&muted=false&"
                    "parent=twitch.tv&player=popfout&volume=0.46000000834465027",
                    camHeight, camWidth, false);
  escribirEscena(0, listNamesSources, "ProblemResolutionView");
  listNamesSources[0] = "screenClassification";
  escribirEscena(1, listNamesSources, "ClassificationView");
  escribirFuenteURL("screenClassification",
                    "https://player.twitch.tv/"
                    "?channel=snoodyboo&enableExtensions=true&muted=false&"
                    "parent=twitch.tv&player=popfout&volume=0.46000000834465027",
                    fullscreenHeight, fullscreenWidth,true);
  fout << "    ],\n";
}

void saltarLinea() { 
  string temp;
  getline(fin, temp);
}

string continuarHastaToken(string token,bool escribe) {
  string buffer;
  size_t pos = 0;
  while (true) {
    getline(fin, buffer);
    pos = buffer.find(token);
    if (pos != -1) // Sustituible por == 0; suponiendo que siempre se busca al inicio
      return buffer;
    if (escribe)
        fout << buffer << "\n";
  }
}

void escribeFichero (string ContestName, string path, string nameJson) {
  string buffer;
  fin.open(path, ios::in);
  fout.open(nameJson, ios::out);
  getline(fin, buffer);
  fout << "{\n";
  fout << "    \"current_program_scene\": \"TeamView\",\n";
  fout << "    \"current_scene\": \"TeamView\",\n";
  continuarHastaToken("    \"current_scene\":", false);
  continuarHastaToken("    \"name\": ", true);
  fout << "    \"name\": \"" << ContestName << "\",\n";
  continuarHastaToken("    \"scene_order\": [", true);
  escribeEscenasOrden();
  escribeSources();
  buffer = continuarHastaToken("    \"transition_duration\":", false);
  fout << buffer << "\n";
  while (!fin.eof()) {
    getline(fin, buffer);
    fout << buffer << "\n";
  }
  fout.close();
  fin.close();
}

void arreglaNombre(string &nombre)
{
	int size = nombre.size() - 1;
	if (size > 3 && nombre[size - 2] == '(' && nombre[size] == ')') { //Es > 3 pues con 3 caracteres no puede haber elementos con (x), pues dicha cadena ocupa 3 caracteres
		nombre[size - 2] = '_';
		nombre[size] = '\0';
	}
}

void crearConfiguracion() {

	string contestName, path, nameJson;
	contestName = "TFG"; ///Cambiar a que se obtenga del GUI

	char dst[512];
	//GetConfigPath(dst, 512, "obs-studio/basic/scenes/");
	string perfilActual = obs_frontend_get_current_scene_collection();
	arreglaNombre(perfilActual);

	path = dst + '\'' + perfilActual;
	nameJson = dst + '\'' + contestName + ".json";

	escribeFichero(contestName, path, nameJson);
}


