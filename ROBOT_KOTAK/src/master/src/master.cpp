#include "master/master.hpp"

int main(int argc, char **argv)
{
    //--ROS Init
    ros::init(argc, argv, "master");
    ros::NodeHandle nh;
    ros::MultiThreadedSpinner spinner(3);

    //--Subscriber
    sub_data_int = nh.subscribe("/joystick/int", 1, subDataIntCallback);
    sub_data_float = nh.subscribe("/joystick/float", 1, subDataFloatCallback);

    sub_stm_data_int = nh.subscribe("/stm/master/int", 1, subStmDataIntCallback);
    sub_stm_data_float = nh.subscribe("/stm/master/float", 1, subStmDataFloatCallback);

    //--Publisher
    pub_stm_int = nh.advertise<std_msgs::Int16MultiArray>("/master/stm/int", 1);
    pub_stm_float = nh.advertise<std_msgs::Float32MultiArray>("/master/stm/float", 1);

    //--Timer
    timer_master = nh.createTimer(ros::Duration(0.01), timerCallback);

    //--ROS Spin
    spinner.spin();

    return 0;
}

void timerCallback(const ros::TimerEvent &)
{
    // from callbackSubStick
    static int n = 10;

    int8_t vel_x_buffer;
    int8_t vel_y_buffer;

    vel_x_buffer = analog_l[0] * (1.75 * n + button_r1 * 1 * n);
    vel_y_buffer = analog_l[1] * (1.75 * n + button_r1 * 1 * n);

    float norm_kiri = (axes_l2 - 1) / (-2.0);
    float norm_kanan = (axes_r2 - 1) / (-2.0);

    float buffer_th = norm_kiri - norm_kanan;

    int8_t vel_th_buffer = buffer_th * (20 + button_r1 * 10);

    if (button_triangle == 1)
    {
        setOffset(400, 600, 90);
    }

    if (button_x == 1)
    {
        printf("BUTTON X : %d", button_x);
        // dribble_kanan = 100;
        // dribble_kiri = 100;
        kick(63, 15);

        last_time_kick = ros::Time::now().toSec();
        ROS_INFO("NENDANG!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    }
    else if (button_square == 1)
    {
        printf("BUTTON [] : %d", button_x);
        kick(63, 30);

        last_time_kick = ros::Time::now().toSec();
        ROS_INFO("NENDANG!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    }

    setFinalPose();
    // setVelocity(vel_x_buffer, vel_y_buffer, vel_th_buffer);
    setVelocityLap(vel_x_buffer, vel_y_buffer, vel_th_buffer);
    // printf("set x: %d y: %d th: %d || raw x: %d y: %d th: %d\n", stm_data.vel.x, vel_y, vel_th, vel_x_buffer, vel_y_buffer, vel_th_buffer);

    // -----

    // ----- from callback send
    stm_data.buzzer_count = 0;
    stm_data.buzzer_time = 0;

    if (button_x == 1 && sensor_bola_filter > 45000)
    {
        printf("faktor noll \n");
        factor = 0;
    }
    else
    {
        factor = 1;
    }

    if (button_up == 1)
    {
        dribble_control = -1;
    }
    else if (button_l1 == 1)
    {
        dribble_control = 1;
    }
    else
    {
        dribble_control = 0;
    }

    if (ros::Time::now().toSec() - last_time_kick > 0.5)
    {
        stm_data.kick_mode = 0;
        stm_data.kick_speed = 0;
    }

    // printf("%d\n", dribble_control);
    stm_data.dribble_right = (-100 - 400 * dribble_control) * factor;
    stm_data.dribble_left = (-100 - 400 * dribble_control) * factor;

    // -----

    std::cout << " anlg lx: " << analog_l[0];
    std::cout << " anlg ly: " << analog_l[1];
    std::cout << " anlg rx: " << analog_r[0];
    std::cout << " anlg ry: " << analog_r[1] << std::endl;

    transmitAll();
}

void setOffset(float x, float y, float th)
{
    stm_data.pose_offset.x = pos_x_buffer - x;
    stm_data.pose_offset.y = pos_y_buffer - y;
    stm_data.pose_offset.theta = th + pos_th_buffer;

    while (stm_data.pose_offset.theta > 180)
        stm_data.pose_offset.theta -= 360;
    while (stm_data.pose_offset.theta < -180)
        stm_data.pose_offset.theta += 360;
}

void setFinalPose()
{
    pose_final[0] = pos_x_buffer - stm_data.pose_offset.x;
    pose_final[1] = pos_y_buffer - stm_data.pose_offset.y;
    pose_final[2] = stm_data.pose_offset.x - pos_th_buffer;

    while (pose_final[2] > 180)
        pose_final[2] -= 360;
    while (pose_final[2] < -180)
        pose_final[2] += 360;

    // printf("she %f %f %f \n",pos_x_buffer,pos_y_buffer,pos_th_buffer);
    // printf("ats %f %f %f \n",pose_offset[0],pose_offset[1],pose_offset[2]);
    // printf("bwh %f %f %f \n",pose_final[0],pose_final[1],pose_final[2]);
}

void setVelocity(int8_t vx, int8_t vy, int vth)
{

    static float v_buffer[2];

    float delta_v[2];
    delta_v[0] = vx - v_buffer[0];
    delta_v[1] = vy - v_buffer[1];

    float r = sqrt(delta_v[0] * delta_v[0] + delta_v[1] * delta_v[1]);
    float theta = atan2(delta_v[1], delta_v[0]);

    if (r > 5)
        r = 5;

    v_buffer[0] += r * cos(theta);
    v_buffer[1] += r * sin(theta);

    // printf("Velocity datas: %f %f %f\n", v_buffer[0], v_buffer[1], _vth);
    if (button_left == 1)
    {
        stm_data.vel.x = 0;
        stm_data.vel.y = 0;
        stm_data.vel.theta = 0;
        v_buffer[0] = 0;
        v_buffer[1] = 0;
    }
    else
    {
        stm_data.vel.x = v_buffer[0];
        stm_data.vel.y = v_buffer[1];
        stm_data.vel.theta = vth;
    }
}

void setVelocityLap(int8_t vx, int8_t vy, int vth)
{
    int8_t th_target = vth;
    int8_t vector_output[2];

    vector_output[0] = cos((90 - pose_final[2]) * DEG2RAD) * vx - sin((90 - pose_final[2]) * DEG2RAD) * vy;
    vector_output[1] = sin((90 - pose_final[2]) * DEG2RAD) * vx + cos((90 - pose_final[2]) * DEG2RAD) * vy;

    setVelocity(vector_output[0], vector_output[1], vth);
}

void subDataIntCallback(const std_msgs::Int16MultiArray::ConstPtr &msg)
{
    //--Data from Joystick with int type
    button_square = msg->data[0];
    button_x = msg->data[1];
    button_o = msg->data[2];
    button_triangle = msg->data[3];
    button_r1 = msg->data[4];
    button_l1 = msg->data[5];
    button_r2 = msg->data[6];
    button_l2 = msg->data[7];
    button_up = msg->data[8];
    button_down = msg->data[9];
    button_left = msg->data[10];
    button_right = msg->data[11];
}

void kick(int8_t mode, uint16_t kecepatan_tendang_lokal)
{
    stm_data.kick_mode = mode;
    stm_data.kick_speed = kecepatan_tendang_lokal;
}

void subDataFloatCallback(const std_msgs::Float32MultiArray::ConstPtr &msg)
{
    analog_l[0] = msg->data[0];
    analog_l[1] = msg->data[1];
    analog_r[0] = msg->data[2];
    analog_r[1] = msg->data[3];
    axes_l2 = msg->data[4];
    axes_r2 = msg->data[5];
    //--Data from Joystick with float type
}

void subStmDataIntCallback(const std_msgs::Int16MultiArray::ConstPtr &msg)
{
    sensor_garis_buffer = msg->data[0];
    sensor_bola_filter = msg->data[1];
    tombol_buffer = msg->data[2];
}

void subStmDataFloatCallback(const std_msgs::Float32MultiArray::ConstPtr &msg)
{
    factor = msg->data[0];
    pos_x_buffer = msg->data[1];
    pos_y_buffer = msg->data[2];
    pos_th_buffer = msg->data[3];
    epoch = msg->data[4];
}
void transmitAll()
{
    //--Publish all data to STM
    std_msgs::Float32MultiArray msg_stm_float;
    std_msgs::Int16MultiArray msg_stm_int;

    msg_stm_int.data.push_back(stm_data.vel.x);
    msg_stm_int.data.push_back(stm_data.vel.y);
    msg_stm_int.data.push_back(stm_data.vel.theta);
    msg_stm_int.data.push_back(stm_data.buzzer_count);
    msg_stm_int.data.push_back(stm_data.buzzer_time);
    msg_stm_int.data.push_back(stm_data.dribble_left);
    msg_stm_int.data.push_back(stm_data.dribble_right);
    msg_stm_int.data.push_back(stm_data.kick_mode);
    msg_stm_int.data.push_back(stm_data.kick_speed);

    msg_stm_float.data.push_back(stm_data.pose_offset.x);
    msg_stm_float.data.push_back(stm_data.pose_offset.y);
    msg_stm_float.data.push_back(stm_data.pose_offset.theta);

    pub_stm_int.publish(msg_stm_int);
    pub_stm_float.publish(msg_stm_float);
}
