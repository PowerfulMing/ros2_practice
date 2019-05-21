#include "universal_subscriber/universal_subscriber.hpp"
#include "iostream"

int main(int argc, char * argv[])
{
  rclcpp::init(0, NULL);
  UniversalSubscriber my_Subscriber("node","topic");
  rclcpp::Rate loop_rate(10);
  while(rclcpp::ok())
  {
    loop_rate.sleep();
    my_Subscriber.call_spin_some();
  }
  rclcpp::shutdown();
  return 0;
}
