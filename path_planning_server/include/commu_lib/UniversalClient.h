/**
 * Project Alice
 */
#ifndef _UNIVERSALCLIENT_H
#define _UNIVERSALCLIENT_H
#include "rclcpp/rclcpp.hpp"
#include "universal_interface/srv/string_service.hpp"
#include <chrono>
#include "boosttimer.hpp"
using StringService = universal_interface::srv::StringService;
typedef std::shared_future<std::shared_ptr<universal_interface::srv::StringService_Response>> StringServiceResponse;
using namespace std::placeholders;

class UniversalClient
{
public:
  enum TimeOutUnit
  {
    micro,
    milli
  };
  UniversalClient();
  UniversalClient(std::string node_name, std::string service_name);
  ~UniversalClient(void);
  bool send_request(std::string request_string);
  bool send_request_with_call_back(std::string request_string); //,const boost::function<void(std::string)> &func);

  // life
  void init(void);
  void reboot(void);

  // set
  void set_timeout_unit(TimeOutUnit tou) { time_out_unit_ = tou; }
  void set_timeout_interval_us(int64_t microSec) { timeout_interval_us_ = std::chrono::microseconds(microSec); }
  void set_timeout_interval_ms(int64_t milliSec) { timeout_interval_ms_ = std::chrono::milliseconds(milliSec); }
  void set_node_name(std::string nodeName) { node_name_ = nodeName; }
  void set_service_name(std::string serviceName) { service_name_ = serviceName; }
  // get
  int64_t timeout_interval_us(void) { return timeout_interval_us_.count(); }
  std::string node_name(void) { return node_name_; }
  std::string service_name(void) { return service_name_; }

protected:
  bool acquire_result();
  virtual void handle_response() = 0;

  std::string node_name_;

  std::string service_name_;

  rclcpp::Node::SharedPtr node_;

  rclcpp::Client<StringService>::SharedPtr client_;

  std::string result_;
  TimeOutUnit time_out_unit_;
  std::chrono::microseconds timeout_interval_us_;
  std::chrono::milliseconds timeout_interval_ms_;

  StringServiceResponse result_future_;

private:
  void wait_for_service_log();
};

#endif //_UNIVERSALCLIENT_H
