#ifndef _UNIVERSALACTIONCLIENT_H
#define _UNIVERSALACTIONCLIENT_H
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "universal_interface/action/string_task.hpp"
#include <chrono>
#include <inttypes.h>
#include <memory>
#include "iostream"
#include <thread>
#include "boosttimer.hpp"

using namespace std::chrono;
using StringTask = universal_interface::action::StringTask;
using GoalHandleStringTask = rclcpp_action::ClientGoalHandle<StringTask>;

typedef std::shared_future<std::shared_ptr<universal_interface::action::StringTask_Result_Response>> StringTaskResultResponse;
typedef std::shared_future<std::shared_ptr<universal_interface::action::StringTask_Goal_Response>> StringTaskGoalResponse;


class UniversalActionClient
{
public:
    UniversalActionClient();
    UniversalActionClient(std::string node_name, std::string service_name);
    ~UniversalActionClient(void);
    bool send_request(std::string request_string);

    // life
    void init(void);
    void reboot(void);
    bool is_goal_done(){return goal_done_;}

    // set
    void set_timeout_interval_us(int64_t microSec) { timeout_interval_us_ = std::chrono::microseconds(microSec); }
    void set_node_name(std::string nodeName) { node_name_ = nodeName; }
    void set_action_service_name(std::string taskName) { task_name_ = taskName; }
    // get
    int64_t get_timeout_interval_us(void) { return timeout_interval_us_.count(); }
    std::string get_node_name(void) { return node_name_; }
    std::string get_action_service_name(void) { return task_name_; }
    std::string get_feedback(){return feedback_;}
    std::string get_result();

    // temporarily... used to print out the message of ROS... Remember to remove it...
    rclcpp::Node::SharedPtr get_node(){return node_;}
protected:
    // virtual void handle_result();
    // virtual void handle_feedback();
    void feedback_callback(rclcpp_action::ClientGoalHandle<StringTask>::SharedPtr,
                           const std::shared_ptr<const StringTask::Feedback> feedback);
    std::string node_name_;
    std::string task_name_;

    rclcpp::Node::SharedPtr node_;
    rclcpp_action::Client<StringTask>::SharedPtr action_client_;
    rclcpp_action::ClientGoalHandle<StringTask>::SharedPtr goal_handle;
    std::shared_future<rclcpp_action::ClientGoalHandle<StringTask>::SharedPtr> goal_handle_future;
    std::shared_future<rclcpp_action::ClientGoalHandle<StringTask>::Result> result_future;
    rclcpp_action::ClientGoalHandle<StringTask>::Result result;

    bool goal_done_;
    bool goal_accept_;
    std::string result_;
    std::string feedback_;
    std::chrono::microseconds timeout_interval_us_;
    StringTaskGoalResponse result_future_;
    void result_callback();
    void spin_goal_handle();

    // timer
    BoostTimer<> timer_;
    BoostTimer<> timer_2;
    std::thread spinGoalThread;
};

#endif //_UNIVERSALACTIONCLIENT_H

