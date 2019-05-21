/**
 * Project Alice
 */

#include "UniversalPublisher.h"

/**
 * UniversalPublisher implementation
 */
UniversalPublisher::UniversalPublisher(std::string node_name, std::string topic_name)
{
    node_name_ = node_name;
    topic_name_ = topic_name;
    node_ = rclcpp::Node::make_shared(node_name_);
    publisher_ = node_->create_publisher<std_msgs::msg::String>(topic_name_);
}

void UniversalPublisher::setup_message(std::string input_message)
{
    input_message_ = input_message;
}

void UniversalPublisher::publish_message(void)
{
    auto msg = std::make_shared<std_msgs::msg::String>();
    auto message = std_msgs::msg::String();
    msg->data = input_message_;
    message.data = input_message_;
    publisher_->publish(message);
}

void UniversalPublisher::spin_publisher(void)
{
    rclcpp::spin_some(node_);
}
void UniversalPublisher::start_publish(void)
{
    timer_ = node_->create_wall_timer(500ms, std::bind(&UniversalPublisher::timer_callback, this));
}
void UniversalPublisher::timer_callback(void)
{
    publish_message();
}