#include "SiblingClient.h"

SiblingClient::SiblingClient(void)
{

}

void SiblingClient::create_data_update_request(void)
{
    sibling_tree_.set_id(RequestType::kUpdating);
    mas_request_ = sibling_tree_.to_string();
    std::cout << "request data command: " << mas_request_ << std::endl;
}

void SiblingClient::create_data_acquire_request(void)
{
    sibling_tree_.set_id(RequestType::kGetting);
    mas_request_ = sibling_tree_.to_string();
    std::cout << "acquire data command: " << mas_request_ << std::endl;
}

bool SiblingClient::parse_response(void)
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
    if (root_info["typename"].compare("Sibling") != 0)
    {
        // root_info[i] is not "Sibling" (typename is not "Sibling")
        return false;
    }

    // check "id" : recognize what is client ask for "updating" or "getting" map
    std::cout<< root_info.toStyledString() << std::endl;
    if (root_info["id"] == RequestType::kUpdating)
    {
        if (root_info["data"].compare("OK") != 0) // if update not success
        {
            cout << "Sibling Client Error: server didn't update correctly" << endl;
            return false;
        }
        else // if map update success
        {
            cout << "Sibling Client: server has already updated" << endl;
            data_acquired_ = true;
            return true;
        }
    }
    else // getting map, and save into graph_
    {
        if (!sibling_tree_.parse_string(mas_response_))
        {
            cout << "Sibling Client Error : Sibling parse fail" << endl;
            return false;
        }
        cout << "Sibling Client: get sibling tree success" << endl;
        data_acquired_ = true;
        return true;
    }
}


bool SiblingClient::set_sibling(SiblingTree *s)
{
    return set_sibling(s->to_string());
}


bool SiblingClient::set_sibling(std::string ss)
{
    std::cout << "SiblingClient: set_sibling" << std::endl;
    return this->sibling_tree_.parse_string(ss);
    // std::cout << this->sibling_tree_.parse_string(ss) << std::endl;
}