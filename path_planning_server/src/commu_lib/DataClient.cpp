/**
 * Project Alice
 */

#include "DataClient.h"
#include "jsoncpp/json/json.h"

/**
 * DataClient implementation
 */
DataClient::DataClient()
{
    data_acquired_ = false;
    set_data_type("datatype");
}

DataClient::~DataClient()
{
}

void DataClient::acquire_data(void)
{
    mode_ = RequestDataMode::query;
    send_request();
}
void DataClient::update_data(void)
{
    mode_ = RequestDataMode::mutation;
    update_success_ = false;
    send_request();
}
void DataClient::handle_response(void)
{
    Client::handle_response();
    if (parse_response())
    {
        if (mode_ == RequestDataMode::query)
        {
            data_acquired_ = true;
        }
        else if (mode_ == RequestDataMode::mutation)
        {
            update_success_ = true;
        }
    }
    else
    {
        if (mode_ == RequestDataMode::mutation)
        {
            update_success_ = false;
        }
    }
}
void DataClient::create_request(void)
{
    if (mode_ == RequestDataMode::query)
    {
        create_data_acquire_request();
    }
    else if (mode_ == RequestDataMode::mutation)
    {
        std::cout<<"DataClient mode_ == RequestDataMode::mutation" <<std::endl;
        create_data_update_request();
    }
}
// update data request
void DataClient::create_data_update_request(void)
{
    Json::Value root;
    Json::Value MACData;
    Json::Value Data;
    Data[0]["add"] = "vertex";
    Data[0]["value"] = "value";
    MACData["id"] = 0;
    MACData["typename"] = data_type();
    MACData["data"] = Data;
    root["MultiAgentSystemElements"] = Json::Value(Json::arrayValue);
    root["MultiAgentSystemElements"][0] = MACData;
    mas_request_ = root.toStyledString();
}
// acquire data request
void DataClient::create_data_acquire_request(void)
{
    Json::Value root;
    Json::Value MACData;
    MACData["id"] = 0;
    MACData["typename"] = data_type();
    root["MultiAgentSystemElements"] = Json::Value(Json::arrayValue);
    root["MultiAgentSystemElements"][0] = MACData;
    mas_request_ = root.toStyledString();
}
bool DataClient::parse_response(void)
{
    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(mas_response_, root))
    {
        return false;
    }
    if (root["MultiAgentSystemElements"][0]["typename"] != data_type())
    {
        return false;
    }

    return true;
}
