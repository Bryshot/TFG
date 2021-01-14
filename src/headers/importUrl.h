#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <cctype>

using namespace std;

const int MAX_NUM_TEAMS = 100;

typedef struct{
  string nombreEquipo;
  string urlCamara;
  string urlScreen;
  string urlLogo;
} UrlsTeam;

typedef struct  {
 UrlsTeam urlsTeams[MAX_NUM_TEAMS];
 int numTeams;
} UrlsContest;

void asignaNombreEquipo(UrlsTeam teams[], int i);
void asignaCamaraEquipo(UrlsTeam teams[], int i);
void asignaPantallaEquipo(UrlsTeam teams[], int i);
void asignaLogoEquipo(UrlsTeam teams[], int i);

UrlsContest importUrlContest(string path);
