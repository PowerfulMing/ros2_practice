/**
 * Project Alice
 */

#ifndef _UNIVERSALSERVICE_H
#define _UNIVERSALSERVICE_H
#include "rclcpp/rclcpp.hpp"
#include "universal_interface/srv/string_service.hpp"
#include "boosttimer.hpp"
#include <boost/bind.hpp>
using StringService = universal_interface::srv::StringService;
using namespace std::placeholders;
using namespace std::chrono_literals;

class UniversalService
{
public:
  UniversalService(void);
  UniversalService(std::string node_name, std::string service_name);
  ~UniversalService(void);
  // life cycles
  void init(void);
  void start_service(void);
  void reboot(void);
  void shudown(void);
  // set
  void set_node_name(std::string nodeName) { node_name_ = nodeName; }
  void set_service_name(std::string serviceName) { service_name_ = serviceName; }
  // get
  std::string node_name(void) { return node_name_; }
  std::string service_name(void) { return service_name_; }
  
  // Fill in what server should did
  virtual bool do_something(std::string str_req,
                            std::string *str_rep) = 0;

protected:
  // handle service call back function
  void handle_service(const std::shared_ptr<rmw_request_id_t> request_header,
                      const std::shared_ptr<StringService::Request> request,
                      std::shared_ptr<StringService::Response> response);

  std::string node_name_;
  std::string service_name_;
  rclcpp::Node::SharedPtr node_;
  rclcpp::Service<StringService>::SharedPtr srv_;
  // timer
  BoostTimer<> timer_;
  void init_timer(void);
  void spin_loop(void);
  void call_spin();
  void call_spin_some();
};

#endif //_UNIVERSALSERVICE_H