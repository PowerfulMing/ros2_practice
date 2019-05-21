#include "PathPlanningClient.h"

int main()
{
    PathPlanningClient *client = new PathPlanningClient();
    client->set_node_name("Client");
    client->set_service_name("PathPlanningService");
    client->init();
    std::cout << "Send update request..." << std::endl;
    client->send_request();

    while(1)
    {
    }
    return 0;
}
