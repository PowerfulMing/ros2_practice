#include "universal_actionlib/universal_actionlib_server.hpp"
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "move_base_msgs/MoveBaseAction.h"
#include "actionlib/client/simple_action_client.h"
#include "geometry_msgs/Pose.h"
#include <string.h> // FOR STROKE

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

class ITRIQ600ROS1Bridge:public UniversalActionServer
{
public:
    ITRIQ600ROS1Bridge(string node_name, string action_name): UniversalActionServer(node_name, action_name)
    {
        action_name_ = action_name;
        movebase_ac_ = new MoveBaseClient("Libra/move_base", true);
        std::cout << "ITRIQ600ROS1Bridge Constructor!" << std::endl;
        while(!movebase_ac_->waitForServer(ros::Duration(5.0))){
          ROS_INFO("Waiting for the move_base action server to come up");
        }
    }
    ~ITRIQ600ROS1Bridge(){}


    void split(const std::string& s, std::vector<std::string>& sv,const char delim = ' ')
    {
      sv.clear();
      std::istringstream iss(s);
      std::string temp;

      while (std::getline(iss, temp, delim)) {
        sv.emplace_back(std::move(temp));
      }

      return;
    }
    
    void phrase_str_to_cmd(string goal)
    {
      char str[goal.length()];
      strcpy(str, goal.c_str());
      std::vector<std::string> sv;
      std::string::size_type sz;     // alias of size_t

      printf ("Splitting string \"%s\" into tokens:\n",str);
      split(str,sv,',');
      for(int i=0;i<sv.size();i++)
        std::cout << sv[i] << std::endl;

      goal_cmd_.target_pose.pose.position.x = std::stod(sv[0],&sz);
      goal_cmd_.target_pose.pose.position.y = std::stod(sv[1],&sz);
      goal_cmd_.target_pose.pose.position.z = std::stod(sv[2],&sz);
      goal_cmd_.target_pose.pose.orientation.x = std::stod(sv[3],&sz);
      goal_cmd_.target_pose.pose.orientation.y = std::stod(sv[4],&sz);
      goal_cmd_.target_pose.pose.orientation.z = std::stod(sv[5],&sz);
      goal_cmd_.target_pose.pose.orientation.w = std::stod(sv[6],&sz);
    }

    bool accept_goal(string goal)
    {
      once_ = true;
      phrase_str_to_cmd(goal);
      return true;
    }

    bool do_something(string goal, string *feedback, string *result)
    {
      if(once_)
      {
        std::cout << "Current Goal: " << goal << std::endl;
        goal_cmd_.target_pose.header.frame_id = "map";
        goal_cmd_.target_pose.header.stamp = ros::Time::now();
        ROS_INFO("Sending goal...");
        movebase_ac_->sendGoal(goal_cmd_);
        once_ = false;
      }
      
      
      if(movebase_ac_->getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
      {
        *result = "Hooray! Reach GOAL!!!";
        return true;
      }else{
        *feedback = "Still heading forward Goal...";
        *result = *feedback;
        movebase_ac_->waitForResult(ros::Duration(0.1));
        return false;
      }
      
    }
private:
    string action_name_;
    bool once_;
    geometry_msgs::Pose goal_cmd_pose_;
    move_base_msgs::MoveBaseGoal goal_cmd_;
    MoveBaseClient *movebase_ac_;
};

int main(int argc, char ** argv)
{
    // ROS1
    ros::init(argc, argv, "ITRIQ600_ros1bridge");    

    // ROS2
    rclcpp::init(argc, argv);
    UniversalActionServer *uni_actionlib_server_ptr;
    ITRIQ600ROS1Bridge q600_ROS1_bridge_server("test_node","test");
    uni_actionlib_server_ptr = &q600_ROS1_bridge_server;
    uni_actionlib_server_ptr->init();
    
    // while (ros::ok())
    // {
    //   uni_actionlib_server_ptr->call_spin_once();
    // }
    rclcpp::shutdown();
    return 0;
}
