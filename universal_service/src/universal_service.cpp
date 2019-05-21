#include "universal_service/universal_service.hpp"

UniversalService::UniversalService(std::string node_name, std::string service_name)
{
    node_name_ = node_name;
    service_name_ = service_name;
    node_ = rclcpp::Node::make_shared(node_name_);
    RCLCPP_INFO(node_->get_logger(),"Create Service");
    /*** Not work!!! ***/
    // auto bind_fun = std::bind(
    //     &UniversalService::handle_service,
    //     this,
    //     std::placeholders::_1,
    //     std::placeholders::_2, 
    //     std::placeholders::_3
    // );
    // srv_ = node_->create_service<StringService>(
    //     service_name_, 
    //     bind_fun
    // );

    /*** Work!!! ***/
    auto handle_service =
      [this](const std::shared_ptr<rmw_request_id_t> request_header,
             const std::shared_ptr<StringService::Request> request,
                   std::shared_ptr<StringService::Response> response) -> void
      {
          (void)request_header;
          RCLCPP_INFO(node_->get_logger(),"request: %s", request->str_req.c_str());
          this->do_something(request->str_req, &response->str_rep);
        //   response->str_rep = "Done";
      };

    srv_ = node_->create_service<StringService>(service_name_, handle_service);
}

void UniversalService::call_spin_some()
{
  rclcpp::spin_some(node_);
}
void UniversalService::call_spin()
{
    rclcpp::spin(node_);
}

void UniversalService::handle_service(
                      const std::shared_ptr<rmw_request_id_t> request_header,
                      const std::shared_ptr<StringService::Request> request,
                            std::shared_ptr<StringService::Response> response)
{
    (void)request_header;
    RCLCPP_INFO(node_->get_logger(),"request: %s", request->str_req.c_str());
    response->str_rep = "Done";
}