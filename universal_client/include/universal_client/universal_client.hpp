#ifndef UNIVERSAL_CLIENT_H
#define UNIVERSAL_CLIENT_H
#include "rclcpp/rclcpp.hpp"
#include "universal_interface/srv/string_service.hpp"
#include <chrono>

using StringService = universal_interface::srv::StringService;
using namespace std::placeholders;

class UniversalClient
{
public:
  UniversalClient(std::string node_name, std::string service_name);
  ~UniversalClient()
  {
    rclcpp::shutdown();
  }
  bool send_request(std::string request_string);

private:
  std::string node_name_;
  std::string service_name_;
  rclcpp::Node::SharedPtr node_;
  // rclcpp::Service<StringService>::SharedPtr client_;
  rclcpp::Client<StringService>::SharedPtr client_;
};
#endif // UNIVERSAL_CLIENT_H
