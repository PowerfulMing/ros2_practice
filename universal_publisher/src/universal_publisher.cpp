#include "universal_publisher/universal_publisher.hpp"

UniversalPublisher::UniversalPublisher(string node_name, string topic_name)
{
  node_name_ = node_name;
  topic_name_ = topic_name;
  rclcpp::init(0, NULL);
  node_ = rclcpp::Node::make_shared(node_name_);

  publisher_ = node_->create_publisher<std_msgs::msg::String>(topic_name_);
}

void UniversalPublisher::setup_message(string input_message)
{
  input_message_ = input_message;
}
  
void UniversalPublisher::publish_message()
{
  auto message = std_msgs::msg::String();
  message.data = input_message_;
  RCLCPP_INFO(node_->get_logger(), "Publishing: '%s'", message.data.c_str());
  publisher_->publish(message);
}

