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

    void setup_from_to(unsigned int from_id, unsigned int to_id)
    {
        from_id_ = from_id; to_id_ = to_id;
    }

    // Implement Function from Client
    void create_request(void);

    // Your own Handle Response
    void handle_response();
    bool phrase_response();
private:
    unsigned int from_id_;
    unsigned int to_id_;
};
