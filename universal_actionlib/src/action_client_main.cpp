#include "universal_actionlib/universal_actionlib_client.h"

int main(int argc, char ** argv)
{
    UniversalActionClient test_actionlib_client("test_action_client_node","Fibonacci");
    test_actionlib_client.init();
    test_actionlib_client.send_request("5");
    while(!test_actionlib_client.is_goal_done())
    {
        std::cout << "Waiting Result in main..." << std::endl;
        std::cout << "The feedback is "
                  << test_actionlib_client.get_feedback() << std::endl;
        std::this_thread::sleep_for(std::chrono::microseconds(100000));
    }
    std::cout << "The result is " << test_actionlib_client.get_result() << std::endl;

    return 0;
}
