/**
 * Project Alice
 */

#include "GraphClient.h"
#include "jsoncpp/json/json.h"

/**
 * GraphClient implementation
 */
GraphClient::GraphClient(void)
{
    set_data_type("Graph");
}

// henry write here
/** create_data_update_request(void)
 * send a request to GraphServer for "updating map"
 * transform Graph to Json format and send to GraphServer
 */
void GraphClient::create_data_update_request(void)
{
    graph_.set_id(RequestType::kUpdatingMap);
    mas_request_ = graph_.to_string();
    std::cout << "request data command: " << mas_request_ << std::endl;
}

/** create_data_acquire_request(void)
 * send a request to GraphServer for "getting map"
 * 
 */
void GraphClient::create_data_acquire_request(void)
{
    graph_.set_id(RequestType::kGettingMap);
    mas_request_ = graph_.to_string();
    std::cout << "acquire data command: " << mas_request_ << std::endl;
}

bool GraphClient::parse_response(void)
{
    Json::Value response_root;
    Json::Reader reader;
    std::cout << "has response: " << std::endl;
    std::cout << mas_response_ << std::endl;
    // if mas_response_ has message, and the string format is as same as json,
    // then put mas_response_ message into root
    if (!reader.parse(mas_response_, response_root))
    {
        return false;
    }

    Json::Value root_info = response_root["MultiAgentSystemElements"][0];
    if (root_info["typename"].compare("Graph") != 0)
    {
        // root_info[i] is not "graph" (typename is not "graph")
        return false;
    }

    // check "id" : recognize what is client ask for "updating" or "getting" map
    std::cout<< root_info.toStyledString() << std::endl;
    if (root_info["id"] == RequestType::kUpdatingMap)
    {
        if (root_info["data"].compare("OK") != 0) // if update not success
        {
            cout << "Graph Client Error: server didn't update correctly" << endl;
            return false;
        }
        else // if map update success
        {
            cout << "Graph Client: server has already updated" << endl;
            data_acquired_ = true;
            return true;
        }
    }
    else // getting map, and save into graph_
    {
        if (!graph_.parse_string(mas_response_))
        {
            cout << "Graph Client Error : Graph parse fail" << endl;
            return false;
        }
        cout << "Graph Client: get map success" << endl;
        data_acquired_ = true;
        return true;
    }
}

bool GraphClient::set_graph(std::string gs)
{
    return this->graph_.parse_string(gs);
}
bool GraphClient::set_graph(Graph *g)
{
    return set_graph(g->to_string());
}
