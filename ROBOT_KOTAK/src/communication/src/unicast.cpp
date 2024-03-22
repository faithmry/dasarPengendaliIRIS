#include <ros/ros.h>
#include <ros/package.h>

#include <iostream>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#include "std_msgs/Int16MultiArray.h"
#include "std_msgs/Float32MultiArray.h"

int fd;                      /* our socket */
struct sockaddr_in myaddr;   /* our address */
struct sockaddr_in dst_addr; /* remote address */
struct sockaddr_in stmaddr;  /* remote address */

socklen_t addr_len = sizeof(stmaddr); /* length of addresses */
socklen_t dst_len = sizeof(dst_addr);

char recv_buf[64];
char send_buf[64] = "its";

ros::Timer recv_timer;
ros::Timer send_timer;

uint8_t sensor_garis_buffer;
uint16_t sensor_bola_filter;
uint8_t tombol_buffer;
float factor;
float pos_x_buffer;
float pos_y_buffer;
float pos_th_buffer;
float epoch;

typedef struct
{
    float x;
    float y;
    float theta;
} pose2d_t;

typedef struct
{
    int8_t x;
    int8_t y;
    int8_t theta;
} vel2d_t;

typedef struct
{
    vel2d_t vel;
    pose2d_t pose_offset;
    uint8_t kick_mode;
    uint16_t kick_speed;
    uint8_t buzzer_count;
    uint8_t buzzer_time;
    int16_t dribble_right;
    int16_t dribble_left;
} stm_t;

stm_t stm_data;

ros::Publisher pub_stm_data_int;
ros::Publisher pub_stm_data_float;

void sub_stm_int(const std_msgs::Int16MultiArray::ConstPtr &msg)
{
    stm_data.vel.x = msg->data[0];
    stm_data.vel.y = msg->data[1];
    stm_data.vel.theta = msg->data[2];
    stm_data.buzzer_count = msg->data[3];
    stm_data.buzzer_time = msg->data[4];
    stm_data.dribble_left = msg->data[5];
    stm_data.dribble_right = msg->data[6];
    stm_data.kick_mode = msg->data[7];
    stm_data.kick_speed = msg->data[8];

    printf("\nvel x :%d vel y:%d vel th:%d\n", stm_data.vel.x, stm_data.vel.y, stm_data.vel.theta);
    printf("dri kn :%d dri kr:%d \n", stm_data.dribble_left, stm_data.dribble_right);
}

void sub_stm_float(const std_msgs::Float32MultiArray::ConstPtr &msg)
{
    stm_data.pose_offset.x = msg->data[0];
    stm_data.pose_offset.y = msg->data[1];
    stm_data.pose_offset.theta = msg->data[2];

    printf("of x :%f of y:%f of th:%f\n", stm_data.pose_offset.x, stm_data.pose_offset.y, stm_data.pose_offset.theta);
}

void timerSend(const ros::TimerEvent &event)
{
    stm_data.vel.x = 100;

    memcpy(send_buf + 3, &stm_data, sizeof(stm_t));
    sendto(fd, (void *)send_buf, sizeof(send_buf), 0, (struct sockaddr *)&dst_addr, dst_len);
}

void timerRecv(const ros::TimerEvent &event)
{
    int8_t recvlen = recvfrom(fd, recv_buf, 64, MSG_DONTWAIT, (struct sockaddr *)&stmaddr, &addr_len);

    if (recvlen > 0 && recv_buf[0] == 'i' && recv_buf[1] == 't' && recv_buf[2] == 's')
    {
        memcpy(&pos_x_buffer, recv_buf + 3, 4);
        memcpy(&pos_y_buffer, recv_buf + 7, 4);
        memcpy(&pos_th_buffer, recv_buf + 11, 4);
        memcpy(&sensor_garis_buffer, recv_buf + 15, 1);
        memcpy(&sensor_bola_filter, recv_buf + 16, 2);
        memcpy(&tombol_buffer, recv_buf + 18, 1);
        memcpy(&epoch, recv_buf + 19, 4);
    }
}

void transmitAll()
{
    std_msgs::Float32MultiArray msg_stm_float;
    std_msgs::Int16MultiArray msg_stm_int;

    msg_stm_int.data.push_back(sensor_garis_buffer);
    msg_stm_int.data.push_back(sensor_bola_filter);
    msg_stm_int.data.push_back(tombol_buffer);

    msg_stm_float.data.push_back(factor);
    msg_stm_float.data.push_back(pos_x_buffer);
    msg_stm_float.data.push_back(pos_y_buffer);
    msg_stm_float.data.push_back(pos_th_buffer);
    msg_stm_float.data.push_back(epoch);

    pub_stm_data_int.publish(msg_stm_int);
    pub_stm_data_float.publish(msg_stm_float);
}

int setup_addr()
{
    std::cout << "connecting" << std::endl;

    // Create UDP socket
    if ((fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
        perror("cannot create socket\n");
        return 0;
    }

    // dribble_kanan = -100-400*button_triangle;
    // dribble_kiri = -100-400*button_triangle;

    // Set PC conf
    bzero(&myaddr, sizeof(struct sockaddr_in));
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(8888);
    myaddr.sin_addr.s_addr = INADDR_ANY;

    // Set STM addr conf
    bzero(&dst_addr, sizeof(struct sockaddr_in));
    dst_addr.sin_family = AF_INET;
    dst_addr.sin_port = htons(44444);
    dst_addr.sin_addr.s_addr = inet_addr("192.168.10.159");

    if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0)
    {
        perror("bind failed");
        return 0;
    }

    return 1;
}

int main(int argc, char **argv)
{
    // Initialize the ROS node
    ros::init(argc, argv, "unicast");
    ros::NodeHandle nh;
    ros::MultiThreadedSpinner spinner(0);

    setup_addr();

    // ros::Subscriber sub_float = nh.subscribe("/master/stm/float", 1000, sub_stm_float);
    ros::Subscriber sub_int = nh.subscribe("/master/stm/int", 1, sub_stm_int);
    ros::Subscriber sub_float = nh.subscribe("/master/stm/float", 1, sub_stm_float);

    pub_stm_data_int = nh.advertise<std_msgs::Int16MultiArray>("/stm/master/int", 1);
    pub_stm_data_float = nh.advertise<std_msgs::Float32MultiArray>("/stm/master/float", 1);

    send_timer = nh.createTimer(ros::Duration(0.01), timerRecv);
    send_timer = nh.createTimer(ros::Duration(0.01), timerSend);

    // Set the loop rate (1 Hz)
    spinner.spin();

    return 0;
}
