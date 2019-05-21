/**
 * Project Alice
 */

#ifndef _UNIVERSALPUBLISHER_H
#define _UNIVERSALPUBLISHER_H
#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
using namespace std::chrono_literals;
class UniversalPublisher
{
  public:
    UniversalPublisher(std::string node_name, std::string topic_name);
    void setup_message(std::string input_message);
    void publish_message(void);
    void spin_publisher(void);
    void start_publish(void);

  private:
    void timer_callback(void);
    std::string node_name_;
    std::string topic_name_;
    std::string input_message_;
    rclcpp::Node::SharedPtr node_;
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
};

#endif //_UNIVERSALPUBLISHER_H