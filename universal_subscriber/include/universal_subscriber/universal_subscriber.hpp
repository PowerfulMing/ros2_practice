#ifndef UNIVERSAL_SUBSCRIBER_H
#define UNIVERSAL_SUBSCRIBER_H
#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;
using namespace std;

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
using std::placeholders::_1;

class UniversalSubscriber
{
public:
  UniversalSubscriber();
  UniversalSubscriber(string node_name, string topic_name);

  void set_node_name(std::string name);
  void set_topic_name(std::string name);

  void init();
  std::string get_node_names();
  std::string get_topic_names();
  std::string get_message();
  void call_spin_some();

private:
  void topic_callback(const std_msgs::msg::String::SharedPtr msg);

  string node_name_;
  string topic_name_;
  string receive_message_;
  rclcpp::Node::SharedPtr node_;
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

#endif // UNIVERSAL_SUBSCRIBER_H