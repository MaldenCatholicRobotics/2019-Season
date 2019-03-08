#include <kipr/botball.h>

void line_follower(int runtime, int m_port_l, int m_port_r, int ir_port_l, int ir_port_r, int slow_speed, int reg_speed, int fast_speed, int tape_benchmark);


int main()
{
    line_follower(10, 0, 1, 0, 1, 200, 400, 600, 3000);
    return 0;
}
//runtime- the number of tenths of seconds the bot should follow the line
//m_port_l and m_port_r- the ports of the left and right motors
//ir_port_l and ir_port_r- the ports of the left and right ir sensors
//slow_speed, reg_speed and fast_speed- the motor speeds for going straight and turning
//tape_benchmark- the ir value that decides if the sensor is on the tape or not
void line_follower(int runtime, int m_port_l, int m_port_r, int ir_port_l, int ir_port_r, int slow_speed, int reg_speed, int fast_speed, int tape_benchmark)
{
    //creates a temporary counter to control the while loop
    int counter = 0;
    //keeps the program running for a specified time
    while(counter <= runtime)
    {
        //if both sensors are off tape
        if(analog(ir_port_l) <= tape_benchmark && analog(ir_port_r) <= tape_benchmark)
        {
            //move straight
            mav(m_port_l, reg_speed);
            mav(m_port_r, reg_speed);
            msleep(100);
            printf("Moving straight");
        }
        //if the left sensor is off tape but the rigth sensor is on
        else if(analog(ir_port_l) <= tape_benchmark && analog(ir_port_r) > tape_benchmark)
        {
            //turn left
            mav(m_port_l, slow_speed);
            mav(m_port_r, fast_speed);
            msleep(100);
            printf("Moving left");
        }
        //assumed that the right sensor is off tape but the left is on
       else
        {
           //turn right
            mav(m_port_l, fast_speed);
            mav(m_port_r, slow_speed);
            msleep(100);
            printf("Moving straight");
        }
    }
    //shuts motors off
    ao();
}