#include <iostream>
#include <string>

#include <chrono>
#include <thread>

#include <signal.h>

#include "ros/ros.h"
#include "std_msgs/String.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define  BUFF_SIZE   35

int cvtnum(char* a)
{
        char* b;
        int index = 0;
        for (int x = 0; x<4; x++)
        {
                if (a[x] == -1)
                {
                        index = x - 1;
                        break;
                }
        }
        b = new char[index + 1];
        for (int x = 0; x<index; x++)
        {
                b[x] = a[x];
        }
        return atoi(b);

}
// Ctrl+C를 누르면 이 함수가 호출 됨
void cbSigintHandler(int sig)
{
        // node의 ros를 종료 함
        ros::shutdown();
}

int main(int argc, char **argv)
{
        char   buff_rcv[BUFF_SIZE + 5];
        int   server_socket;
        int   client_socket;
        socklen_t   client_addr_size;
        char* b;
        int index = 0;

struct sockaddr_in   server_addr;
struct sockaddr_in   client_addr;
std::string my_name = "controller_node";

for (int x = 0; x<BUFF_SIZE + 5; x++)
{
        buff_rcv[x] = -1;
}
server_socket = socket(PF_INET, SOCK_STREAM, 0);
if (-1 == server_socket)
{
        printf("server socket 생성 실패\n");
        exit(1);
}

memset(&server_addr, 0, sizeof(server_addr));
server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(2150);
server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
if (-1 == bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)))
{
        printf("bind() 실행 에러\n");
        exit(1);
}

if (-1 == listen(server_socket, 5))
{
        printf("listen() 실행 실패\n");
        exit(1);
}

// All node must call this function to communicate with the roscore.
ros::init(argc, argv, my_name);

// ctrl+C를 눌렀을 때 프로그램을 종료하도록 이벤트 처리
signal(SIGINT, cbSigintHandler);

// 'chatter' 토픽에 대해 subscribe하고, publish 할 수 있도록 설정
ros::NodeHandle n;

ros::Publisher  pub_moveX = n.advertise<std_msgs::String>("mansoo_drone/x_pos", 10);
ros::Publisher  pub_moveY = n.advertise<std_msgs::String>("mansoo_drone/y_pos", 10);
ros::Publisher  pub_moveX2 = n.advertise<std_msgs::String>("mansoo_drone/x_pos2", 10);
ros::Publisher  pub_moveY2 = n.advertise<std_msgs::String>("mansoo_drone/y_pos2", 10);
ros::Publisher  pub_moveZ = n.advertise<std_msgs::String>("mansoo_drone/z_pos", 10);
ros::Publisher  pub_moveZ2 = n.advertise<std_msgs::String>("mansoo_drone/z_pos2", 10);
ros::Publisher  pub_isFinished = n.advertise<std_msgs::String>("mansoo_drone/isFinished", 10);
ros::Publisher  pub_isBacked = n.advertise<std_msgs::String>("mansoo_drone/isBacked", 10);
ros::Publisher  pub_clock = n.advertise<std_msgs::String>("mansoo_drone/clock", 10);
ros::Publisher  pub_counterClock = n.advertise<std_msgs::String>("mansoo_drone/counterClock", 10);
std_msgs::String pub_msg;

// 콘솔 입력할 때 ros로부터 수신되는 이벤트 처리가 안되기 때문에 ros의 이벤트 확인은 thread 처리로 변경
std::thread t([]() {
        ros::AsyncSpinner spinner(1); // Use 4 threads
        spinner.start();
        ros::waitForShutdown();
});

while (ros::ok())
{
        std::string in_msg;
        client_addr_size = sizeof(client_addr);
        std::cout<<"connection waitng....."<<std::endl;
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, (socklen_t *)&client_addr_size);
        std::cout<<"connection success"<<std::endl;
        if (-1 == client_socket)
        {
                printf("클라이언트 연결 수락 실패\n");
                exit(1);
        }
        read(client_socket, buff_rcv, BUFF_SIZE);
        printf("receive: %s\n", buff_rcv);
        for (int x = 1; x<4; x++)
        {
                if (buff_rcv[x]<1)
                {
                        index = x;
                        break;
                }
        }
        for (int y = 1; y<index; y++)
        {
                in_msg.push_back(buff_rcv[y]);
        }
        if (buff_rcv[0] >= 0)
        {

                if (buff_rcv[1]>0)
                {
                        if (buff_rcv[0] == 'n') //forward
                        {
                                pub_msg.data = in_msg;
                                pub_moveX.publish(pub_msg);
                                ROS_INFO("*** move forward ***");
                        }
                        if (buff_rcv[0] == 's') //back
                        {
                                pub_msg.data = in_msg;
                                pub_moveX2.publish(pub_msg);
                                ROS_INFO("*** move back ***");
                        }
                        if (buff_rcv[0] == 'l') //left
                        {
                                pub_msg.data = in_msg;
                                pub_moveY.publish(pub_msg);
                                ROS_INFO("*** move left ***");
                        }
                        if (buff_rcv[0] == 'r') //right
                        {
                                pub_msg.data = in_msg;
                                pub_moveY2.publish(pub_msg);
                                ROS_INFO("*** move right ***");
                        }
                        if (buff_rcv[0] == 'u') //up

                        {

                                pub_msg.data = in_msg;        

                                pub_moveZ.publish(pub_msg);

                                ROS_INFO("*** move up ***");

                        }

                        if (buff_rcv[0] == 'd') //down

                        { 

                                pub_msg.data = in_msg;        

                                pub_moveZ2.publish(pub_msg);

                                ROS_INFO("*** move down ***");

                        }
                }
                if (buff_rcv[0] == 'q') //right
                {
                        break;
                        ROS_INFO("*** out***");
                }
                else
                {
                        if (buff_rcv[0] == 'f')
                        {

                                pub_msg.data = buff_rcv[0];
                                pub_isFinished.publish(pub_msg);
                                ROS_INFO("****** finish ******");
                        }
                        if (buff_rcv[0] == 'b')
                        {

                                pub_msg.data = buff_rcv[0];
                                pub_isBacked.publish(pub_msg);
                                ROS_INFO("****** back ******");
                        }
                        if (buff_rcv[0] == 'c') //clockwise
                        {

                                pub_msg.data = buff_rcv[0];
                                pub_clock.publish(pub_msg);
                                ROS_INFO("****** clock_wise rotation ******");
                        }
                        if (buff_rcv[0] == 'w') //counterclockwise
                        {

                                pub_msg.data = buff_rcv[0];
                                pub_counterClock.publish(pub_msg);
                                ROS_INFO("****** counter_clock_wise rotation ******");
                        }
                }



        }
        close(client_socket);
}

ros::shutdown();

t.join();

return 0;
}
