#ifndef UNIVERSAL_ACTIONLIB_SERVER_H
#define UNIVERSAL_ACTIONLIB_SERVER_H
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "universal_interface/action/string_task.hpp"
#include "boosttimer.hpp"
#include "string"
#include "iostream"
#include "thread"
#include "functional"

using namespace std;
// using StringTask = universal_interface::action::StringTask;
// using GoalHandleStringTask = rclcpp_action::ServerGoalHandle<StringTask>;
using StringTask = universal_interface::action::StringTask;
using ServerGoalHandleStringTask = rclcpp_action::ServerGoalHandle<StringTask>;
using ClientGoalHandleStringTask = rclcpp_action::ClientGoalHandle<StringTask>;

// ROS2 Crysyal
//using GoalID = rclcpp_action::GoalID;
// ROS2 Dashing
using GoalID = rclcpp_action::GoalUUID;

class UniversalActionServer
{
public:
  UniversalActionServer(void);
  UniversalActionServer(std::string node_name, std::string action_name);
  ~UniversalActionServer(void);
  // life cycles
  void init(void);
  void start_action_service(void);
  void reboot(void);
  void shudown(void);
  // set
  void set_node_name(std::string nodeName) { node_name_ = nodeName; }
  void set_action_name(std::string actionName) { action_name_ = actionName; }
  // get
  std::string node_name(void) { return node_name_; }
  std::string action_name(void) { return action_name_; }

  // Fill in the rules to accept or deny the goal
  virtual bool accept_goal(std::string goal);
  // Fill in what action server should did
  virtual bool do_something(std::string goal, std::string *feedback, std::string *result);

protected:
  rclcpp_action::GoalResponse handle_goal(const GoalID &, std::shared_ptr<const StringTask::Goal>);
  rclcpp_action::CancelResponse handle_cancel(std::shared_ptr<ServerGoalHandleStringTask> goal_handle);
  void handle_accepted(std::shared_ptr<ServerGoalHandleStringTask> goal_handle);
  void execute_action(const std::shared_ptr<ServerGoalHandleStringTask> goal_handle);
  std::string node_name_;

  std::string action_name_;
  rclcpp::Node::SharedPtr node_;
  rclcpp_action::Server<StringTask>::SharedPtr action_server_;
  // timer
  BoostTimer<> timer_;
  void init_timer(void);
  void spin_loop(void);
  void call_spin();
  void call_spin_some();
};
#endif //_UNIVERSALACTIONSERVER_H
