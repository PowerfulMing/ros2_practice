#include "universal_actionlib/universal_actionlib_client.h"

UniversalActionClient::UniversalActionClient()
{
    // timeout_interval_us_ = std::chrono::microseconds(100);
}
UniversalActionClient::UniversalActionClient(std::string node_name, std::string task_name)
{
    node_name_ = node_name;
    task_name_ = task_name;
    // timeout_interval_us_ = std::chrono::microseconds(100);
}
UniversalActionClient::~UniversalActionClient(void)
{
    spinGoalThread.join();
    rclcpp::shutdown();
}

void UniversalActionClient::init(void)
{
    if(!rclcpp::is_initialized())
        rclcpp::init(0, nullptr);
    node_ = rclcpp::Node::make_shared(node_name_);
    goal_accept_ = false;
    try
    {
        action_client_ = rclcpp_action::create_client<StringTask>(node_, task_name_);
        if (!action_client_->wait_for_action_server(std::chrono::seconds(3)))
        {
            RCLCPP_ERROR(node_->get_logger(), "Action server not available after waiting");
        }
    }
    catch (rclcpp::exceptions::InvalidServiceNameError err)
    {
        RCLCPP_ERROR(node_->get_logger(), err.error_msg);
    }
}

void UniversalActionClient::reboot(void)
{
    action_client_.reset();
    node_.reset();
    rclcpp::shutdown();
    if (rclcpp::ok())
    {
        init();
    }
}

void UniversalActionClient::feedback_callback(rclcpp_action::ClientGoalHandle<StringTask>::SharedPtr,
  const std::shared_ptr<const StringTask::Feedback> feedback)
{
    feedback_ = feedback->feedback_s.c_str();
    RCLCPP_INFO(node_->get_logger(), "---> FEEDBACK: %s", feedback->feedback_s.c_str());
}

void UniversalActionClient::spin_goal_handle()
{
    if (rclcpp::spin_until_future_complete(node_, goal_handle_future) != rclcpp::executor::FutureReturnCode::SUCCESS)
    {
        RCLCPP_ERROR(node_->get_logger(), "send goal call failed :(");
        goal_accept_ = false;
        return;
    }
    RCLCPP_INFO(node_->get_logger(), "After Sending goal");
    goal_handle = goal_handle_future.get();
    RCLCPP_INFO(node_->get_logger(), "After get");

    if (!goal_handle) {
        RCLCPP_ERROR(node_->get_logger(), "Goal was rejected by server");
        goal_accept_ = false;
        return;
    }
    goal_accept_ = true;
    if(goal_accept_)
    {
        result_callback();
    }
}

bool UniversalActionClient::send_request(std::string task_string)
{
    goal_done_ = false;
    auto goal_msg = StringTask::Goal();
    auto goal_msg2 = StringTask::Goal();
    goal_msg.task_s = task_string;
    goal_msg2.task_s = "6";
    RCLCPP_INFO(node_->get_logger(), "Sending goal");
    goal_handle_future = action_client_->async_send_goal(goal_msg, std::bind(&UniversalActionClient::feedback_callback, this, std::placeholders::_1, std::placeholders::_2));
    goal_handle_future = action_client_->async_send_goal(goal_msg2, std::bind(&UniversalActionClient::feedback_callback, this, std::placeholders::_1, std::placeholders::_2));
    spinGoalThread = std::thread(&UniversalActionClient::spin_goal_handle,this);

    return goal_accept_;
}
void UniversalActionClient::result_callback()
{
    result_future = goal_handle->async_result();
    RCLCPP_INFO(node_->get_logger(), "Waiting for result");

    if (rclcpp::spin_until_future_complete(node_, result_future) !=
        rclcpp::executor::FutureReturnCode::SUCCESS)
    {
        RCLCPP_ERROR(node_->get_logger(), "get result call failed :(");
        return;
    }
    result = result_future.get();

    switch(result.code) {
      case rclcpp_action::ResultCode::SUCCEEDED:
        goal_done_ = true;
        break;
      case rclcpp_action::ResultCode::ABORTED:
        RCLCPP_ERROR(node_->get_logger(), "Goal was aborted");
        return;
      case rclcpp_action::ResultCode::CANCELED:
        RCLCPP_ERROR(node_->get_logger(), "Goal was canceled");
        return;
      default:
        RCLCPP_ERROR(node_->get_logger(), "Unknown result code");
        return;
    }
    RCLCPP_INFO(node_->get_logger(), "result received");
    result_ = result.response->result_s;
}

std::string UniversalActionClient::get_result()
{
//    std::cout << "ACTIONLIB SERVER RESULT: " << result_ << std::endl;
    return result_;
}
