#include "universal_actionlib/universal_actionlib_server.hpp"

UniversalActionServer::UniversalActionServer(void)
{
    init_timer();
}
UniversalActionServer::UniversalActionServer(std::string node_name, std::string actionName)
{
    node_name_ = node_name;
    action_name_ = actionName;
}
UniversalActionServer::~UniversalActionServer(void)
{
    rclcpp::shutdown();
}
void UniversalActionServer::init_timer(void)
{
    timer_.bind(boost::bind(&UniversalActionServer::spin_loop, this));
}
void UniversalActionServer::init()
{
    if (!rclcpp::is_initialized())
        rclcpp::init(0, NULL);
    node_ = rclcpp::Node::make_shared(node_name());
    action_server_ = rclcpp_action::create_server<StringTask>(
        node_,
        action_name_,
        std::bind(&UniversalActionServer::handle_goal, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&UniversalActionServer::handle_cancel, this, std::placeholders::_1),
        std::bind(&UniversalActionServer::handle_accepted, this, std::placeholders::_1));
}
void UniversalActionServer::start_action_service(void)
{
    timer_.start(1);
}
void UniversalActionServer::reboot(void)
{
    rclcpp::shutdown();
    if (rclcpp::ok())
    {
        init();
    }
}
void UniversalActionServer::shudown(void)
{
    action_server_.reset();
    node_.reset();
    timer_.stop();
    std::cout << "timer stopped" << std::endl;
    timer_.destroy();
    std::cout << "timer destoried" << std::endl;
}

bool UniversalActionServer::accept_goal(std::string goal)
{
    RCLCPP_INFO(rclcpp::get_logger(action_name_), "Goal : " + goal);
    return true;
}

bool UniversalActionServer::do_something(std::string goal, std::string *feedback, std::string *result)
{
    RCLCPP_INFO(rclcpp::get_logger(action_name_), "Canceling the goal: " + goal);
    *feedback = "NOT IMPLEMENT YET";
    *result = "NOT IMPLEMENT YET";
    return true;
}

rclcpp_action::GoalResponse UniversalActionServer::handle_goal(const GoalID &uuid, std::shared_ptr<const StringTask::Goal> goal)
{
    RCLCPP_INFO(rclcpp::get_logger(action_name_), "goal id: " + rclcpp_action::to_string(uuid));

    if (accept_goal(goal->task_s))
    {
        std::cout << "goal accepted" << std::endl;
        return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
    }
    else
    {
        return rclcpp_action::GoalResponse::REJECT;
    }
}

rclcpp_action::CancelResponse UniversalActionServer::handle_cancel(std::shared_ptr<ServerGoalHandleStringTask> goal_handle)
{
    RCLCPP_INFO(rclcpp::get_logger(action_name_), "Got request to cancel goal");
    (void)goal_handle;
    return rclcpp_action::CancelResponse::ACCEPT;
}

void UniversalActionServer::handle_accepted(std::shared_ptr<ServerGoalHandleStringTask> goal_handle)
{
    std::thread{&UniversalActionServer::execute_action, this, goal_handle}.detach();
}

void UniversalActionServer::execute_action(const std::shared_ptr<ServerGoalHandleStringTask> goal_handle)
{
    // RCLCPP_INFO(rclcpp::get_logger(action_name_), "Executing goal");
    rclcpp::Rate loop_rate(1);
    const auto goal = goal_handle->get_goal();
    auto _feedback = std::make_shared<StringTask::Feedback>();
    auto _result = std::make_shared<StringTask::Result>();

    // Storage the result from do_something
    std::string tmp_goal;
    std::string tmp_feedback;
    std::string tmp_result;

    tmp_goal = goal->task_s;

    while (!do_something(tmp_goal, &tmp_feedback, &tmp_result))
    {
        _feedback->feedback_s = tmp_feedback;
        goal_handle->publish_feedback(_feedback);
        // RCLCPP_INFO(rclcpp::get_logger(action_name_), "Publish Feedback");
        // loop_rate.sleep();
    }
    while(!rclcpp::ok())
    {
        ;
    }
    if (rclcpp::ok())
    {
        _result->result_s = tmp_result;
        goal_handle->succeed(_result);
        RCLCPP_INFO(rclcpp::get_logger(action_name_), "Goal Suceeded");
    }
}
void UniversalActionServer::call_spin_some()
{
    rclcpp::spin_some(node_);
}
void UniversalActionServer::call_spin()
{
    rclcpp::spin(node_);
}
void UniversalActionServer::spin_loop(void)
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
