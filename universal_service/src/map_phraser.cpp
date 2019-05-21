#include "universal_service/universal_service.hpp"
#include "yaml-cpp/yaml.h"
#include <iostream>
#include <vector>
#include <string>
#include <jsoncpp/json/writer.h>
#include <fstream>

class MapPhraserServer:public UniversalService
{
public:
  MapPhraserServer(std::string node_name, std::string service_name): UniversalService(node_name, service_name)
  {
    node_name_ = node_name;
    service_name_ = service_name;

  }
  ~MapPhraserServer(){}
  
  virtual bool do_something(std::string str_req, std::string *str_rep)
  {
      read_yaml(str_req);
      *str_rep = write_json();

      return true;
  }
private:
  void read_yaml(std::string str_req)
  {
    node = YAML::LoadFile(str_req);
    std::cout << "Load map: " << str_req << std::endl;

    image_path = node["image"].as<std::string>();
    resolution = node["resolution"].as<std::string>();
    origin = node["origin"].as<std::vector<double>>();
    negate = node["negate"].as<std::string>();
    occupied_thresh = node["occupied_thresh"].as<std::string>();
    free_thresh = node["free_thresh"].as<std::string>();
  }
  std::string write_json()
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

    Json::FastWriter fastWriter;
    std::string output = fastWriter.write(MultiAgentSystemElements);

    Json::StyledWriter styledWriter;
    file_id << styledWriter.write(MultiAgentSystemElements);
    
    file_id.close();
    std::cout << output << std::endl;
    return output;
  }
  std::string node_name_;
  std::string service_name_;

  YAML::Node node;
  std::string image_path;
  std::string resolution;
  std::vector<double> origin;
  std::string negate;
  std::string occupied_thresh;
  std::string free_thresh;

  std::string json_string;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  // UniversalService my_service("my_server","my_service");
  MapPhraserServer map_phraser("map_phraser_server","phrase_map_service");
  rclcpp::Rate loop_rate(10);
  while(rclcpp::ok())
  {
    std::cout << "-------------------" << std::endl;
    loop_rate.sleep();
    map_phraser.call_spin_some();
  }
  rclcpp::shutdown();
  return 0;
}
