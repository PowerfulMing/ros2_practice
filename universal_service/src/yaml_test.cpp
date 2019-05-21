#include <string>
#include <iostream>
#include "yaml-cpp/yaml.h"
#include <vector>

using namespace std;

int main() {
  YAML::Node node = YAML::LoadFile("map.yaml");
  string image_path = node["image"].as<string>();
  string resolution = node["resolution"].as<string>();
  std::vector<double> origin = node["origin"].as<std::vector<double>>();
  string negate = node["negate"].as<string>();
  string occupied_thresh = node["occupied_thresh"].as<string>();
  string free_thresh = node["free_thresh"].as<string>();

  cout << image_path << endl;
  cout << resolution << endl;
  cout << origin[0] << endl;
  cout << negate << endl;
  cout << occupied_thresh << endl;
  cout << free_thresh << endl;
  
  return 0;
}