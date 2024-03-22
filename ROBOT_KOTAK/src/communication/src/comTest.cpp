#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int16MultiArray.h"

void arrayCallback(const std_msgs::Int16MultiArray::ConstPtr &msg)
{
    // Print the received array data
    ROS_INFO("Received Int16MultiArray message with %lu elements:", msg->data.size());

    for (size_t i = 0; i < msg->data.size(); ++i)
    {
        ROS_INFO("Element %zu: %d", i, msg->data[i]);
    }
}
int main(int argc, char **argv)
{
    // Initialize the ROS node
    ros::init(argc, argv, "publisher_subscriber_node");

    // Create a node handle
    ros::NodeHandle nh;

    // Create a publisher with topic name "chat"
    ros::Publisher pub = nh.advertise<std_msgs::Int16MultiArray>("/abcd", 1000);

    // Create a subscriber with topic name "chat" and callback function
    ros::Subscriber sub = nh.subscribe("/abcd", 1000, arrayCallback);

    // Set the loop rate (1 Hz)
    ros::Rate loop_rate(1);

    int count = 0;
    while (ros::ok())
    {
        // Create a string message
        std_msgs::Int16MultiArray array_msg;

        array_msg.data.resize(5); // Example: 5 elements

        // Populate the array with some data
        for (size_t i = 0; i < array_msg.data.size(); ++i)
        {
            array_msg.data[i] = i * 10; // Example: Increment by 10
        }

        pub.publish(array_msg);

        // Spin once to let the callback functions execute
        ros::spinOnce();

        // Sleep to maintain the loop rate
        loop_rate.sleep();
    }

    return 0;
}
