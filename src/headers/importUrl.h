#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <cctype>
#include <map>
using namespace std;

typedef struct{
  string ipCam;
  string ipScreen;
  string urlLogo;
} UrlsTeam;

typedef struct  {
 map<string,UrlsTeam> urlsTeams;///Cambiar por vector
 int numTeams;
 string urlClassification;
} UrlsContest;


void asignaString(string & s);

UrlsContest importUrlContest(string path);
