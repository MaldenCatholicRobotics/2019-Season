#include <kipr/botball.h>
#include <stdbool.h>

//Notes
//

//FUNCTION DECLARATIONS

//follows the edge of a line using a single ir sensor
void line_follower(int runtime, int tape_benchmark);

//follows the edge of a line backwards using a single ir sensor
void reverse_line_follower(int runtime, int tape_benchmark);

//determines if a building or medical center is on fire and returns a boolean value
bool fire_scan(double red_benchmark);

//moves a servo from one given position to another at a certain speed
void servo_change(int op, int wp, int servoport, int speed);

//turns the bot at a specified speed and time
void turn(int time, int power, int port) ;

//drives the bot forward for a certain time, with each motor able to spin at a different speed
void drive(int time, int l_power, int r_power); 

//drives the bot in either direction until the ir sensor reaches the line
void drive_until_line(int tape_benchmark, int l_power, int r_power);

//puts an object in front of the not on-fire medical center
void deliver();

//goes from starting box to start of centers tape while grabbing the ambulance
void opening_sequence();

//determines on-fire center and puts ambulance on the other center
void ambulance();

//grabs a set of 6 people and puts them in front of not on-fire medical center
void people(int set);

//GLOBAL VARIABLES
//ports of the servos, motors and ir sensors
int m_port_l = ;
int m_port_r = ;
int f_ir_port = ;
int r_ir_port = ; 
int servo_port_scooper = ;
int servo_port_sweeper = ;

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
int r_reg_speed = -1200;
int r_fast_speed = -1600;
int r_slow_speed = -800;

//the values for the scooper positions
int scooper_up = ;
int scooper_down = ;

//the values for the sweeper positions
int sweeper_up = ;
int sweeper_down = ;

//data collected by scan_centers function
int center_on_fire;
int safe_center;

//variables to finetune the line following
//QUICK_CORRECT + STRAIGHTEN_CORRECT MUST = 10
int quick_correct = 8;
int straighten_correct = 2;

//MAIN
int main()

{
    enable_servos();
    //sets inital claw position to up and closed	  
    set_servo_position(servo_port_scooper, scooper_up);
    set_servo_position(servo_port_sweeper, sweeper_up);
    opening_sequence();
    ambulance();
    people(0);
    disable_servos();
    return 0;
}

//FUNCTIONS

//follows the edge of a line using a single ir sensor
//runtime- the number of tenths of seconds the bot should follow the line
//tape_benchmark- the ir value that decides if the sensor is on the tape or not
void line_follower(int runtime, int tape_benchmark)
{
    //creates temporary counters to control the while loops
    int counter1 = 0;
    int counter 2 = 0;
    //keeps the program running for a specified time
    while(counter1 <= quick_correct*runtime)
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
        counter1++;
    }
	
    //keeps the program running for a specified time
    while(counter2 <= straighten_correct*runtime)
    {
        //if sensor on tape
        if(analog(f_ir_port) >= 3000)
        {
            //veer slightly left
            mav(m_port_l, 1200);
            mav(m_port_r, 1150);
	    //checks position every 1/100 seconds
            msleep(10);
        }
        //assumed that sensor is off tape
        else
        {
            //veer slightly right
            mav(m_port_l, 1150);
            mav(m_port_r, 1200);
            //checks position every 1/100 seconds
            msleep(10);
        }
	//increments counter
        counter2++;
    }
    //turns off motors
    ao();
}

