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
    UrlsTeam team;
    string name;
    asignaString(name);
    asignaString(team.ipScreen);
    asignaString(team.ipCam);
    asignaString(team.urlLogo);
    contest.urlsTeams.insert(pair(name, team));
  }
  FicheroUrls.close();
  return contest;
}

void asignaString(string & s)
{
	string buffer;
	getline(FicheroUrls, buffer);
	s = buffer;
}


