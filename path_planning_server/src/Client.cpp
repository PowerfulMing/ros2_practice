/**
 * Project Alice
 */

#include "Client.h"

/**
 * Client implementation
 */
Client::Client(void)
{
    has_request_ = false;
    already_send_request_ = false;
    set_node_name("Client");
    set_service_name("Service");

    timer_.bind(boost::bind(&Client::main_loop, this));
}

Client::~Client(void)
{
}
void Client::send_request(void)
{
    has_request_ = true;
    create_request();

    timer_.start(1);
}
void Client::create_request(void)
{
    mas_request_ = "test";
}
void Client::handle_response(void)
{
    timer_.stop();
    mas_response_ = result_;
    has_request_ = false;
    already_send_request_ = false;
}
void Client::main_loop(void)
{
    rclcpp::spin_some(node_);
    if (has_request_)
    {
        if (!already_send_request_)
        {
            if (send_request_with_call_back(mas_request_))
            {
                already_send_request_ = true;
            }
            else
            {
                //fail to send request
            }
        }
    }
}