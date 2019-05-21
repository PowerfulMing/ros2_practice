#ifndef UNIVERSAL_PUBLISHER
#define UNIVERSAL_PUBLISHER
#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;
using namespace std;

class UniversalPublisher
{
public:
  UniversalPublisher(string node_name, string topic_name);
  void setup_message(string input_message);
  void publish_message();
private:

  string node_name_;
  string topic_name_;
  string input_message_;
  rclcpp::Node::SharedPtr node_;
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
};

#endif // UNIVERSAL_PUBLISHER
