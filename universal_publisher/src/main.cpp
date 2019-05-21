#include "universal_publisher/universal_publisher.hpp"

int main(int argc, char * argv[])
{
  UniversalPublisher my_rcl_topic("node","topic");
  // UniversalRclTopic aa("my_node","my_topic");
  rclcpp::Rate loop_rate(10);
  while(rclcpp::ok())
  {
    my_rcl_topic.setup_message("test");
    my_rcl_topic.publish_message();
    loop_rate.sleep();
  }
  rclcpp::shutdown();
  return 0;
}
