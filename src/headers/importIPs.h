#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <cctype>
#include <map>
using namespace std;

typedef struct{
  string ipCam; //Dirección ip y puerto donde se encuentra la camara
  string ipScreen; //Dirección ip y puerto donde se encuentra la pantalla
  string urlLogo; //Dirección url donde esta el logo del equipo
} IpsTeam;

typedef struct  {
 map<string,IpsTeam> ipsTeams;
 int numTeams;
 string ipClassification;
} IpsContest;


void asignaString(string & s);

IpsContest importUrlContest(string path);
