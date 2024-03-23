#include <ros/ros.h>
#include <ros/package.h>

#include <iostream>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#include "std_msgs/Int16MultiArray.h"
#include "std_msgs/Float32MultiArray.h"

//

#define MAX_DATA_SIZE 128
#define MULTICAST_IP "224.0.0.1"
#define PORT 54321

ros::Timer recv_timer;
ros::Publisher pub_int_data;
ros::Publisher pub_float_data;

float analog_l[2][2], analog_r[2][2], axes_l2, axes_r2;

int8_t button_kotak[2], button_lingkaran[2],
    button_silang[2], button_segitiga[2],
    button_r1[2], button_l1[2], button_up[2],
    button_down[2], button_left[2], button_right[2],
    button_r2[2], button_l2[2];

int sock;

void closeSocket()
{
    ROS_INFO("close socket");

    if (sock != -1)
        shutdown(sock, SHUT_RDWR);
}

int openSocket()
{
    ROS_INFO("open socket");

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        std::cerr << "Error creating socket" << std::endl;
        return -1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        std::cerr << "Error binding socket" << std::endl;
        return -1;
    }

    return 1;
}

void recv_cllbck(const ros::TimerEvent &)
{
    // ROS_INFO("tes");

    char recv_buf[128];
    // /float dst;
    socklen_t len;
    struct sockaddr_in sender_addr;
    int received = recvfrom(sock, recv_buf, sizeof(recv_buf) - 1, 0, (struct sockaddr *)&sender_addr, &len);

    std::cout << recv_buf << std::endl;
    // memcpy(&dst, recv_buf, strlen(recv_buf) + 1);
    // memcpy(&dst, recv_buf, strlen(recv_buf) + 1);

    memcpy(&analog_l[0][0], recv_buf, 5);
    memcpy(&analog_l[0][1], recv_buf + 5, 5);
    memcpy(&analog_r[0][0], recv_buf + 10, 5);
    memcpy(&analog_r[0][1], recv_buf + 15, 5);

    memcpy(&button_silang[0], recv_buf + 20, 2);
    memcpy(&button_lingkaran[0], recv_buf + 22, 2);
    memcpy(&button_segitiga[0], recv_buf + 24, 2);
    memcpy(&button_kotak[0], recv_buf + 26, 2);
    memcpy(&button_l1[0], recv_buf + 28, 2);
    memcpy(&button_r1[0], recv_buf + 30, 2);
    memcpy(&button_l2[0], recv_buf + 32, 2);
    memcpy(&button_r2[0], recv_buf + 34, 2);
    memcpy(&button_up[0], recv_buf + 36, 2);
    memcpy(&button_down[0], recv_buf + 38, 2);
    memcpy(&button_left[0], recv_buf + 40, 2);
    memcpy(&button_right[0], recv_buf + 42, 2);
    memcpy(&axes_l2, recv_buf + 44, 5);
    memcpy(&axes_r2, recv_buf + 49, 5);

    std::cout << "Silang " << static_cast<int>(button_silang[0]) << " Lingkaran " << static_cast<int>(button_lingkaran[0]) << " Segitiga " << static_cast<int>(button_segitiga[0]) << " Kotak " << static_cast<int>(button_kotak[0]) << std::endl;
    std::cout << "L1 " << static_cast<int>(button_l1[0]) << " R1 " << static_cast<int>(button_r1[0]) << " L2 " << static_cast<int>(button_l2[0]) << " R2 " << static_cast<int>(button_r2[0]) << std::endl;
    std::cout << "Naik " << static_cast<int>(button_up[0]) << " Turun " << static_cast<int>(button_down[0]) << " Kiri " << static_cast<int>(button_left[0]) << " Kanan " << static_cast<int>(button_right[0]) << std::endl;
    std::cout << "Analog L " << analog_l[0][0] << " " << analog_l[0][1] << std::endl;
    std::cout << "Analog R " << analog_r[0][0] << " " << analog_r[0][1] << std::endl;

    std_msgs::Float32MultiArray float_array;

    float_array.data.push_back(analog_l[0][0]);
    float_array.data.push_back(analog_l[0][1]);
    float_array.data.push_back(analog_r[0][0]);
    float_array.data.push_back(analog_r[0][1]);
    float_array.data.push_back(axes_l2);
    float_array.data.push_back(axes_r2);

    std_msgs::Int16MultiArray int_array;

    int_array.data.push_back(button_kotak[0]);
    int_array.data.push_back(button_silang[0]);
    int_array.data.push_back(button_lingkaran[0]);
    int_array.data.push_back(button_segitiga[0]);
    int_array.data.push_back(button_r1[0]);
    int_array.data.push_back(button_l1[0]);
    int_array.data.push_back(button_r2[0]);
    int_array.data.push_back(button_l2[0]);
    int_array.data.push_back(button_up[0]);
    int_array.data.push_back(button_down[0]);
    int_array.data.push_back(button_left[0]);
    int_array.data.push_back(button_right[0]);

    pub_int_data.publish(int_array);
    pub_float_data.publish(float_array);
}

// void publish(const ros::TimerEvent &)
// {
//     ROS_INFO("pub");
//
//     button_silang[0] = 12;
//     button_kotak[0] = 32;
//     button_segitiga[0] = 8;
//     button_lingkaran[0] = 5;
//
//     std_msgs::Int16MultiArray array_msg;
//
//     array_msg.data.resize(4); // Example: 5 elements
//     ROS_INFO("tes7");
//
//     array_msg.data[0] = static_cast<int>(button_silang[0]);
//     array_msg.data[1] = static_cast<int>(button_lingkaran[0]);
//     array_msg.data[2] = static_cast<int>(button_kotak[0]);
//     array_msg.data[3] = static_cast<int>(button_segitiga[0]);
//
//     pub_all_data.publish(array_msg);
// }

int main(int argc, char **argv)
{
    ros::init(argc, argv, "multicast");
    ros::NodeHandle nh;
    ros::MultiThreadedSpinner spinner(0);

    closeSocket();

    if (openSocket() == -1)
    {
        return -1;
    }

    pub_int_data = nh.advertise<std_msgs::Int16MultiArray>("/joystick/int", 100);
    pub_float_data = nh.advertise<std_msgs::Float32MultiArray>("/joystick/float", 100);

    recv_timer = nh.createTimer(ros::Duration(0.01), recv_cllbck);
    // recv_timer = nh.createTimer(ros::Duration(1), publish);

    spinner.spin();
    recv_timer.stop();

    return 0;
}
