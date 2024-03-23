#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <ros/ros.h>
#include <sensor_msgs/Joy.h>

using namespace std;
int sock;
struct sockaddr_in remote_addr;

float analog_l[2][2], analog_r[2][2], axes_l2, axes_r2;
int8_t button_kotak[2], button_lingkaran[2], button_silang[2], button_segitiga[2], button_r1[2], button_l1[2], button_up[2], button_down[2], button_left[2], button_right[2];

//const ros::TimerEvent &event;
char sendBuffer[128];

void JoyCllbckjs0(const sensor_msgs::Joy::ConstPtr& joy)
{
    analog_l[0][0] = joy->axes[0];
    analog_l[0][1] = joy->axes[1];
    analog_r[0][0] = joy->axes[3];
    analog_r[0][1] = joy->axes[4];
    axes_l2 = joy->axes[2];
    axes_r2 = joy->axes[5];

    button_silang[0] = joy->buttons[0];
    button_lingkaran[0] = joy->buttons[1];
    button_segitiga[0] = joy->buttons[2];
    button_kotak[0] = joy->buttons[3];
    button_l1[0] = joy->buttons[4];
    button_r1[0] = joy->buttons[5];

    button_up[0] = joy->axes[7];
    button_down[0] = joy->axes[7];
    button_left[0] = joy->axes[6];
    button_right[0] = joy->axes[6];

    // ROS_INFO("Silang %d Lingkaran %d Segitiga %d Kotak %d", button_silang[0], button_lingkaran[0], button_segitiga[0], button_kotak[0]);
    // ROS_INFO("L1 %d R1 %d L2 %d R2 %d", button_l1[0], button_r1[0], button_l2[0], button_r2[0]);
    // ROS_INFO("Naik %d Turun %d Kiri %d Kanan %d", button_up[0], button_down[0], button_left[0], button_right[0]);
    // ROS_INFO("Analog L %f %f", analog_l[0][0], analog_l[0][1]);
    // ROS_INFO("Analog R %f %f", analog_r[0][0], analog_r[0][1]);
}

void sendCllbck(const ros::TimerEvent& event){
    //
    memcpy(sendBuffer, &analog_l[0][0], 5);
    memcpy(sendBuffer + 5, &analog_l[0][1], 5);
    memcpy(sendBuffer + 10, &analog_r[0][0], 5);
    memcpy(sendBuffer + 15, &analog_r[0][1], 5);
    memcpy(sendBuffer + 20, &button_silang[0], 2);
    memcpy(sendBuffer + 22, &button_lingkaran[0], 2);
    memcpy(sendBuffer + 24, &button_segitiga[0], 2);
    memcpy(sendBuffer + 26, &button_kotak[0], 2);
    memcpy(sendBuffer + 28, &button_l1[0], 2);
    memcpy(sendBuffer + 30, &button_r1[0], 2);
    memcpy(sendBuffer + 36, &button_up[0], 2);
    memcpy(sendBuffer + 38, &button_down[0], 2);
    memcpy(sendBuffer + 40, &button_left[0], 2);
    memcpy(sendBuffer + 42, &button_right[0], 2);
    memcpy(sendBuffer + 44, &axes_l2, 5);
    memcpy(sendBuffer + 49, &axes_r2, 5);

    printf("Silang %d Lingkaran %d Segitiga %d Kotak %d\n", button_silang[0], button_lingkaran[0], button_segitiga[0], button_kotak[0]);
    printf("L1 %d R1 %d L2 %f R2 %f\n", button_l1[0], button_r1[0], axes_l2, axes_r2);
    printf("Naik %d Turun %d Kiri %d Kanan %d\n", button_up[0], button_down[0], button_left[0], button_right[0]);
    printf("Analog L %f %f\n", analog_l[0][0], analog_l[0][1]);
    printf("Analog R %f %f\n", analog_r[0][0], analog_r[0][1]);

    // std::string str = "Hello World";
    // sendto(sock, str.c_str(), str.size(), 0, (struct sockaddr*)&remote_addr, sizeof(remote_addr));

    sendto(sock, sendBuffer, sizeof(sendBuffer), 0, (struct sockaddr*)&remote_addr, sizeof(remote_addr));
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "controller");
    ros::NodeHandle nh;

    // UDP setup
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(12345);
    //inet_pton(AF_INET, "192.168.0.188", &remote_addr.sin_addr);
    inet_pton(AF_INET, "192.168.0.100", &remote_addr.sin_addr);

    // Joystick setup
         ros::Subscriber subjs0 = nh.subscribe("/js0/joy", 100, JoyCllbckjs0);

    //send udp
     ros::Timer timer = nh.createTimer(ros::Duration(0.001), sendCllbck);

    // Sending loop
    // ros::Rate loop_rate(10); // adjust as needed
    // while (ros::ok())
    // {
    //     // cout<<"hallo world"<<endl;
    //     // sendCllbck();
    //     ros::spinOnce();
    //     //loop_rate.sleep();
    // }
    ros::spin();

    // close(sock);

    return 0;
}
// #include <iostream>
// #include <cstring>
// #include <unistd.h>
// #include <arpa/inet.h>
// #include <ros/ros.h>
// #include <sensor_msgs/Joy.h>

// using namespace std;
// int sock;
// struct sockaddr_in multicast_addr;

