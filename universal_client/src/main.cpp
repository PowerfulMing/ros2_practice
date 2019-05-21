#include "universal_client/universal_client.hpp"
#include "iostream"

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  // UniversalService my_service("my_server","my_service");
  UniversalClient my_client("my_client","my_service");
  rclcpp::Rate loop_rate(10);
  int i=0;
  while(rclcpp::ok())
  {
    loop_rate.sleep();
    std::string my_str = std::to_string(i);
    my_client.send_request(my_str);
    std::cout << "------------------------------------" << std::endl;
    i++;
  }
  rclcpp::shutdown();
  return 0;
}