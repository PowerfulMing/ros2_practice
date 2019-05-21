#include "universal_client/universal_client.hpp"

UniversalClient::UniversalClient(std::string node_name, std::string service_name)
{
    node_name_ = node_name;
    service_name_ = service_name;
    node_ = rclcpp::Node::make_shared(node_name_);
    RCLCPP_INFO(node_->get_logger(),"Create Client");
    client_ = node_->create_client<StringService>(service_name_);
    while (!client_->wait_for_service(std::chrono::seconds(1)))
    {
      if (!rclcpp::ok()) {
        RCLCPP_ERROR(node_->get_logger(), "client interrupted while waiting for service to appear.");
      }
      RCLCPP_INFO(node_->get_logger(), "waiting for service to appear...");
    }
}

bool UniversalClient::send_request(std::string request_string)
{
  auto request = std::make_shared<StringService::Request>();
  request->str_req = request_string;
  auto result_future = client_->async_send_request(request);
  if (rclcpp::spin_until_future_complete(node_, result_future) !=
    rclcpp::executor::FutureReturnCode::SUCCESS)
  {
    RCLCPP_ERROR(node_->get_logger(), "service call failed :(");
    return false;
  }
  auto result = result_future.get();
  RCLCPP_INFO(node_->get_logger(), "result of %s", result->str_rep.c_str());
  return true;
}