// float analog_l[2][2], analog_r[2][2];
// int8_t button_kotak[2], button_lingkaran[2], button_silang[2], button_segitiga[2], button_r1[2], button_l1[2], button_up[2], button_down[2], button_left[2], button_right[2], button_r2[2], button_l2[2];

// char sendBuffer[128];

// void JoyCllbckjs0(const sensor_msgs::Joy::ConstPtr &joy)
// {
//     analog_l[0][0] = joy->axes[0];
//     analog_l[0][1] = joy->axes[1];
//     analog_r[0][0] = joy->axes[3];
//     analog_r[0][1] = joy->axes[4];

//     button_silang[0] = joy->buttons[0];
//     button_lingkaran[0] = joy->buttons[1];
//     button_segitiga[0] = joy->buttons[2];
//     button_kotak[0] = joy->buttons[3];
//     button_l1[0] = joy->buttons[4];
//     button_r1[0] = joy->buttons[5];
//     button_l2[0] = joy->buttons[6];
//     button_r2[0] = joy->buttons[7];

//     button_up[0] = joy->axes[7];
//     button_down[0] = joy->axes[7];
//     button_left[0] = joy->axes[6];
//     button_right[0] = joy->axes[6];

//     // ROS_INFO("Silang %d Lingkaran %d Segitiga %d Kotak %d", button_silang[0], button_lingkaran[0], button_segitiga[0], button_kotak[0]);
//     // ROS_INFO("L1 %d R1 %d L2 %d R2 %d", button_l1[0], button_r1[0], button_l2[0], button_r2[0]);
//     // ROS_INFO("Naik %d Turun %d Kiri %d Kanan %d", button_up[0], button_down[0], button_left[0], button_right[0]);
//     // ROS_INFO("Analog L %f %f", analog_l[0][0], analog_l[0][1]);
//     // ROS_INFO("Analog R %f %f", analog_r[0][0], analog_r[0][1]);
// }

// void sendCllbck(const ros::TimerEvent &event)
// {
//     memcpy(sendBuffer, &analog_l[0][0], 4);
//     memcpy(sendBuffer + 4, &analog_l[0][1], 4);
//     memcpy(sendBuffer + 8, &analog_r[0][0], 4);
//     memcpy(sendBuffer + 12, &analog_r[0][1], 4);
//     memcpy(sendBuffer + 13, &button_silang[0], 1);
//     memcpy(sendBuffer + 14, &button_lingkaran[0], 1);
//     memcpy(sendBuffer + 15, &button_segitiga[0], 1);
//     memcpy(sendBuffer + 16, &button_kotak[0], 1);
//     memcpy(sendBuffer + 17, &button_l1[0], 1);
//     memcpy(sendBuffer + 18, &button_r1[0], 1);
//     memcpy(sendBuffer + 19, &button_l2[0], 1);
//     memcpy(sendBuffer + 20, &button_r2[0], 1);
//     memcpy(sendBuffer + 21, &button_up[0], 1);
//     memcpy(sendBuffer + 22, &button_down[0], 1);
//     memcpy(sendBuffer + 23, &button_left[0], 1);
//     memcpy(sendBuffer + 24, &button_right[0], 1);


//     printf("Silang %d Lingkaran %d Segitiga %d Kotak %d\n", button_silang[0], button_lingkaran[0], button_segitiga[0], button_kotak[0]);
//     printf("L1 %d R1 %d L2 %d R2 %d\n", button_l1[0], button_r1[0], button_l2[0], button_r2[0]);
//     printf("Naik %d Turun %d Kiri %d Kanan %d\n", button_up[0], button_down[0], button_left[0], button_right[0]);
//     printf("Analog L %f %f\n", analog_l[0][0], analog_l[0][1]);
//     printf("Analog R %f %f\n", analog_r[0][0], analog_r[0][1]);
// }

// int main(int argc, char **argv)
// {
//     ros::init(argc, argv, "controller");
//     ros::NodeHandle nh;

//     // UDP setup
//     sock = socket(AF_INET, SOCK_DGRAM, 0);
//     if (sock < 0)
//     {
//         std::cerr << "Error creating socket" << std::endl;
//         return 1;
//     }

//     // Setup multicast address and port
//     memset(&multicast_addr, 0, sizeof(multicast_addr));
//     multicast_addr.sin_family = AF_INET;
//     multicast_addr.sin_addr.s_addr = inet_addr("224.0.0.1"); // Multicast group address
//     multicast_addr.sin_port = htons(12345);                  // Multicast group port

//     // Enable multicast on socket
//     int enable = 1;
//     if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0)
//     {
//         std::cerr << "setsockopt(SO_REUSEADDR) failed" << std::endl;
//         return 1;
//     }

//     // Bind the socket to the multicast address
//     if (bind(sock, (struct sockaddr *)&multicast_addr, sizeof(multicast_addr)) < 0)
//     {
//         std::cerr << "bind() failed" << std::endl;
//         return 1;
//     }

//     // Joystick setup
//     ros::Subscriber subjs0 = nh.subscribe("/js0/joy", 100, JoyCllbckjs0);

//     // Setup send timer
//     ros::Timer timer = nh.createTimer(ros::Duration(0.001), sendCllbck);

//     ros::spin();

//     return 0;
// }
