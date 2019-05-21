#include "universal_actionlib/universal_actionlib_server.h"
#include <iterator> // for ostream_iterator

class TestActionlibServer:public UniversalActionlibServer
{
public:
    TestActionlibServer(string node_name, string action_name): UniversalActionlibServer(node_name, action_name)
    {
      node_name_ = node_name;
      action_name_ = action_name;
      std::cout << "TestActionlibServer Constructor!" << std::endl;
    }
    ~TestActionlibServer(){}

    bool accept_goal(string goal)
    {
        int order = atoi( goal.c_str());
        once_ = true;
        current_iter_ = 2;
        if (order > 9000)
        {
            return false;
        }
        return true;
    }
    bool do_something(string goal, string *feedback, string *result)
    {        
      std::cout << "In do something outside!" << std::endl;
      std::cout << "The goal is " << std::stoi(goal) << std::endl;
        if(once_)
        {
          sequence_.clear();
          sequence_.push_back(0);
          sequence_.push_back(1);
          once_ = false;
        }
        std::stringstream tmp_string;
        std::copy(sequence_.begin(), sequence_.end(), std::ostream_iterator<int>(tmp_string, " "));
        *feedback = tmp_string.str().c_str();
        *result = *feedback;
        
        sequence_.push_back(sequence_[current_iter_-2]+sequence_[current_iter_-1]);

        if(current_iter_>=std::stoi(goal))
        {
          *result = *feedback;
          return true;
        }else{
          current_iter_++;
          return false;
        }
    }
private:
    string node_name_;
    string action_name_;
    std::vector<int> sequence_;
    int current_iter_;
    bool once_;

};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  UniversalActionlibServer *uni_actionlib_server_ptr;
  TestActionlibServer test_actionlib_server("test_action_server_node", "Fibonacci");
  uni_actionlib_server_ptr = &test_actionlib_server;
  // test_actionlib_server.init();
  // test_actionlib_server.call_spin();
  uni_actionlib_server_ptr->init();
  uni_actionlib_server_ptr->call_spin();

  rclcpp::shutdown();
  return 0;
}
