/**
 * Project Alice
 */

#include "UniversalClient.h"

/**
 * UniversalClient implementation
 */
UniversalClient::UniversalClient()
{
  set_timeout_unit(TimeOutUnit::milli);
  set_timeout_interval_ms(100);
  //timeout_interval_us_ = std::chrono::microseconds(100);
}
UniversalClient::UniversalClient(std::string node_name, std::string service_name)
{
  node_name_ = node_name;
  service_name_ = service_name;
  set_timeout_unit(TimeOutUnit::milli);
  set_timeout_interval_ms(100);
  // timeout_interval_us_ = std::chrono::microseconds(100);
}
UniversalClient::~UniversalClient(void)
{
  rclcpp::shutdown();
}

void UniversalClient::init(void)
{
  rclcpp::init(0, NULL);
  node_ = rclcpp::Node::make_shared(node_name_);
  try
  {
    client_ = node_->create_client<StringService>(service_name_);
  }
  catch (rclcpp::exceptions::InvalidServiceNameError err)
  {
    RCLCPP_ERROR(node_->get_logger(), err.error_msg);
  }
}
void UniversalClient::reboot(void)
{
  rclcpp::shutdown();
  if (rclcpp::ok())
  {
    init();
  }
}
bool UniversalClient::send_request_with_call_back(std::string request_string) //, const boost::function<void(std::string)> &func)
{
  auto request = std::make_shared<StringService::Request>();
  request->str_req = request_string;
  bool serviceAccessible = false;
  if (time_out_unit_ == TimeOutUnit::micro)
  {
    serviceAccessible = client_->wait_for_service(timeout_interval_us_);
  }
  else
  {
    serviceAccessible = client_->wait_for_service(timeout_interval_ms_);
  }

  if (!serviceAccessible)
  {
    wait_for_service_log();
    return false;
  }
  using ServiceResponseFuture =
      rclcpp::Client<universal_interface::srv::StringService>::SharedFuture;
  auto response_received_callback = [this](ServiceResponseFuture future) {
    auto result = future.get();
    result_ = result->str_rep;
    handle_response();
  };
  result_future_ = client_->async_send_request(request, response_received_callback);
  return true;
}
bool UniversalClient::send_request(std::string request_string)
{
  auto request = std::make_shared<StringService::Request>();
  request->str_req = request_string;
  bool serviceAccessible = false;
  if (time_out_unit_ == TimeOutUnit::micro)
  {
    serviceAccessible = client_->wait_for_service(timeout_interval_us_);
  }
  else
  {
    serviceAccessible = client_->wait_for_service(timeout_interval_ms_);
  }

  if (!serviceAccessible)
  {
    wait_for_service_log();
    return false;
  }
  result_future_ = client_->async_send_request(request);
  return true;
}
bool UniversalClient::acquire_result()
{
  auto returnCode = rclcpp::spin_until_future_complete(node_, result_future_);
  if (returnCode != rclcpp::executor::FutureReturnCode::SUCCESS)
  {
    if (returnCode == rclcpp::executor::FutureReturnCode::TIMEOUT)
    {
      RCLCPP_ERROR(node_->get_logger(), " timeout with duration: 1ms");
    }
    else
    {
      RCLCPP_ERROR(node_->get_logger(), "service call failed :(");
    }
    return false;
  }
  else // success
  {
    auto result = result_future_.get();
    std::cout << result->str_rep.c_str();
    return true;
  }
}
void UniversalClient::wait_for_service_log()
{
  if (!rclcpp::ok())
  {
    RCLCPP_ERROR(node_->get_logger(), "client interrupted while waiting for service to appear.");
  }
  RCLCPP_INFO(node_->get_logger(), "waiting for service to appear...");
}
