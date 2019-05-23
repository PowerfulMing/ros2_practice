/**
 * Project Alice
 */
#include "UniversalService.h"

/**
 * UniversalService implementation
 */
UniversalService::UniversalService(void)
{
    init_timer();
}
UniversalService::UniversalService(std::string node_name, std::string service_name)
{
    node_name_ = node_name;
    service_name_ = service_name;
    init_timer();
}
UniversalService::~UniversalService(void)
{
    rclcpp::shutdown();
}
//
void UniversalService::init_timer(void)
{
    timer_.bind(boost::bind(&UniversalService::spin_loop, this));
}
void UniversalService::init(void)
{
    rclcpp::init(0, NULL);
    node_ = rclcpp::Node::make_shared(node_name_);
    auto bind_fun = std::bind(
        &UniversalService::handle_service,
        this,
        std::placeholders::_1,
        std::placeholders::_2,
        std::placeholders::_3);
    srv_ = node_->create_service<StringService>(
        service_name_,
        bind_fun);

}
void UniversalService::start_service(void)
{
    timer_.start(1);
}
void UniversalService::reboot(void)
{
    rclcpp::shutdown();
    if (rclcpp::ok())
    {
        init();
    }
}
void UniversalService::shudown(void)
{
    timer_.stop();
    std::cout << "timer stopped" << std::endl;
    timer_.destroy();
    std::cout << "timer destoried" << std::endl;
}


void UniversalService::handle_service(
    const std::shared_ptr<rmw_request_id_t> request_header,
    const std::shared_ptr<StringService::Request> request,
    std::shared_ptr<StringService::Response> response)
{
    (void)request_header;
    this->do_something(request->str_req, &response->str_rep);
}
void UniversalService::call_spin_some()
{
    rclcpp::spin_some(node_);
}
void UniversalService::call_spin()
{
    rclcpp::spin(node_);
}
void UniversalService::spin_loop(void)
{
    if (rclcpp::ok())
    {
        call_spin_some();
    }
    else
    {
        RCLCPP_ERROR(node_->get_logger(), "rclcpp not ok, system shutdown");
        shudown();
    }
}

