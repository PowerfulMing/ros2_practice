#include "PathPlanningServer.h"
#include <iostream>

using namespace std;

int main()
{
    PathPlanningServer *server = new PathPlanningServer();
    server->set_node_name("Service");
    server->set_service_name("PathPlanningService");
    server->init();
    server->run();
    getchar();
    server->stop();
//    while(true)
//    {
////        cout << "hello world!" << endl;
//        std::this_thread::sleep_for(std::chrono::microseconds(100000));
//    }
    return 0;
}
