#include "Client.h"
#include <iostream>
#include <jsoncpp/json/writer.h>
#include <jsoncpp/json/reader.h>
#include <fstream>

using namespace std;

class PathPlanningClient : public Client
{
public:
    PathPlanningClient(){}
    ~PathPlanningClient(){}

    // Implement Function from Client
    void create_request(void);

    // Your own Handle Response
    void handle_response();
    bool phrase_response();
};
