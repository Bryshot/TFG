#include "headers\importUrl.h"

ifstream FicheroUrls;

UrlsContest importUrlContest(string path)
{
  UrlsContest contest;
  string buffer;
  FicheroUrls.open(path, ios::in);
  getline(FicheroUrls, buffer);
  if (!isdigit(std::stoi(buffer)))
  {
	  contest.numTeams = -1;
	  return contest;
  }
  contest.numTeams = std::stoi(buffer); // Obtener del archivo
  for (int i = 0; i < contest.numTeams; i++) {
    asignaNombreEquipo(contest.urlsTeams, i);
    asignaCamaraEquipo(contest.urlsTeams, i);
    asignaPantallaEquipo(contest.urlsTeams, i);
    asignaLogoEquipo(contest.urlsTeams, i);
  }
  return contest;
}

void asignaNombreEquipo(UrlsTeam teams[], int i) {
    string buffer;
    getline(FicheroUrls, buffer);
    teams[i].nombreEquipo = buffer;
}

void asignaCamaraEquipo(UrlsTeam teams[], int i) {
  string buffer;
  getline(FicheroUrls, buffer);
  teams[i].urlCamara = buffer;
}

void asignaPantallaEquipo(UrlsTeam teams[], int i) {
  string buffer;
  getline(FicheroUrls, buffer);
  teams[i].urlScreen = buffer;
}

void asignaLogoEquipo(UrlsTeam teams[], int i) {
  string buffer;
  getline(FicheroUrls, buffer);
  teams[i].urlLogo = buffer;
}

