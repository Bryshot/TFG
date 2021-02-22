#include "headers/curlCore.h"

static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
			    void *userp)
{
	((std::string *)userp)->append((char *)contents, size * nmemb);
	return size * nmemb;
}

string getRemoteData(std::string &url) //quiza es string url
{
	std::string readBuffer;

	if (switcher->curl && f_curl_setopt && f_curl_perform) {
		f_curl_setopt(switcher->curl, CURLOPT_URL, url.c_str());
		f_curl_setopt(switcher->curl, CURLOPT_WRITEFUNCTION,
			      WriteCallback);
		f_curl_setopt(switcher->curl, CURLOPT_WRITEDATA, &readBuffer);

		f_curl_perform(switcher->curl);
	}
	return readBuffer;
}


inline string vectorToObj(string vector)
{
	return "{ \"array\": " + vector + "}";
}

contestInfo getContestRealTimeInfo(string contestName)
{
	contestInfo contest;

	/*Configuramos las variables del curl para nuestro torneo*/
	switcher->curlContest = switcher->curlAllContest+ contestName;
	switcher->curlProblems = switcher->curlContest + "/problems";
	switcher->curlScoreboard = switcher->curlContest + "/scoreboard";
	switcher->curlTeams = switcher->curlContest + "/teams?ids%5B%5D=";

	/*Obtenemos la información general del torneo*/
	string jsonContestInfo = getRemoteData(switcher->curlContest);
	obs_data_t* dataContestInfo = obs_data_create_from_json(jsonContestInfo.c_str());

	contest.name = obs_data_get_string(dataContestInfo, "name");
	contest.penalty_time = obs_data_get_int(dataContestInfo, "penalty_time");
	contest.start_time = obs_data_get_string(dataContestInfo, "start_time");
	contest.end_time = obs_data_get_string(dataContestInfo, "end_time");

	/*Obteneos la informacion general de los problemas del torneo*/
	string jsonProblemsInfo = getRemoteData(switcher->curlProblems);
	string jsonFixedProblemInfo = vectorToObj(jsonProblemsInfo);//Como el servidor devuelve el vector directamente, es necesario una adaptación para usar la función del obs
	obs_data_t *dataProblemsInfo = obs_data_create_from_json(jsonFixedProblemInfo.c_str());
	obs_data_array_t *arrayProblems = obs_data_get_array(dataProblemsInfo, "array");

	size_t sizeProblems = obs_data_array_count(arrayProblems);
	/*Iteramos sobre el array para obtener la información necesaria de cada elemento*/
	for (size_t i = 0; i <sizeProblems; i++)
	{
		problemInfo info;
		obs_data_t *problem = obs_data_array_item(arrayProblems, i);
		info.id = obs_data_get_string(problem, "id");
		info.label = obs_data_get_string(problem, "label");
		info.name = obs_data_get_string(problem, "name");
		info.timeLimit = obs_data_get_int(problem, "time_limit");
		contest.problems.push_back(info);
	}

	/*Obtenemos la informacion de los equipos*/
	string jsonScoreboardInfo = getRemoteData(switcher->curlScoreboard);
	obs_data_t *dataScoreboardInfo = obs_data_create_from_json(jsonScoreboardInfo.c_str());
	obs_data_array_t *arrayScoreboard = obs_data_get_array(dataScoreboardInfo, "rows");

	size_t sizeScoreboard = obs_data_array_count(arrayScoreboard);
	/*Iteramos sobre el array para obtener la información necesaria de cada elemento*/
	for (size_t i = 0; i < sizeScoreboard; i++) {

		teamInfo info;
		obs_data_t *team = obs_data_array_item(arrayScoreboard, i);

		/*Obtenemos la información general relacionada con un equipo*/
		info.rank = obs_data_get_int(team, "rank");
		string id = obs_data_get_string(team, "team_id");

		/*Obtenemos la información relativa a la puntuación del equipo*/
		obs_data_t *score = obs_data_get_obj(team, "score");
		info.score.num_solved = obs_data_get_int(score, "num_solved");
		info.score.total_time = obs_data_get_int(score, "total_time");

		obs_data_array_t *problemsTeam = obs_data_get_array(team, "problems");
		size_t sizeProblemsTeam = obs_data_array_count(problemsTeam);

		/*Obtenemos la informacion relativa a los problemas del equipo*/
		for (size_t j = 0; j < sizeProblemsTeam; j++) {
			problemTeamInfo pInfo;
			obs_data_t *problem = obs_data_array_item(problemsTeam,j);

			pInfo.problemId = obs_data_get_string(problem, "problem_id");
			pInfo.num_judged = obs_data_get_int(problem, "num_judged");
			int pending = obs_data_get_int(problem, "num_pending");
			pInfo.solved = obs_data_get_bool(problem, "solved");
			if (pending > 0 && !pInfo.solved) {
				info.score.problem_pending_ids.push_back(
					pInfo.problemId);
				info.score.num_pending += pending;
			}
			pInfo.first_to_solve = obs_data_get_bool(problem, "first_to_solve");
			pInfo.time = obs_data_get_int(problem, "time");

			info.problems.push_back(pInfo);
		}

		string curlTeam = switcher->curlTeams + id;

		string jsonTeamInfo = getRemoteData(curlTeam);
		string jsonFixedTeamInfo = vectorToObj(jsonTeamInfo);
		obs_data_t *dataTeamInfo = obs_data_create_from_json(jsonFixedTeamInfo.c_str());
		obs_data_array_t *arrayTeamInfo = obs_data_get_array(dataTeamInfo, "array");

		obs_data_t *tamInfo = obs_data_array_item(arrayTeamInfo, 0);

		info.identificationInfo.affiliation = obs_data_get_string(tamInfo, "affiliation");
		info.identificationInfo.name = obs_data_get_string(tamInfo, "name");
		info.identificationInfo.nationality = obs_data_get_string(tamInfo, "nationality");
		contest.scoreBoard.insert(pair<string,teamInfo>(id,info));
	}
	return contest;
}
