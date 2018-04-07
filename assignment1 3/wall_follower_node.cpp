#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/Twist.h>
#include <sstream>
#include <cmath>
#include "std_msgs/Float64.h"
#include <wall_following_assignment/pid.h>
#include <vector>
#include <algorithm>
#include <dynamic_reconfigure/server.h>
#include <wall_following_assignment/controlConfig.h>


using namespace std;


ros::Publisher cmd_pub;
ros::Publisher cte_pub;
double dt = 0.02;
PID *pid = new PID(4, 3.5, 18, dt);

double desired_distance_from_wall = 1.0; // in meters
double forward_speed = 1.0;              // in meters / sec


void callback(wall_following_assignment::controlConfig &config, uint32_t level) {
    
    pid = new PID(config.Kp, config.Td, config.Ti, dt);


}




void laser_scan_callback(const sensor_msgs::LaserScan::ConstPtr& msg) {
  geometry_msgs::Twist cmd;
  cmd.linear.x = forward_speed;  // forward speed is fixed
  


  //Fetch the minimum distance from ranges
  double min = 9999999;
  for (int i = 0; i < msg->ranges.size(); i++){
      if (msg->ranges[i] < min){
          min = msg->ranges[i];
      }
  }


  
  //calculate the cross-track error
  double cte = min - desired_distance_from_wall;
  //declare message and store cte as its data
  std_msgs::Float64 message; 
  message.data = cte;
  //publish the data
  cte_pub.publish(message);
  
  // Populate this command based on the distance to the closest
  // object in laser scan. I.e. compute the cross-track error
  // as mentioned in the PID slides.

  // You can populate the command based on either of the following two methods:
  // (1) using only the distance to the closest wall
  // (2) using the distance to the closest wall and the orientation of the wall
  //
  // If you select option 2, you might want to use cascading PID control. 
  
  // cmd.angular.z = ???
  //
  // update control
  pid->update_control(cte);
  // set angular.z according to pid control
  cmd.angular.z = pid->get_control();
  // publish command
  cmd_pub.publish(cmd);
  


}


int main(int argc, char **argv) {
  ros::init(argc, argv, "wall_follower_node");
  ros::NodeHandle nh;

  // Getting params before setting up the topic subscribers
  // otherwise the callback might get executed with default
  // wall following parameters
  nh.getParam("forward_speed", forward_speed);
  nh.getParam("desired_distance_from_wall", desired_distance_from_wall);


  cte_pub = nh.advertise<std_msgs::Float64>("/husky_1/cte", 10);
  



  // todo: set up the command publisher to publish at topic '/husky_1/cmd_vel'
  // using geometry_msgs::Twist messages
  cmd_pub = nh.advertise<geometry_msgs::Twist>("/husky_1/cmd_vel", 10);
  
  // todo: set up the laser scan subscriber
  // this will set up a callback function that gets executed
  // upon each spinOnce() call, as long as a laser scan
  // message has been published in the meantime by another node
  ros::Subscriber laser_sub = nh.subscribe("/husky_1/scan", 10, laser_scan_callback);
 


  
  dynamic_reconfigure::Server<wall_following_assignment::controlConfig> server;
  dynamic_reconfigure::Server<wall_following_assignment::controlConfig>::CallbackType f;

  f = boost::bind(&callback, _1, _2);
  server.setCallback(f);





  ros::Rate rate(50);
  // this will return false on ctrl-c or when you call ros::shutdown()
  while (ros::ok()) {
    ros::spinOnce();
    rate.sleep();
  }
  
  return 0;
}
   
