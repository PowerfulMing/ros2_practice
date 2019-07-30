#include "universal_actionlib/universal_actionlib_client.h"

UniversalActionClient::UniversalActionClient()
{
    set_timeout_unit(TimeOutUnit::sec);
    set_timeout_interval_ms(1000);
    //timeout_interval_us_ = std::chrono::microseconds(100);
}
UniversalActionClient::UniversalActionClient(std::string nodeName, std::string actionName)
{
    node_name_ = nodeName;
    action_name_ = actionName;
    set_timeout_unit(TimeOutUnit::sec);
    set_timeout_interval_ms(1000);
    // timeout_interval_us_ = std::chrono::microseconds(100);
}
UniversalActionClient::~UniversalActionClient(void)
{
    action_client_.reset();
    node_.reset();
    rclcpp::shutdown();
}

void UniversalActionClient::init(void)
{
    if (!rclcpp::is_initialized())
    {
        rclcpp::init(0, NULL);
    }
    node_ = rclcpp::Node::make_shared(node_name_);
    try
    {
        action_client_ = rclcpp_action::create_client<StringTask>(
            node_,
            action_name_);
    }
    catch (rclcpp::exceptions::InvalidServiceNameError err)
    {
        RCLCPP_ERROR(node_->get_logger(), err.error_msg);
    }
}
void UniversalActionClient::reboot(void)
{
    shutdown();
    if (rclcpp::ok())
    {
        init();
    }
}
void UniversalActionClient::shutdown(void)
{
    action_client_.reset();
    node_.reset();
    rclcpp::shutdown();
}
bool UniversalActionClient::send_request_with_call_back(std::string request_string) //, const boost::function<void(std::string)> &func)
{

    // using namespace std::placeholders;

    // this->timer_->cancel();

    // this->goal_done_ = false;

    if (!this->action_client_)
    {
        RCLCPP_ERROR(node_->get_logger(), "Action client not initialized");
    }

    if (!this->action_client_->wait_for_action_server(std::chrono::seconds(10)))
    {
        RCLCPP_ERROR(node_->get_logger(), "Action server not available after waiting");
        // this->goal_done_ = true;
        return false;
    }

    auto goal = StringTask::Goal();
    goal.task_s = request_string;

    RCLCPP_INFO(node_->get_logger(), "Sending goal");

    auto send_goal_options = rclcpp_action::Client<StringTask>::SendGoalOptions();
    send_goal_options.goal_response_callback =
        std::bind(&UniversalActionClient::goal_response_callback, this, std::placeholders::_1);
    send_goal_options.feedback_callback =
        std::bind(&UniversalActionClient::feedback_callback, this, std::placeholders::_1, std::placeholders::_2);
    send_goal_options.result_callback =
        std::bind(&UniversalActionClient::result_callback, this, std::placeholders::_1);
    auto goal_handle_future = this->action_client_->async_send_goal(goal, send_goal_options);

    return true;
}

void UniversalActionClient::goal_response_callback(std::shared_future<ClientGoalHandleStringTask::SharedPtr> future)
{
    auto goal_handle = future.get();
    if (!goal_handle)
    {
        RCLCPP_ERROR(node_->get_logger(), "Goal was rejected by server");
    }
    else
    {
        RCLCPP_INFO(node_->get_logger(), "Goal accepted by server, waiting for result");
    }
}

void UniversalActionClient::feedback_callback(
    ClientGoalHandleStringTask::SharedPtr,
    const std::shared_ptr<const StringTask::Feedback> feedback)
{
    RCLCPP_INFO(
        node_->get_logger(),
        "Next number in sequence received: %s",
        feedback->feedback_s);
    feedback_ = feedback->feedback_s;
}

void UniversalActionClient::result_callback(const ClientGoalHandleStringTask::WrappedResult &result)
{
    // this->goal_done_ = true;
    switch (result.code)
    {
    case rclcpp_action::ResultCode::SUCCEEDED:
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

    RCLCPP_INFO(node_->get_logger(), "Result received");
    // for (auto number : result.result->)
    // {
    //     RCLCPP_INFO(node_->get_logger(), "%" PRId64, number);
    // }
    // result.result->result_s;
    result_ = result.result->result_s;
    handle_response();
}