
#include<iostream>

#include <ros/ros.h>
#include <std_msgs/String.h>
#include <geometry_msgs/PoseStamped.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
#include <mavros_msgs/PositionTarget.h>


mavros_msgs::State              g_current_state;
geometry_msgs::PoseStamped     g_pose;
mavros_msgs::PositionTarget     moveMsg;

int pos_x=0;

int pos_y=0;

int pos_z=3;

int pos_x_back=0;

int pos_y_back=0;

int pos_z_back=3;

double yaw = 1;


void cbState(const mavros_msgs::State::ConstPtr& msg)
{
    g_current_state = *msg;

}



void cbMoveX(const std_msgs::String::ConstPtr& msg) //forward

{


 

  std::string::size_type sz;   // alias of size_t

 

  int pos = std::stoi (msg->data, &sz);

 

  pos_x_back=pos_x;

 

  pos_y_back=pos_y;

 

  pos_z_back=pos_z;

 

  moveMsg.position.x = pos_x+pos;

  

  pos_x=moveMsg.position.x;

}

 

void cbMoveY(const std_msgs::String::ConstPtr& msg) //left

{

  std::string::size_type sz;   // alias of size_t

  

  int pos = std::stoi (msg->data, &sz);

 

  pos_x_back=pos_x;

 

  pos_y_back=pos_y;

 

  pos_z_back=pos_z;

 

  moveMsg.position.y = pos_y+pos;

 

  pos_y=moveMsg.position.y;

}

 

void cbMoveX2(const std_msgs::String::ConstPtr& msg) //backward

{

  std::string::size_type sz;   // alias of size_t

 

  int pos = std::stoi (msg->data, &sz);

 

  pos_x_back=pos_x;

 

  pos_y_back=pos_y;

 

  pos_z_back=pos_z;


  moveMsg.position.x = pos_x-pos;


  pos_x=moveMsg.position.x;

}

 

void cbMoveY2(const std_msgs::String::ConstPtr& msg) //right

{

  std::string::size_type sz;   // alias of size_t

 

  int pos = std::stoi (msg->data, &sz);

 

  pos_x_back=pos_x;

 

  pos_y_back=pos_y;

 

  pos_z_back=pos_z;

 

  moveMsg.position.y = pos_y-pos;

 

  pos_y=moveMsg.position.y;

}

 

void cbMoveZ(const std_msgs::String::ConstPtr& msg) //up

{

  std::string::size_type sz;   // alias of size_t

 

  int pos = std::stoi (msg->data, &sz);

 

  pos_x_back=pos_x;

 

  pos_y_back=pos_y;

 

  pos_z_back=pos_z;

 

  moveMsg.position.z = pos_z+pos;

 

  pos_z=moveMsg.position.z;

}

 

void cbMoveZ2(const std_msgs::String::ConstPtr& msg) //down

{

  std::string::size_type sz;   // alias of size_t

 

  int pos = std::stoi (msg->data, &sz);

 

  if(pos<pos_z){

 

    pos_x_back=pos_x;

 

    pos_y_back=pos_y;

 

    pos_z_back=pos_z;

 

    moveMsg.position.z = pos_z-pos;

 

    pos_z=moveMsg.position.z;

 

  }

 

}

 

void isFinished(const std_msgs::String::ConstPtr& msg)

{

  

  moveMsg.position.x = 0;

 

  moveMsg.position.y = 0;

 

  moveMsg.position.z = 3;

 

  pos_x=moveMsg.position.x;

 

  pos_y=moveMsg.position.y;

 

  pos_z=moveMsg.position.z;

}

 

void isBacked(const std_msgs::String::ConstPtr& msg)

{

  

  moveMsg.position.x = pos_x_back;

 

  moveMsg.position.y = pos_y_back;

 

  moveMsg.position.z = pos_z_back;

 

  pos_x=moveMsg.position.x;

 

  pos_y=moveMsg.position.y;

 

  pos_z=moveMsg.position.z;

 
}

void clockwise(const std_msgs::String::ConstPtr& msg){
 
  yaw = yaw - 90;
  moveMsg.yaw = yaw * 0.0174532925;
}
void counterclockwise(const std_msgs::String::ConstPtr& msg){

  yaw = yaw + 90;
  moveMsg.yaw = yaw * 0.0174532925;

}
 

