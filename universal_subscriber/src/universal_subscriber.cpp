#include "universal_subscriber/universal_subscriber.hpp"

UniversalSubscriber::UniversalSubscriber(){}
UniversalSubscriber::UniversalSubscriber(string node_name, string topic_name)
{
    node_name_ = node_name;
    topic_name_ = topic_name;
    node_ = rclcpp::Node::make_shared(node_name_);
    subscription_ = node_->create_subscription<std_msgs::msg::String>(
    topic_name_, std::bind(&UniversalSubscriber::topic_callback, this, _1));
}

void UniversalSubscriber::set_node_name(std::string name)
{
    node_name_ = name;
}
void UniversalSubscriber::set_topic_name(std::string name)
{
    topic_name_ = name;
}

void UniversalSubscriber::init()
{
    if(node_name_.empty())
    {
        node_name_ = "node";
    }
    if(topic_name_.empty())
    {
        topic_name_ = "topic";
    }
    node_ = rclcpp::Node::make_shared(node_name_);
    subscription_ = node_->create_subscription<std_msgs::msg::String>(
    topic_name_, std::bind(&UniversalSubscriber::topic_callback, this, _1));
}

std::string UniversalSubscriber::get_node_names()
{
    return node_name_;
}
std::string UniversalSubscriber::get_topic_names()
{
    return topic_name_;
}

std::string UniversalSubscriber::get_message()
{
    return receive_message_;
}
void UniversalSubscriber::call_spin_some()
{
    rclcpp::spin_some(node_);
}

void UniversalSubscriber::topic_callback(const std_msgs::msg::String::SharedPtr msg)
{
    RCLCPP_INFO(node_->get_logger(), "I heard: '%s'", msg->data.c_str());
    receive_message_ = msg->data.c_str();
}