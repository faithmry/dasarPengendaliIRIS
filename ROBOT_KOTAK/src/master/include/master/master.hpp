#ifndef MASTER_HPP
#define MASTER_HPP

#define DEG2RAD 0.017452925
#define RAD2DEG 57.295780

//--ROS Header
#include <ros/ros.h>

//--ROS Messages
#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/Int16MultiArray.h"

//--Subscriber
ros::Subscriber sub_data_int;
ros::Subscriber sub_data_float;

ros::Subscriber sub_stm_data_int;
ros::Subscriber sub_stm_data_float;

//--Publisher
ros::Publisher pub_stm_int;
ros::Publisher pub_stm_float;

//--Timer
ros::Timer timer_master;

//--Prototypes
void timerCallback(const ros::TimerEvent &);
void subDataIntCallback(const std_msgs::Int16MultiArray::ConstPtr &);
void subDataFloatCallback(const std_msgs::Float32MultiArray::ConstPtr &);

void setOffset(float, float, float);
void setFinalPose();
void kick(int8_t, uint16_t);
void setVelocityLap(int8_t, int8_t, int);
void setVelocity(int8_t, int8_t, int);

void subStmDataIntCallback(const std_msgs::Int16MultiArray::ConstPtr &);
void subStmDataFloatCallback(const std_msgs::Float32MultiArray::ConstPtr &);
void transmitAll();

//--Data Stick
float analog_l[2],
    analog_r[2],
    axes_l2,
    axes_r2;

int button_square,
    button_x,
    button_o,
    button_triangle,
    button_r1,
    button_r2,
    button_l1,
    button_l2,
    button_ps,
    button_up,
    button_down,
    button_left,
    button_right,
    button_select,
    button_play,
    dribble_control;

float pose_final[3];

uint8_t sensor_garis_buffer;
uint16_t sensor_bola_filter;
uint8_t tombol_buffer;
float factor;
float pos_x_buffer;
float pos_y_buffer;
float pos_th_buffer;
float epoch;

// data buatan
double last_time_kick = 0;

//--Global Variables
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

struct
{
    int16_t data[4];
} data_int;

#endif