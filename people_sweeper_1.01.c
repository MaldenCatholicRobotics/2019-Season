#include <kipr/botball.h>
#include <stdbool.h>

//VARIABLES

//motor ports
int m_port_l = 3;
int m_port_r = 2;

//IR sensor ports
int f_ir_port = 0;
int r_ir_port = 1;

//value for the black tape
int black_tape = 2000;

//varoables for fire scan
int num_pics = 20;
double fire_benchmark = 0.2;
int red_channel = 0;

//speeds
int reg_speed = 1200;
int r_reg_speed = -1200;
int slow_speed = 600;
int r_slow_speed = -600;

//for the line follower runtime breakdowns
//MUST ADD UP TO 10
int quick_correct = 8;
int straighten_correct = 2;

//servo ports
int lower_servo_port = ;
int upper_servo_port = 0;

//servo positions 
int lower_up = ;
int lower_down = ;
int upper_up = ;
int upper_down = ;

//FUNCTIONS

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
	
    //shuts off motors
    msleep(time); 
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

int main()
{
    enable_servos();
	
    //sets upper servo to ???
    set_servo_position(0, 800);
	
    //drive robot up to tape
    drive(2800, reg_speed, reg_speed);

    //turns left onto buildings tape
    turn(1300, reg_speed, m_port_r);
    msleep(2000);
    
    //drives up to the medical center
    line_follower(25, black_tape);
    
    //turns to face center
    turn(1300, reg_speed, m_port_l);
    
    //if the center is on fire
    if(fire_scan(red_benchmark))
    {
	//drives backwards
        reverse_line_follower(12, black_tape);
        turn(1400, reg_speed, m_port_r);
        drive(2000, r_slow_speed, r_slow_speed); 
    }
    else
    {
	    
    }
    
    //makes robot get into position for scn med. cent.
    //turnLeft(900, l_port, reg_speed);
    //makes robot get into position for scn med. cent.
    //driveStraight(10500, reg_speed, reg_speed);
    //makes robot get into position for scn med. cent. 
    //turnArd(1200, reg_speed, r_reg_speed);
	
    disable_servos();
    return 0;
}