//follows the edge of a line backwards using a single ir sensor
//runtime- the number of tenths of seconds the bot should follow the line
//tape_benchmark- the ir value that decides if the sensor is on the tape or not
void reverse_line_follower(int runtime, int tape_benchmark)
{
    //creates temporary counters to control the while loops
    int counter1 = 0;
    int counter 2 = 0;
    //keeps the program running for a specified time
    while(counter1 <= quick_correct*runtime)
    {
        //if sensor on tape
        if(analog(r_ir_port) >= 3000)
        {
            //veer slightly left
            mav(m_port_l, -1200);
            mav(m_port_r, -1000);
	    //checks position every 1/100 seconds
            msleep(10);
        }
        //assumed that sensor is off tape
        else
        {
            //veer slightly right
            mav(m_port_l, -1000);
            mav(m_port_r, -1200);
            //checks position every 1/100 seconds
            msleep(10);
        }
	//increments counter
        counter1++;
    }
	
    //keeps the program running for a specified time
    while(counter2 <= straighten_correct*runtime)
    {
        //if sensor on tape
        if(analog(r_ir_port) >= 3000)
        {
            //veer slightly left
            mav(m_port_l, -1200);
            mav(m_port_r, -1150);
	    //checks position every 1/100 seconds
            msleep(10);
        }
        //assumed that sensor is off tape
        else
        {
            //veer slightly right
            mav(m_port_l, -1150);
            mav(m_port_r, -1200);
            //checks position every 1/100 seconds
            msleep(10);
        }
	//increments counter
        counter2++;
    }
    //turns off motors
    ao();
}

//determines if a building or medical center is on fire and returns a boolean value
//red_benchmark- the minimum average confidence for the building to be seen as on fire
//a higher red_benchmark makes the function more selective
//make sure the configuration name matches the one on the wallaby
bool fire_scan(double red_benchmark) 
{ 
    //create counter variables for the while loops
    int counter_1 = 0; 
    int counter_2 = 0;
	
    //creates variables to store values gathered by camera
    double red_confidence; 
    double red_total; 
    double red_average; 

    //configures and updates camera
    camera_open_black(); 
    camera_load_config("fire"); 
    camera_update(); 

    //one picture and about one tenth of a second per iteration
    //creates a one second buffer period for the camera to set up
    while(counter_1 < 10) 
    { 
        camera_update();
        msleep(100);
        counter_1++;
    }   
	
    //creates a running tally of the confidence in each snapshot
    while(counter_2 < num_pics)
    {
	//gets the confidence that the building is on fire
        camera_update(); 
        red_confidence = get_object_confidence(red_channel, 0);

        //print statement for debugging
        //printf("red_confidence: %f\n", red_confidence);

        //adds the confidence to the running tally
        red_total += red_confidence; 
        msleep(100); 
        counter_2++; 
    }

    //creates an average of all collected red_condifences
    red_average = red_total/num_pics;  
	
    //closes the camera
    camera_close(); 

    //print statement for debugging
    printf("red average:%f\n", red_average);   

    //if building is on fire
    if (red_average >= red_benchmark) 

    { 
        printf("FIRE\n");
        return true; 
    } 
    //if building is not on fire
    else 
    { 
        printf("SAFE\n");
        return false; 
    } 
} 

//moves a servo from one given position to another at a certain speed
//op- the starting position of the servo
//wp- the ending positon of the servo
//servoport- the port of the servo 
//speed- the increment by which the servo position is repeatedly moved
void servo_change(int op, int wp, int servoport, int speed) 
{   
    //allows for the servo to change in any direction
    if (op > wp) 
    { 
        while(op > wp)  
        {   
	    //if the position is less than one increment away from the wanted position
	    if(op - wp < speed)
	    {
		    //just set the claw to the end position
		    set_servo_position(servoport, wp);
		    op = wp;
	    }
	    else
	    {
		    //increments the position
		    op -= speed;  

		    //sets the servo position to the incremented position
		    set_servo_position(servoport, op);   
		    msleep(100); 
	    }
        }   
    }
    else 
    { 
        while(wp > op)  
        {   
	    //if the position is less than one increment away from the wanted position
	    if(wp - op < speed)
	    {
		    //just set the claw to the end position
		   set_servo_position(servoport, op); 
		    op = wp;
	    }
	    else
	    {
		    //increments the position
		    op += speed; 

		    //sets the servo position to the incremented position
		    set_servo_position(servoport, op);   
		    msleep(100); 
	    }
        }   
    } 
}    

//turns the bot at a specified speed and time
//to turn left, call the right servo port 
//to turn right, call the left servo port 
//time- the larger the time the larger the degree of the turn
//power- the power with which the wheel turns
//port- the port of the motor turning
void turn(int time, int power, int port) 
{ 
    //spins the motor at a certain power for a certain amount of time
    mav(port, power); 
    msleep(time); 
  
    //shuts off motors
    ao();
} 

