#ifndef UNIVERSAL_ACTIONLIB_SERVER_H
#define UNIVERSAL_ACTIONLIB_SERVER_H
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "universal_interface/action/string_task.hpp"
#include "string"
#include "iostream"
#include "thread"
#include "functional"

using namespace std;
using StringTask = universal_interface::action::StringTask;
using GoalHandleStringTask = rclcpp_action::ServerGoalHandle<StringTask>;
using GoalID = rclcpp_action::GoalID;

class UniversalActionlibServer
{
public:
    UniversalActionlibServer(string node_name, string action_name);
    ~UniversalActionlibServer(){};

    void init();
    // Fill in the rules to accept or deny the goal
    virtual bool accept_goal(string goal);
    // virtual bool cancel_goal();
    // Fill in the algorithm that actionlib sever should did
    virtual bool do_something(string goal, string *feedback, string *result);
    void call_spin()
    {
        rclcpp::spin(node_);
    }

private:
    rclcpp_action::GoalResponse handle_goal(const GoalID &, std::shared_ptr<const StringTask::Goal>);
    rclcpp_action::CancelResponse handle_cancel(shared_ptr<GoalHandleStringTask> goal_handle);
    void handle_accepted(shared_ptr<GoalHandleStringTask> goal_handle);
    void execute(const shared_ptr<GoalHandleStringTask> goal_handle);

    string node_name_;
    string action_name_;
    rclcpp::Node::SharedPtr node_;
    rclcpp_action::Server<StringTask>::SharedPtr action_server_;
};
#endif // UNIVERSAL_ACTIONLIB_SERVER_H
