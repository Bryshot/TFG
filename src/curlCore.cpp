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

contestInfo getContestRealTimeInfo()
{
	contestInfo contest;

	setCurls();
	getGeneralContestInfo(contest);
	getTeamsContestInfo(contest,true);
	
	return contest;
}

void updateContestRealTimeInfo(contestInfo& data)
{
	getTeamsContestInfo(data, false);
	getArrayInfo(data, switcher->curlSubmissions,typesInfo::Submissions);
	switcher->updatedSubmissions = true;
}

void setCurls() {
	switcher->curlContest = switcher->contestServerWebsite +"/api/v4/contests/" + switcher->contestName;
	switcher->curlScoreboard = switcher->curlContest + "/scoreboard";
	switcher->curlTeams = switcher->curlContest + "/teams?ids%5B%5D=";
	switcher->curlSubmissions = switcher->curlContest + "/submissions";
	switcher->curlJudgements = switcher->curlContest + "/judgements?submission_id=";
}

void getGeneralContestInfo(contestInfo& contest)
{
	string jsonContestInfo = getRemoteData(switcher->curlContest);
	obs_data_t *dataContestInfo = obs_data_create_from_json(jsonContestInfo.c_str());
	contest.name = obs_data_get_string(dataContestInfo, "name");
	contest.start_time = obs_data_get_string(dataContestInfo, "start_time");
	contest.end_time = obs_data_get_string(dataContestInfo, "end_time");
	obs_data_release(dataContestInfo);
}

void getTeamsContestInfo(contestInfo &contest, bool startingInfo)
{

	int bestHeuristic = 0;
	teamInfo best;
	string bestId;

	string jsonScoreboardInfo = getRemoteData(switcher->curlScoreboard);
	obs_data_t *dataScoreboardInfo = obs_data_create_from_json(jsonScoreboardInfo.c_str());
	obs_data_array_t *arrayScoreboard = obs_data_get_array(dataScoreboardInfo, "rows");

	size_t sizeScoreboard = obs_data_array_count(arrayScoreboard);
	/*Iteramos sobre el array para obtener la información necesaria de cada elemento*/
	for (size_t i = 0; i < sizeScoreboard; i++) {

		teamInfo info;
		obs_data_t *team = obs_data_array_item(arrayScoreboard, i);

		/*Obtenemos la información general relacionada con un equipo*/
		string id = obs_data_get_string(team, "team_id");
		if (!startingInfo) {
			map<string, teamInfo>::iterator it = contest.scoreBoard.find(id);
			info = it->second;
		}
		info.rank = obs_data_get_int(team, "rank");

		/*Obtenemos la información relativa a la puntuación del equipo*/
		obs_data_t *score = obs_data_get_obj(team, "score");
		info.score.num_solved = obs_data_get_int(score, "num_solved");
		info.score.total_time = obs_data_get_int(score, "total_time");
		obs_data_release(score);

		obs_data_array_t *problemsTeam = obs_data_get_array(team, "problems");
		size_t sizeProblemsTeam = obs_data_array_count(problemsTeam);

		/*Obtenemos la informacion relativa a los problemas del equipo*/
		for (size_t j = 0; j < sizeProblemsTeam; j++) {
			obs_data_t *problem = obs_data_array_item(problemsTeam, j);
			int pending = obs_data_get_int(problem, "num_pending");
			bool solved = obs_data_get_bool(problem, "solved");
			if (pending > 0 && !solved)
				info.score.num_pending += 1;
			obs_data_release(problem);
		}

		obs_data_release(team);
		obs_data_array_release(problemsTeam);

		if (startingInfo) {
			getIdentificationTeamInfo(id, info);
			contest.scoreBoard.insert( pair<string, teamInfo>(id, info));
		}
		else
		{
			int temp = (switcher->rankWeight/ info.rank) + (switcher->numPendingWeight * (info.score.num_pending / info.rank)) - (switcher->timeInStreamWeight * (info.timeInStream * info.rank));
			if (temp > bestHeuristic) {
				bestHeuristic = temp;
				best = info;
				bestId = id;
			}
		}
	}
	obs_data_array_release(arrayScoreboard);
	obs_data_release(dataScoreboardInfo);

	if (!startingInfo) {
		heuristic(bestHeuristic, best, bestId, contest);
	}
	
}

void getSubmissionsInfo(contestInfo &contest, obs_data_t *data)
{
	submissionInfo info;
	info.idTeam = obs_data_get_string(data, "team_id");
	info.idProblem = obs_data_get_string(data, "problem_id");
	string id = obs_data_get_string(data, "id");
	contest.submissionPendings.insert(pair(id,info));
	switcher->numSubmissionsPrevious++;
}

