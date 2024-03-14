#include <iostream>
#include <cmath>
#include <termios.h>
#include <unistd.h>
using namespace std;

void setNonBlockingMode()
{
    struct termios t;

    tcgetattr(STDIN_FILENO, &t);

    t.c_lflag &= ~ICANON;
    t.c_cc[VMIN] = 0;
    t.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

// Get keyboard input
char getKeyPress()
{
    char ch = 0;

    if (read(STDIN_FILENO, &ch, 1) != 1)
        return 0;
    return ch;
}

float motion(float time)
{
    return tanh(time);
}

void reset(float &vel2, float &t)
{
    vel2 = 0, t = 0;
}

int main()
{
    float
        t = 0,
        vel = 0,
        vel2 = 0,
        speed = 0,
        a = 0,
        acc = 0;

    char key;
    unsigned int microseconds = 100000;

    setNonBlockingMode();

    while (true)
    {
        key = getKeyPress();

        if (key == '1')
        {
            speed = 10;
            acc = speed - vel2;
            t = 0;
        }
        else if (key == '2')
        {
            speed = 20;
            acc = speed - vel2;
            t = 0;
        }
        else if (key == '3')
        {
            speed = -30;
            acc = speed - vel2;
            t = 0;
        }
        else if (key == '0')
        {
            speed = -vel2;
            acc = speed - vel2;
            t = 0;
        }
        else if (key == '9')
        {
            speed = 50;
            acc = speed - vel2;

            t = 0;
        }

        vel2 += (tanh(t + 0.1) - tanh(t)) * acc;

        // if (vel2 >= 0)
        // {
        // }
        // else
        // {
        //     vel2 = 0;
        //     speed = 0;
        // }

        vel = tanh(t) * speed;

        t += 0.1;

        // a = (1 - tanh(t) * tanh(t)) * speed;

        cout
            << "time : " << t
            << "\tacc : " << acc
            << "\t\tvel-2 : " << vel2 << endl;
        // << "\t\tac : " << a << endl;

        usleep(microseconds);
    }
}
