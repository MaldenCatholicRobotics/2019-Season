#include <kipr/botball.h>
int m_port_l = 0;
int m_port_r = 1;
int runtime = 100;
int main()
{
    enable_servos();
    set_servo_position(0, 1000);
    //creates a temporary counter to control the while loop
    int counter = 0;
    //keeps the program running for a specified time
    while(counter <= runtime)
    {
        //if both sensors are off tape
        if(analog(0) >= 3000)
        {
            //move left
            mav(m_port_l, 1200);
            mav(m_port_r, 1100);
            msleep(100);
        }
        //assumed that the right sensor is off tape but the left is on
        else
        {
            //turn right
            mav(m_port_l, 1100);
            mav(m_port_r, 1200);
            msleep(100);
        }
        counter++;
    }
    //shuts motors off
    disable_servos();
    ao();
    return 0;
}
