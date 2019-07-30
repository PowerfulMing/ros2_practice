#include "universal_actionlib/universal_actionlib_client.h"

using namespace std;

class Fibonacci_ActionClient: public UniversalActionClient
{
public:
    Fibonacci_ActionClient(string node_name, string action_name): UniversalActionClient(node_name, action_name)
    {
        node_name_ = node_name;
        action_name_ = action_name;
        std::cout << "FibonacciActionClient Constructor!" << std::endl;
    }
    ~Fibonacci_ActionClient(){}
    void handle_response()
    {
        std:: cout << "response: " << result_ << std::endl;
    }

    void handle_feedback()
    {
        std:: cout << "feedback: " << feedback_ << std::endl;
    }
};

int main(int argc, char ** argv)
{
    Fibonacci_ActionClient test_actionlib_client("test_action_client_node","Fibonacci");
    test_actionlib_client.init();
    // test_actionlib_client.send_request_with_call_back("5");
    // while(!test_actionlib_client.is_goal_done())
    // {
    //     std::cout << "Waiting Result in main..." << std::endl;
    //     std::cout << "The feedback is "
    //               << test_actionlib_client.get_feedback() << std::endl;
    //     std::this_thread::sleep_for(std::chrono::microseconds(100000));
    // }
    // std::cout << "The result is " << test_actionlib_client.get_result() << std::endl;

    return 0;
}
