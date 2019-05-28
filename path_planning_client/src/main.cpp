#include "PathPlanningClient.h"
#include <cstdlib>

int main(int argc, char *argv[])
{
    PathPlanningClient *client = new PathPlanningClient();
    client->set_node_name("Client");
    client->set_service_name("PathPlanningService");
    client->init();

    std::cout << "Send Request..." << std::endl;
    if(argc > 1)
    {
        std::cout << "Enter input from command!" << std::endl;
        std::cout << "From " << std::atoi(argv[1]) << " to " << std::atoi(argv[2]) << std::endl;
        client->setup_from_to(std::atoi(argv[1]),std::atoi(argv[2]));
    }else{
        std::cout << "Enter default input!" << std::endl;
        client->setup_from_to(0, 35);
    }
    client->send_request();

    while(1)
    {
    }
    return 0;
}
