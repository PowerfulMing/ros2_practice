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
//using StringTask = universal_interface::action::StringTask;
//using GoalHandleStringTask = rclcpp_action::ClientGoalHandle<StringTask>;
using StringTask = universal_interface::action::StringTask;
using ServerGoalHandleStringTask = rclcpp_action::ServerGoalHandle<StringTask>;
using ClientGoalHandleStringTask = rclcpp_action::ClientGoalHandle<StringTask>;

// typedef std::shared_future<std::shared_ptr<universal_interface::action::StringTask_Result_Response>> StringTaskResultResponse;
// typedef std::shared_future<std::shared_ptr<universal_interface::action::StringTask_Goal_Response>> StringTaskGoalResponse;


class UniversalActionClient
{
public:
    enum TimeOutUnit
    {
        micro,
        milli,
        sec
    };
    UniversalActionClient();
    UniversalActionClient(std::string nodeName, std::string actionServiceName);
    ~UniversalActionClient(void);
    bool send_request(std::string request_string);
    bool send_request_with_call_back(std::string request_string); //,const boost::function<void(std::string)> &func);

    // life
    void init(void);
    void reboot(void);
    void shutdown(void);
    // set
    void set_timeout_unit(TimeOutUnit tou) { time_out_unit_ = tou; }
    void set_timeout_interval_us(int64_t microSec) { timeout_interval_us_ = std::chrono::microseconds(microSec); }
    void set_timeout_interval_ms(int64_t milliSec) { timeout_interval_ms_ = std::chrono::milliseconds(milliSec); }
    void set_node_name(std::string nodeName) { node_name_ = nodeName; }
    void set_action_name(std::string actionName) { action_name_ = actionName; }
    // get
    int64_t timeout_interval_us(void) { return timeout_interval_us_.count(); }
    std::string node_name(void) { return node_name_; }
    std::string action_name(void) { return action_name_; }

protected:
    bool acquire_result();
    virtual void handle_response() = 0;
    virtual void handle_feedback() = 0;
    std::string node_name_;

    std::string action_name_;

    rclcpp::Node::SharedPtr node_;
    rclcpp_action::Client<StringTask>::SharedPtr action_client_;

    std::string result_;
    std::string feedback_;
    TimeOutUnit time_out_unit_;
    std::chrono::microseconds timeout_interval_us_;
    std::chrono::milliseconds timeout_interval_ms_;

private:
    void goal_response_callback(std::shared_future<ClientGoalHandleStringTask::SharedPtr> future);

    void feedback_callback(
        ClientGoalHandleStringTask::SharedPtr,
        const std::shared_ptr<const StringTask::Feedback> feedback);

    void result_callback(const ClientGoalHandleStringTask::WrappedResult &result);
};

#endif //_UNIVERSALACTIONCLIENT_H


