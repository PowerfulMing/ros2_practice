#include "PathPlanningClient.h"

void PathPlanningClient::create_request(void)
{
        Json::Value data_vec(Json::arrayValue);
        Json::Value data;
        data["typename"] = "AMR";
        data["from"] = to_id_;
        data["to"] = from_id_;
        data_vec.append(data);

        Json::Value MultiAgentSystemElementsVec(Json::arrayValue);
        Json::Value Elements;
        Elements["typename"] = "PathPlanningService";
        Elements["id"] = 0;
        Elements["data"] = data_vec;
        MultiAgentSystemElementsVec.append(Elements);

        Json::Value MultiAgentSystemElements;
        MultiAgentSystemElements["MultiAgentSystemElements"] = MultiAgentSystemElementsVec;
        Json::FastWriter fastWriter;
        mas_request_ = fastWriter.write(MultiAgentSystemElements);

        // Write file for test
//        std::ofstream file_id;
//        file_id.open("my_json_data.json");
//        Json::StyledWriter styledWriter;
//        file_id << styledWriter.write(MultiAgentSystemElements);
//        file_id.close();
}
void PathPlanningClient::handle_response()
{
	Client::handle_response();
	if(phrase_response())
	{

	}else{

	}
}
bool PathPlanningClient::phrase_response()
{
	Json::Value response_root;
    Json::Reader reader;
    std::cout << "has response: " << std::endl;
    std::cout << mas_response_ << std::endl;
    if (!reader.parse(mas_response_, response_root))
    {
        return false;
    }

    Json::Value root_info = response_root["MultiAgentSystemElements"][0];
    if (root_info["typename"].compare("PathPlanningService") != 0)
    {
        // root_info[i] is not "graph" (typename is not "graph")
        return false;
    }
    Json::Value data_vec(Json::arrayValue);
    data_vec = root_info["data"];
    std::cout << "======== Path ========" << std::endl;
    for (Json::Value::ArrayIndex i = 0; i != data_vec.size(); i++)
    {
        std::cout << data_vec[i]["id"] << " -> ";
    }
    std::cout << std::endl;
}