//drives the bot forward for a certain time, with each motor able to spin at a different speed
//Use a negative power to drive backwards
//l_power and r_power- the speed for the two different motors (allows for different motor rates)
//time- the duration of driving
void drive(int time, int l_power, int r_power) 
{ 
    //spins the left motor at a certain speed
    motor(m_port_l, l_power); 
	
    //spins the left motor at a certain speed
    motor(m_port_r, r_power); 
    msleep(time); 
	
    //shuts off motors
    ao(); 
} 

//drives the bot in either direction until the ir sensor reaches the line
//Use a negative power to drive backwards
//l_power and r_power- the speed at which the two motors drive (allows for different motor rates)
//tape_benchmark- the analog values of the tape needed
void drive_until_line(int tape_benchmark, int l_power, int r_power)
{
    //creates a  variable to kick the code out of the loop
    int statement = 0; 
    //while the bot has not reached the tape
    while(statement == 0)
    {
	//if the bot should drive forwards
	if(l_power > 0)
	{
		//if both sensors are off tape
		if(analog(f_ir_port) <= tape_benchmark)
		{
		    //move straight
		    mav(m_port_l, l_power);
		    mav(m_port_r, r_power);
		    msleep(10);
		}
		//the bot has reached the tape
		else
		{
		    //kick the code out of the loop
		    statement = 1;
		    msleep(10);
		}
	}
	//assumes the bot should drive forwards
	else
	{
		//if both sensors are off tape
		if(analog(r_ir_port) <= tape_benchmark)
		{
		    //move straight
		    mav(m_port_l, l_power);
		    mav(m_port_r, r_power);
		    msleep(10);
		}
		//the bot has reached the tape
		else
		{
		    //kick the code out of the loop
		    statement = 1;
		    msleep(10);
		}
	}
    }
	
    //shuts off motors
    ao();
}

//finds out which medical center is on fire and stores the center number in the variable
//Starts: beginning of centers tape
//Ends: In front of center 1
void scan_centers()	
{
    //drives to center 1
    line_follower(3, black_tape);

    //scans center 1 for fire
    if(fire_scan(fire_benchmark))
    {
        //center 1 is on fire
        printf("Medical Center 1 is on fire!\n");
	    
	      //sets variables regarding the centers
        center_on_fire = 1;
        safe_center = 2;
    }

    else
    {
        //Assumes that center 2 is on fire
        printf("Medical Center 2 is on fire!\n"); 
	    
	      //sets variables regarding the centers
        center_on_fire = 2;
        safe_center = 1;
    }
  
    //shuts off motors
    ao();
}

//puts an object in front of the not on-fire medical center
//Start: in front of medical center 1
//Ends: in front of medical center 1
void deliver()
{
  
}

//goes from starting box to start of centers tape while grabbing the ambulance
//Starts: Starting position
//Ends: in front of medical center 1
void opening_sequence()
{
	//drive so sweeper is over ambulance
	drive(500, reg_speed, reg_speed);
	
	//put sweeper down over ambulance
	servo_change(sweeper_up, sweeper_down, servo_port_sweeper, 30);
	
	//drive past the starting box tape
	drive(500, reg_speed, reg_speed);
	
	//drive up to the buildings tape
	drive_until_line(black_tape, reg_speed, reg_speed);
	
	//turn left onto the buildings tape
	turn(turn_time, turn_power, m_port_r);
	
	//follow the buildings tape to its start
	line_follower(30, black_tape);
	
	//turns right onto the centers tape
	turn(turn_time, turn_power, m_port_l);
	
	//line follows to in front of medical center 1
	reverse_line_follower(10, black_tape);
}

//determines on-fire center and puts ambulance on the other center
//Starts: in front of medical center 1
//Ends: Start of building tape
void ambulance()
{
  
}

//grabs a set of 6 people and puts them in front of not on-fire medical center
//Starts: Start of building tape
//Ends: Start of building tape
void people(int x)
{
  
}
