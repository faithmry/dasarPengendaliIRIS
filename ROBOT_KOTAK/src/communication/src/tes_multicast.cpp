#include <ros/ros.h>
#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/Int16MultiArray.h"
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>

#define MULTICAST_IP "224.0.0.1"
#define PORT 54321
#define MAX_DATA_SIZE 128

ros::Publisher pub_int;
ros::Publisher pub_float;

float analog_l[2][2], analog_r[2][2];
int8_t button_kotak[2], button_lingkaran[2], button_silang[2], button_segitiga[2], button_r1[2], button_l1[2], button_up[2], button_down[2], button_left[2], button_right[2], button_r2[2], button_l2[2];

void receiver()
{
    int sock;
    struct sockaddr_in local_addr;
    struct ip_mreq mreq;
    char buffer[MAX_DATA_SIZE];
    ssize_t received;

    // Create a UDP socket
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        ROS_ERROR("Failed to create socket");
        return;
    }

    // Set up local address
    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY); // Receive on any interface
    local_addr.sin_port = htons(PORT);

    // Bind the socket to the local address
    if (bind(sock, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0)
    {
        ROS_ERROR("Failed to bind socket");
        close(sock);
        return;
    }

    // Set up multicast group
    mreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_IP);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);

    if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq, sizeof(mreq)) < 0)
    {
        ROS_ERROR("Failed to join multicast group");
        close(sock);
        return;
    }

    // Receive data
    while (ros::ok())
    {
        received = recv(sock, buffer, sizeof(buffer), 0);
        if (received < 0)
        {
            ROS_ERROR("Error receiving data");
            continue;
        }

        memcpy(&analog_l[0][0], buffer, 4);
        memcpy(&analog_l[0][1], buffer + 4, 4);
        memcpy(&analog_r[0][0], buffer + 8, 4);
        memcpy(&analog_r[0][1], buffer + 12, 4);
        memcpy(&button_silang[0], buffer + 13, 1);
        memcpy(&button_lingkaran[0], buffer + 14, 1);
        memcpy(&button_segitiga[0], buffer + 15, 1);
        memcpy(&button_kotak[0], buffer + 16, 1);
        memcpy(&button_l1[0], buffer + 17, 1);
        memcpy(&button_r1[0], buffer + 18, 1);
        memcpy(&button_l2[0], buffer + 19, 1);
        memcpy(&button_r2[0], buffer + 20, 1);
        memcpy(&button_up[0], buffer + 21, 1);
        memcpy(&button_down[0], buffer + 22, 1);
        memcpy(&button_left[0], buffer + 23, 1);
        memcpy(&button_right[0], buffer + 24, 1);

        std::cout << "Silang " << static_cast<int>(button_silang[0]) << " Lingkaran " << static_cast<int>(button_lingkaran[0]) << " Segitiga " << static_cast<int>(button_segitiga[0]) << " Kotak " << static_cast<int>(button_kotak[0]) << std::endl;
        std::cout << "L1 " << static_cast<int>(button_l1[0]) << " R1 " << static_cast<int>(button_r1[0]) << " L2 " << static_cast<int>(button_l2[0]) << " R2 " << static_cast<int>(button_r2[0]) << std::endl;
        std::cout << "Naik " << static_cast<int>(button_up[0]) << " Turun " << static_cast<int>(button_down[0]) << " Kiri " << static_cast<int>(button_left[0]) << " Kanan " << static_cast<int>(button_right[0]) << std::endl;
        std::cout << "Analog L " << analog_l[0][0] << " " << analog_l[0][1] << std::endl;
        std::cout << "Analog R " << analog_r[0][0] << " " << analog_r[0][1] << std::endl;

        // Process received data if necessary
        std_msgs::Int16MultiArray msg_int;
        std_msgs::Float32MultiArray msg_float;

        msg_int.data.push_back(button_kotak[0]);
        msg_int.data.push_back(button_silang[0]);
        msg_int.data.push_back(button_lingkaran[0]);
        msg_int.data.push_back(button_segitiga[0]);
        msg_int.data.push_back(button_r1[0]);
        msg_int.data.push_back(button_l1[0]);
        msg_int.data.push_back(button_r2[0]);
        msg_int.data.push_back(button_l2[0]);
        msg_int.data.push_back(button_up[0]);
        msg_int.data.push_back(button_down[0]);
        msg_int.data.push_back(button_left[0]);
        msg_int.data.push_back(button_right[0]);

        msg_float.data.push_back(analog_l[0][0]);
        msg_float.data.push_back(analog_l[0][1]);
        msg_float.data.push_back(analog_r[0][0]);
        msg_float.data.push_back(analog_r[0][1]);
        // Assuming the received data is in a specific format (e.g., Int16MultiArray)
        // Parse the data into the message here
        // Example: msg.data = {data1, data2, ...};

        pub_int.publish(msg_int);
        pub_float.publish(msg_float);
    }

    // Close the socket when done
    close(sock);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "udp_multicast_receiver");
    ros::NodeHandle nh;

    // Initialize the publisher
    pub_int = nh.advertise<std_msgs::Int16MultiArray>("/joystick/int", 10);
    pub_float = nh.advertise<std_msgs::Float32MultiArray>("/joystick/float", 10);

    // Start the receiver thread
    std::thread receiver_thread(receiver);

    ros::spin();

    // Join the receiver thread before exiting
    receiver_thread.join();

    return 0;
}
