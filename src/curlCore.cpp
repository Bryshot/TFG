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
	getArrayInfo(contest, switcher->curlProblems, typesInfo::Problems);
	getTeamsContestInfo(contest,true);
	
	return contest;
}

void setCurls() {
	switcher->curlContest = switcher->contestServerWebsite +"/api/v4/contests/" + switcher->contestName;
	switcher->curlProblems = switcher->curlContest + "/problems";
	switcher->curlScoreboard = switcher->curlContest + "/scoreboard";
	switcher->curlTeams = switcher->curlContest + "/teams?ids%5B%5D=";
	switcher->curlSubmissions = switcher->curlContest + "/submissions";
	switcher->curlJudgements =switcher->curlContest + "/judgements?submission_id=";
}

void getGeneralContestInfo(contestInfo& contest)
{
	string jsonContestInfo = getRemoteData(switcher->curlContest);
	obs_data_t *dataContestInfo = obs_data_create_from_json(jsonContestInfo.c_str());
	contest.name = obs_data_get_string(dataContestInfo, "name");
	contest.penalty_time = obs_data_get_int(dataContestInfo, "penalty_time");
	contest.start_time = obs_data_get_string(dataContestInfo, "start_time");
	contest.end_time = obs_data_get_string(dataContestInfo, "end_time");
	obs_data_release(dataContestInfo);
}

void getProblemContestInfo(contestInfo& contest, obs_data_t * data)
{	
	problemInfo info;	
	info.id = obs_data_get_string(data, "id");
	info.label = obs_data_get_string(data, "label");
	info.name = obs_data_get_string(data, "name");
	info.timeLimit = obs_data_get_int(data, "time_limit");
	contest.problems.push_back(info);
}

void getTeamsContestInfo(contestInfo& contest, bool startingInfo) {


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
			map<string,teamInfo>::iterator it = contest.scoreBoard.find(id);
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
			problemTeamInfo pInfo;
			obs_data_t *problem =
				obs_data_array_item(problemsTeam, j);

			string problemId = obs_data_get_string(problem, "problem_id");
			if (!startingInfo) {
				map<string, problemTeamInfo>::iterator it2 = info.problems.find(problemId);
				pInfo = it2->second;
			}
			pInfo.num_judged =
				obs_data_get_int(problem, "num_judged");
			int pending = obs_data_get_int(problem, "num_pending");
			pInfo.solved = obs_data_get_bool(problem, "solved");
			if (pending > 0 && !pInfo.solved)
				info.score.num_pending += 1;
			switcher->numPendingSubmissions += pending;
			pInfo.first_to_solve =
				obs_data_get_bool(problem, "first_to_solve");
			pInfo.time = obs_data_get_int(problem, "time");
			obs_data_release(problem);

			if (startingInfo) 
				info.problems.insert(pair(problemId, pInfo));

			obs_data_release(team);	
		}
		obs_data_array_release(problemsTeam);

		if (startingInfo) {
			getIdentificationTeamInfo(id, info);
			contest.scoreBoard.insert(pair<string, teamInfo>(id, info));
		}
		
	}
	obs_data_array_release(arrayScoreboard);
	obs_data_release(dataScoreboardInfo);
	
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
		string jsonJudgementsInfo = getRemoteData(switcher->curlJudgements+it->first);
		string jsonFixedJudgementsInfo = vectorToObj(jsonJudgementsInfo); //Como el servidor devuelve el vector directamente, es necesario una adaptación para usar la función del obs
		obs_data_t *dataJudgementsInfo = obs_data_create_from_json(jsonFixedJudgementsInfo.c_str());
		obs_data_array_t *arrayJudgements = obs_data_get_array(dataJudgementsInfo, "array");

		obs_data_t *judgements = obs_data_array_item(arrayJudgements, 0);
		it->second.result = obs_data_get_string(judgements, "judgement_type_id");
		obs_data_release(judgements);
		switcher->numPendingSubmissions--;
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
		else if (type == typesInfo::Problems)
			getProblemContestInfo(contest, element);

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

