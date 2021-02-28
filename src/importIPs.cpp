#include "headers\importIPs.h"

ifstream FicheroIPs;

bool isNumber(const string &str)
{
	for (char const &c : str) {
		if (std::isdigit(c) == 0)
			return false;
	}
	return true;
}

IpsContest importUrlContest(string path)
{
  IpsContest contest;
  string buffer;
  FicheroIPs.open(path, ios::in);
  getline(FicheroIPs, buffer);
  if (!isNumber(buffer))
  {
	  contest.numTeams = -1;
	  return contest;
  }
  contest.numTeams = std::stoi(buffer); 
  getline(FicheroIPs, buffer);
  contest.ipClassification = buffer;
  for (int i = 0; i < contest.numTeams; i++) {
    IpsTeam team;
    string name;
    asignaString(name);
    asignaString(team.ipScreen);
    asignaString(team.ipCam);
    asignaString(team.urlLogo);
    contest.ipsTeams.insert(pair(name, team));
  }
  FicheroIPs.close();
  return contest;
}

void asignaString(string & s)
{
	string buffer;
	getline(FicheroIPs, buffer);
	s = buffer;
}


