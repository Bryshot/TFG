#include "headers\importUrl.h"

ifstream FicheroUrls;

bool isNumber(const string &str)
{
	for (char const &c : str) {
		if (std::isdigit(c) == 0)
			return false;
	}
	return true;
}

UrlsContest importUrlContest(string path)
{
  UrlsContest contest;
  string buffer;
  FicheroUrls.open(path, ios::in);
  getline(FicheroUrls, buffer);
  if (!isNumber(buffer))
  {
	  contest.numTeams = -1;
	  return contest;
  }
  contest.numTeams = std::stoi(buffer); 
  getline(FicheroUrls, buffer);
  contest.urlClassification = buffer;
  for (int i = 0; i < contest.numTeams; i++) {
    asignaNombreEquipo(contest.urlsTeams, i);
    asignaCamaraEquipo(contest.urlsTeams, i);
    asignaPantallaEquipo(contest.urlsTeams, i);
    asignaLogoEquipo(contest.urlsTeams, i);
  }
  FicheroUrls.close();
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

