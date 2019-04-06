#include <kipr/botball.h>
#include <stdbool.h>

//Notes
//TOOK OUT CLAW_SLIGHTLY_OPEN AND ARM_VERY_UP

//FUNCTION DECLARATIONS
void line_follower(int runtime, int tape_benchmark);

void reverse_line_follower(int runtime, int tape_benchmark);

//GLOBAL VARIABLES
//ports of the servos, motors and ir sensors
int m_port_l = 0;
int m_port_r = 1;
int f_ir_port = 0;
int r_ir_port = 1; 
int servo_port_arm = 0;
int servo_port_claw = 2;

//the current values needed to turn approximately 90 degrees
int turn_power = 1200;
int turn_time = 900;

//holds the channel the red object is held in
int red_channel = 0;

//the number of pictures to be taken in the fire_scan function
int num_pics = 20;

//ir value of the black tape
int black_tape = 3000;

//the average confidence the camera needs to determine a building is on fire
double fire_benchmark = 0.2;

//speeds for the bot to drive forwards
int reg_speed = 1200;
int fast_speed = 1600;
int slow_speed = 800;

//speeds for the bot to drive backwards
int r_reg_speed = (0-1200);
int r_fast_speed = (0-1600);
int r_slow_speed = (0-800);

//the values for the claw and arm positions
int claw_open = 1400;
int claw_close = 550;
int arm_up = 800;
int arm_down = 250;

//data collected by scan_buildings function
int building_on_fire;
int safe_building_right;
int safe_building_left;

//data collected by scan_centers function
int center_on_fire;
int safe_center;

//MAIN
int main()

{
    enable_servos();
    //sets inital claw position to up and closed	  
    set_servo_position(servo_port_arm, arm_up);
    set_servo_position(servo_port_claw, claw_close);
    disable_servos();
    return 0;
}

//FUNCTIONS

//follows the edge of a line using a single ir sensor
//runtime- the number of tenths of seconds the bot should follow the line
//tape_benchmark- the ir value that decides if the sensor is on the tape or not
void line_follower(int runtime, int tape_benchmark)
{
    //creates a temporary counter to control the while loop
    int counter = 0;
    //keeps the program running for a specified time
    //runtime is multiplied by ten to account for 1/100 second iterations but 1/10 second inputs
    while(counter <= 10*runtime)
    {
        //if sensor on tape
        if(analog(f_ir_port) >= 3000)
        {
            //veer slightly left
            mav(m_port_l, 1200);
            mav(m_port_r, 1000);
	    //checks position every 1/100 seconds
            msleep(10);
        }
        //assumed that sensor is off tape
        else
        {
            //veer slightly right
            mav(m_port_l, 1000);
            mav(m_port_r, 1200);
            //checks position every 1/100 seconds
            msleep(10);
        }
	//increments counter
        counter++;
    }
    //turns off motors
    ao();
}

//follows the edge of a line backwards using a single ir sensor
//runtime- the number of tenths of seconds the bot should follow the line
//tape_benchmark- the ir value that decides if the sensor is on the tape or not
void reverse_line_follower(int runtime, int tape_benchmark)
{
    //creates a temporary counter to control the while loop
    int counter = 0;
    //keeps the program running for a specified time
    //runtime is multiplied by ten to account for 1/100 second iterations but 1/10 second inputs
    while(counter <= 10*runtime)
    {
        //if sensor off tape
        if(analog(f_ir_port) <= 3000)
        {
            //veer slightly left
            mav(m_port_l, (0-1200));
            mav(m_port_r, (0-1000));
	    //checks position every 1/100 seconds
            msleep(10);
        }
        //assumed that sensor is on tape
        else
        {
            //veer slightly right
            mav(m_port_l, (0-1000));
            mav(m_port_r, (0-1200));
            //checks position every 1/100 seconds
            msleep(10);
        }
	//increments counter
        counter++;
    }
    //turns off motors
    ao();
}