int main(int argc, char **argv)
{
    ros::init(argc, argv, "mansoo_offboard_node");
    ros::NodeHandle nh;

    ros::Subscriber     state_sub        = nh.subscribe<mavros_msgs::State>("mavros/state", 10, cbState);

    ros::Subscriber     move_x_sub        = nh.subscribe("mansoo_drone/x_pos", 10, cbMoveX);
    ros::Subscriber     move_y_sub        = nh.subscribe("mansoo_drone/y_pos", 10, cbMoveY);
    ros::Subscriber     move_x2_sub        = nh.subscribe("mansoo_drone/x_pos2", 10, cbMoveX2);
    ros::Subscriber     move_y2_sub        = nh.subscribe("mansoo_drone/y_pos2", 10, cbMoveY2);
    ros::Subscriber     move_z_sub        = nh.subscribe("mansoo_drone/z_pos", 10, cbMoveZ);
    ros::Subscriber     move_z2_sub        = nh.subscribe("mansoo_drone/z_pos2", 10, cbMoveZ2);
    ros::Subscriber     isFinished_sub        = nh.subscribe("mansoo_drone/isFinished", 10, isFinished);
    ros::Subscriber     isBacked_sub        = nh.subscribe("mansoo_drone/isBacked", 10, isBacked);
    ros::Subscriber     clock_sub        = nh.subscribe("mansoo_drone/clock", 10, clockwise);
    ros::Subscriber     counterClock_sub        = nh.subscribe("mansoo_drone/counterClock", 10, counterclockwise);

    ros::Publisher      local_pos_pub    = nh.advertise<geometry_msgs::PoseStamped>("mavros/setpoint_position/local", 10);
    ros::ServiceClient  arming_client   = nh.serviceClient<mavros_msgs::CommandBool>("mavros/cmd/arming");
    ros::ServiceClient  set_mode_client = nh.serviceClient<mavros_msgs::SetMode>("mavros/set_mode");


    ros::Publisher      _xyzPub = nh.advertise<mavros_msgs::PositionTarget>("mavros/setpoint_raw/local", 10);


    //the setpoint publishing rate MUST be faster than 2Hz
    ros::Rate rate(20.0);

    // wait for FCU connection
    while(ros::ok() && g_current_state.connected){
        ros::spinOnce();
        rate.sleep();
    }

    moveMsg.coordinate_frame = mavros_msgs::PositionTarget::
                                                FRAME_BODY_OFFSET_NED;
    moveMsg.type_mask =mavros_msgs::PositionTarget::IGNORE_AFX |
                                     mavros_msgs::PositionTarget::IGNORE_AFY |
                                     mavros_msgs::PositionTarget::IGNORE_AFZ |
                                     mavros_msgs::PositionTarget::IGNORE_VX  |
                                     mavros_msgs::PositionTarget::IGNORE_VY  |
                                     mavros_msgs::PositionTarget::IGNORE_VZ ;

    moveMsg.header.stamp = ros::Time::now();
    moveMsg.position.x = 0;
    moveMsg.position.y = 0;
    moveMsg.position.z = 3;
    moveMsg.yaw = yaw * 0.0174532925; //DEG to RAD
    moveMsg.yaw_rate = 1;


    //send a few setpoints before starting
    for(int i = 100; ros::ok() && i > 0; --i)
    {
        _xyzPub.publish(moveMsg);
        //local_pos_pub.publish(g_pose);
        ros::spinOnce();
        rate.sleep();
    }

    mavros_msgs::SetMode offb_set_mode;
    offb_set_mode.request.custom_mode = "OFFBOARD";

    mavros_msgs::CommandBool arm_cmd;
    arm_cmd.request.value = true;

    ros::Time last_request = ros::Time::now();

    while(ros::ok())
    {
        if( g_current_state.mode != "OFFBOARD" &&
            (ros::Time::now() - last_request > ros::Duration(5.0)))
        {
            if( set_mode_client.call(offb_set_mode) &&
                offb_set_mode.response.mode_sent)
            {
                ROS_INFO("Offboard enabled");
            }

            last_request = ros::Time::now();
        }
        else
        {
            if( !g_current_state.armed &&
                (ros::Time::now() - last_request > ros::Duration(5.0)))
            {
                if( arming_client.call(arm_cmd) &&
                    arm_cmd.response.success)
                {
                    ROS_INFO("Vehicle armed");
                }

                last_request = ros::Time::now();
            }
        }

        _xyzPub.publish(moveMsg);
        //local_pos_pub.publish(g_pose);

        ros::spinOnce();
        rate.sleep();
    }

    return 0;
}