void getJudgementsInfo(contestInfo& contest) {

	map<string,submissionInfo>::iterator it = contest.submissionPendings.begin();

	while (it != contest.submissionPendings.end()) {
		string tmp = switcher->curlJudgements + it->first; /*NO SE PORQUE COÑO NO OBTIENE RESULTADO*/
		string jsonJudgementsInfo = getRemoteData(tmp);
		string jsonFixedJudgementsInfo = vectorToObj(jsonJudgementsInfo); //Como el servidor devuelve el vector directamente, es necesario una adaptación para usar la función del obs
		obs_data_t *dataJudgementsInfo = obs_data_create_from_json(jsonFixedJudgementsInfo.c_str());
		obs_data_array_t *arrayJudgements = obs_data_get_array(dataJudgementsInfo, "array");

		obs_data_t *judgements = obs_data_array_item(arrayJudgements, 0);
		it->second.result = obs_data_get_string(judgements, "judgement_type_id");
		obs_data_release(judgements);
		obs_data_array_release(arrayJudgements);
		obs_data_release(dataJudgementsInfo);
		it++;
	}
}

void getArrayInfo(contestInfo &contest, string curl, typesInfo type)
{
	string jsonInfo = getRemoteData(curl);
	string jsonFixedInfo = vectorToObj(jsonInfo); //Como el servidor devuelve el vector directamente, es necesario una adaptación para usar la función del obs
	obs_data_t *dataInfo = obs_data_create_from_json(jsonFixedInfo.c_str());
	obs_data_array_t *array = obs_data_get_array(dataInfo, "array");

	size_t size = obs_data_array_count(array);
	for (size_t i = switcher->numSubmissionsPrevious; i < size; i++) {
		obs_data_t *element = obs_data_array_item(array, i);

		if (type == typesInfo::Submissions)
			getSubmissionsInfo(contest, element);

		obs_data_release(element);
	}
	obs_data_array_release(array);
	obs_data_release(dataInfo);
}

void getIdentificationTeamInfo(string id, teamInfo& info) {
	string curlTeam = switcher->curlTeams + id;

	string jsonTeamInfo = getRemoteData(curlTeam);
	string jsonFixedTeamInfo = vectorToObj(jsonTeamInfo);
	obs_data_t *dataTeamInfo = obs_data_create_from_json(jsonFixedTeamInfo.c_str());
	obs_data_array_t *arrayTeamInfo = obs_data_get_array(dataTeamInfo, "array");

	obs_data_t *tamInfo = obs_data_array_item(arrayTeamInfo, 0);

	info.identificationInfo.affiliation = obs_data_get_string(tamInfo, "affiliation");
	info.identificationInfo.name = obs_data_get_string(tamInfo, "name");
	info.identificationInfo.nationality = obs_data_get_string(tamInfo, "nationality");

	obs_data_release(dataTeamInfo);
	obs_data_array_release(arrayTeamInfo);
	obs_data_release(tamInfo);
}


void heuristic(int bestHeuristic, teamInfo best, string bestId, contestInfo & contest)
{
	/*Si el mejor no es el actual, se realizan los cambios*/
	if (bestId.c_str() != switcher->lastTeamsInStream.cend()->c_str())
	{

		if (bestHeuristic >= (switcher->rankWeight - (switcher->classificationTimeInStream * switcher->timeInStreamWeight)))
		{
			//Busqueda del elemento en la base de datos estática
			map<string, IpsTeam>::iterator it1 = switcher->ipsContestData.ipsTeams.find(best.identificationInfo.name); //IpsContest y la información obtenida por curl se conectan gracias a que el equipo tiene el MISMO NOMBRE

			//Actualización de las variables para la emisión
			switcher->ipCam = it1->second.ipCam;
			switcher->ipScreen = it1->second.ipScreen;
			switcher->textTeamImageFile = it1->second.urlLogo;
			switcher->backUpText = "Team: " + best.identificationInfo.name + "/nClassification: " + std::to_string(best.rank) + "/nNumber of problem solved: " + std::to_string(best.score.num_solved);

			/*Actualización de las variables de control*/
			switcher->swapIp = true;
			if (switcher->lastTeamsInStream.cend()->c_str() == switcher->tokenIdClassification ) {
				switcher->swapScene = true;
			}

			/*Actualización de las variables para la heurística*/
			switcher->lastTeamsInStream.push_back(bestId);
			best.timeInStream++;
		}
		else
		{
			switcher->classificationTimeInStream++;
			switcher->lastTeamsInStream.push_back(switcher->tokenIdClassification);
			if (switcher->lastTeamsInStream.cend()->c_str() != switcher->tokenIdClassification) 
				switcher->swapScene = true;
		}

		//Actualización de las variables para la heurística
		if (switcher->lastTeamsInStream.size() == switcher->cycleSize+1) //El mas uno es para tener en cuenta que durante esta función se inserta un nuevo elemento
		{
			if (switcher->lastTeamsInStream.begin()->c_str() == switcher->tokenIdClassification) //Tratamiento especial de la clasificación
				switcher->classificationTimeInStream--;
			else //Caso general
				contest.scoreBoard.find(switcher->lastTeamsInStream.begin()->c_str())->second.timeInStream--;
			switcher->lastTeamsInStream.erase(switcher->lastTeamsInStream.begin());
		}
		
	}	
}
