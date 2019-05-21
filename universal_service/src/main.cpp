#include "universal_service/universal_service.hpp"
#include "iostream"
class TestServer:public UniversalService
{
public:
    TestServer(std::string node_name, std::string service_name): UniversalService(node_name, service_name)
    {
      node_name_ = node_name;
      service_name_ = service_name;
    }
    ~TestServer(){}
    
    virtual bool do_something(std::string str_req, std::string *str_rep)
    {
      std::cout << "HEY" << std::endl;
      std::cout << "I heard " << str_req << std::endl;
      int a = std::stoi(str_req);
      int tmp = a*2;
      *str_rep = std::to_string(tmp);
      std::cout << "return " <<  *str_rep << std::endl;
      return true;
    }
private:
    std::string node_name_;
    std::string service_name_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  // UniversalService my_service("my_server","my_service");
  TestServer my_service("my_server","my_service");
  rclcpp::Rate loop_rate(10);
  while(rclcpp::ok())
  {
    std::cout << "-------------------" << std::endl;
    loop_rate.sleep();
    my_service.call_spin_some();
  }
  rclcpp::shutdown();
  return 0;
}