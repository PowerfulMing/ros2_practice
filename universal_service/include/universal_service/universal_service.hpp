#ifndef UNIVERSAL_SERVICE_H
#define UNIVERSAL_SERVICE_H
#include "rclcpp/rclcpp.hpp"
#include "universal_interface/srv/string_service.hpp"

using StringService = universal_interface::srv::StringService;
using namespace std::placeholders;
using namespace std::chrono_literals;

class UniversalService
{
public:
  UniversalService(std::string node_name, std::string service_name);

  virtual bool do_something(std::string str_req, std::string *str_rep)
  {
    RCLCPP_INFO(node_->get_logger(),"TEST! Get %s", str_req);
    *str_rep = "inside!";
    return false;
  }
  void call_spin();
  void call_spin_some();
  /*** For bind callback function! ***/
  void handle_service(const std::shared_ptr<rmw_request_id_t> request_header,
                      const std::shared_ptr<StringService::Request> request,
                            std::shared_ptr<StringService::Response> response
                      );
private:
  std::string node_name_;
  std::string service_name_;
  rclcpp::Node::SharedPtr node_;
  rclcpp::Service<StringService>::SharedPtr srv_;
};
#endif // UNIVERSAL_SERVICE_H
