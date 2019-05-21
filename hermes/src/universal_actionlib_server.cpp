#include "universal_actionlib/universal_actionlib_server.hpp"

UniversalActionlibServer::UniversalActionlibServer(string action_name)
{
    action_name_ = action_name;
    node_ = rclcpp::Node::make_shared("test");// v ed
    RCLCPP_INFO(rclcpp::get_logger(action_name_),"Creating Node %s...",action_name_.c_str());
}

void UniversalActionlibServer::init()
{
    // success one:
    // lambda case:
    // auto l_handle_goal = [](
    // const GoalID &, std::shared_ptr<const StringTask::Goal>)
    // {
    //     return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
    // };

    // using GoalHandle = rclcpp_action::ServerGoalHandle<StringTask>;

    // auto l_handle_cancel = [](std::shared_ptr<GoalHandle>)
    // {
    //     return rclcpp_action::CancelResponse::ACCEPT;
    // };

    // std::shared_ptr<GoalHandle> received_handle;
    // auto l_handle_accepted = [&received_handle](std::shared_ptr<GoalHandle> handle)
    // {
    //     received_handle = handle;
    // };

    // action_server_ = rclcpp_action::create_server<StringTask>(
    //     node_,
    //     action_name_,
    //     l_handle_goal,
    //     l_handle_cancel,
    //     l_handle_accepted
    // );


    RCLCPP_INFO(rclcpp::get_logger(action_name_),"Establishing the action server %s...",action_name_.c_str());
    action_server_ = rclcpp_action::create_server<StringTask>(
        node_,
        action_name_,
        std::bind(&UniversalActionlibServer::handle_goal, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&UniversalActionlibServer::handle_cancel, this, std::placeholders::_1),
        std::bind(&UniversalActionlibServer::handle_accepted, this, std::placeholders::_1)
    );
    
}

bool UniversalActionlibServer::accept_goal(string goal)
{
    RCLCPP_INFO(rclcpp::get_logger(action_name_),"Setup the rule for accepting a goal.");
    return true;
}

bool UniversalActionlibServer::do_something(string goal, string *feedback, string *result)
{
    RCLCPP_INFO(rclcpp::get_logger(action_name_),"Canceling the goal...\n");
    *feedback = "NOT IMPLEMENT YET";
    *result = "NOT IMPLEMENT YET";
    return true;
}

rclcpp_action::GoalResponse UniversalActionlibServer::handle_goal(const GoalID &uuid, std::shared_ptr<const StringTask::Goal>goal)
{
    if(accept_goal(goal->task_s))
    {
        return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
    }else{
        return rclcpp_action::GoalResponse::REJECT;
    }
    return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
}

rclcpp_action::CancelResponse UniversalActionlibServer::handle_cancel(shared_ptr<GoalHandleStringTask> goal_handle)
{
    RCLCPP_INFO(rclcpp::get_logger(action_name_), "Got request to cancel goal");
    (void)goal_handle;
    return rclcpp_action::CancelResponse::ACCEPT;
}

void UniversalActionlibServer::handle_accepted(shared_ptr<GoalHandleStringTask> goal_handle)
{
    std::thread{&UniversalActionlibServer::execute, this, goal_handle}.detach();
}

void UniversalActionlibServer::execute(const shared_ptr<GoalHandleStringTask> goal_handle)
{
    RCLCPP_INFO(rclcpp::get_logger(action_name_), "Executing goal");
    rclcpp::Rate loop_rate(1);
    const auto goal = goal_handle->get_goal();
    auto _feedback = std::make_shared<StringTask::Feedback>();
    auto _result = std::make_shared<StringTask::Result>();

    // Storage the result from do_something
    string tmp_goal;
    string tmp_feedback;
    string tmp_result;

    tmp_goal = goal->task_s;

    while(!do_something(tmp_goal, &tmp_feedback, &tmp_result))
    {
        _feedback->feedback_s = tmp_feedback;
        goal_handle->publish_feedback(_feedback);
        RCLCPP_INFO(rclcpp::get_logger(action_name_), "Publish Feedback");

        loop_rate.sleep();
    }
    _result->result_s = tmp_result;
    goal_handle->set_succeeded(_result);
    RCLCPP_INFO(rclcpp::get_logger(action_name_), "Goal Suceeded");
}