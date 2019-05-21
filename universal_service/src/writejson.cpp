#include <jsoncpp/json/writer.h>
#include <iostream>
#include <fstream>

int main()
{
    std::ofstream file_id;
    file_id.open("my_json_data.json");

    Json::Value data_vec(Json::arrayValue);
    Json::Value data;
    data["image"] = "map.pgm";
    data["resolution"] = "0.010000";
        Json::Value origin_data;
        origin_data["x"] = "-6.005000";
        origin_data["y"] = "-6.005000";
        origin_data["z"] = "0.000000";
    data["origin"] = origin_data;
    data["negate"] = "0";
    data["occupied_thresh"] = "0.65";
    data["free_thresh"] = "0.196";
    data_vec.append(data);

    Json::Value MultiAgentSystemElementsVec(Json::arrayValue);
    Json::Value Elements;
    Elements["typename"] = "map";
    Elements["id"] = 0;
    Elements["data"] = data_vec;
    MultiAgentSystemElementsVec.append(Elements);

    Json::Value MultiAgentSystemElements;
    MultiAgentSystemElements["MultiAgentSystemElements"] = MultiAgentSystemElementsVec;

    Json::StyledWriter styledWriter;
    file_id << styledWriter.write(MultiAgentSystemElements);

    file_id.close();
    return 0;
}
