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

//goes from starting box to start of centers tape while grabbing the ambulance
void opening_sequence();

//Runs the main body of the code
void body();

//GLOBAL VARIABLES
//ports of the servos, motors and ir sensors
int m_port_l = 2;
int m_port_r = 3;
int f_ir_port = 0;
int r_ir_port = 1; 
int servo_port_scooper = 1;
int servo_port_sweeper = 0;

//the current values needed to turn approximately 90 degrees
int turn_power = 1200;
int turn_time = 1350;

//holds the channel the red object is held in
int red_channel = 0;

//the number of pictures to be taken in the fire_scan function
int num_pics = 20;

//ir value of the black tape
int black_tape = 1000;

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
int scooper_up = 870;
int scooper_down = 1600;


//the values for the sweeper positions
int sweeper_up = 400;
int sweeper_down = 0;
int sweeper_start = 880;
int sweeper_grab = 300;

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
    set_servo_position(servo_port_sweeper, sweeper_start);   
    opening_sequence();
    body();
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
    int counter2 = 0;
    //keeps the program running for a specified time
    while(counter1 <= quick_correct*runtime)
    {
        //if sensor on tape
        if(analog(f_ir_port) >= black_tape)
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
        if(analog(f_ir_port) >= black_tape)
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
    int counter2 = 0;
    //keeps the program running for a specified time
    while(counter1 <= quick_correct*runtime)
    {
        //if sensor on tape
        if(analog(r_ir_port) >= black_tape)
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
        if(analog(r_ir_port) >= black_tape)
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
		if(analog(f_ir_port) >= tape_benchmark)
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
		if(analog(r_ir_port) >= tape_benchmark)
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

//goes from starting box to start of centers tape while grabbing the ambulance
void opening_sequence()
{
	//put sweeper down over ambulance
	servo_change(sweeper_start, sweeper_down, servo_port_sweeper, 30);
	
	//drive past the starting box tape
	drive(800, reg_speed+100, reg_speed);
	msleep(1000);
	
    	//puts the claw up to grab first set of medical supplies
    	servo_change(sweeper_down, sweeper_up, servo_port_sweeper, 30);
	
	//drive over the medical supplies
    	drive(250, reg_speed, reg_speed);
    	msleep(1000);
	
	//puts sweeper down over the supplies
    	servo_change(sweeper_up, sweeper_down, servo_port_sweeper, 30);
    
	//drives over the buildings tape
    	drive(650, reg_speed, reg_speed);
    	msleep(1000);
	
	//turn left onto the buildings tape
	turn(turn_time+300, turn_power, m_port_r);
	
	//follow the buildings tape until front of sweeper is just behind the medical supplies
	line_follower(11, black_tape);
	msleep(1000);
	
	//puts the sweeper up
    	servo_change(sweeper_down, sweeper_up, servo_port_sweeper, 30);
	
	//drives until sweeper is over the supplies
    	line_follower(2, black_tape);
	
	//puts sweeper down over the supplies
   	 servo_change(sweeper_up, sweeper_down, servo_port_sweeper, 30);
	
	//line follows to the end of the buildings tape
    	line_follower(11, black_tape);
	
	//turns right onto the centers tape
	turn(turn_time-100, turn_power, m_port_l);
}

//Runs the main body of the code
void body()
{
	//if the first center is on fire
	if(fire_scan(fire_benchmark))
    {
	//drives up to the second center
        drive(200, reg_speed, reg_speed);
	
	//turn so amulance is in front of the second center
        turn(1200, turn_power, m_port_r); 
	
	//lifts sweeper arm clear over ambulance
        servo_change(sweeper_down, sweeper_up, servo_port_sweeper, 30);
        msleep(1000);
	
	//backs up so the front bar of the sweeper is behind the ambulance
        drive(1000, r_reg_speed, r_reg_speed);
	
	//puts down the sweeper
        servo_change(sweeper_up, sweeper_down, servo_port_sweeper, 30);
	
	//pushes the ambulance deeper into the second center's box
        drive(1200, reg_speed, reg_speed);
	
	//backs out of the zone
        drive(500, r_reg_speed, r_reg_speed);
	
	//turns to be parallel with centers tape
        turn(700, turn_power, m_port_l);
		
	//backs up until perpendicular with buildngs tape
        drive(700, r_reg_speed, r_reg_speed);
	
	//turns onto the buildings tape
        turn(turn_time-250, turn_power, m_port_l);    
	
	//puts the sweeper up so it doesn't get caught on tape
        servo_change(sweeper_down, sweeper_grab, servo_port_sweeper, 30);
		
	//line follows forward to prepare for correction
        line_follower(19, black_tape);
		
	//line follows bakcwards to correct position
        reverse_line_follower(15, black_tape);
	
	//turns to face the row of people
	turn(turn_time-210, turn_power, m_port_r);
        msleep(1000);
		
        //drives under the skybridge to hover over the people
        drive(1000, reg_speed, reg_speed);
	    
	//puts the sweeper down
        servo_change(sweeper_grab, sweeper_down, servo_port_sweeper, 30);
	
	//scooper knocks down the people on top
        servo_change(scooper_up, scooper_down, servo_port_scooper, 30);
	
	//backs up to the buildings tape
	//top row of people is knocked over and falls into the sweeper
        drive(1500, r_reg_speed, r_reg_speed);
        
	//turns to face the corner of center 2
        turn(turn_time/2+200, turn_power, m_port_r);
        
	//drives so sweeper is in center 2
        drive(1300, reg_speed, reg_speed);
        
        //raises sweeper clear of the ambulance
        servo_change(sweeper_down, sweeper_up, servo_port_sweeper, 30);
	    
	//backs up to prepare for sweeping people
	drive(1100, r_reg_speed, r_reg_speed);
	
	//puts sweeper down
        servo_change(sweeper_up, sweeper_down, servo_port_sweeper, 30);
	    
	//pushes the people deeper into the zone
        drive(1400, reg_speed, reg_speed);
	    
	//backs up slightly
        drive(200, r_reg_speed, r_reg_speed);
        
    }
    else
    {
	//turns part of the way to face center 1
        turn(1000, turn_power, m_port_r);
	    
	//backs up to clear center and prevent crashing
        drive(1000, r_reg_speed, r_reg_speed);
	    
	//turns to fully face the center
        turn(300, turn_power, m_port_r);
	    
	//raises sweeper clear of the ambulance
        servo_change(sweeper_down, sweeper_up, servo_port_sweeper, 30);
	    
	//drives backwards so front bar of sweeper is behind the ambulance
        drive(500, r_reg_speed, r_reg_speed);
	
	//puts sweeper down
        servo_change(sweeper_up, sweeper_down, servo_port_sweeper, 30);
	    
	//pushes the ambulance deeper into the zone
        drive(1200, reg_speed, reg_speed);
	    
	//backs up slightly
        drive(200, r_reg_speed, r_reg_speed);
	    
	//turns right partially to get onto buildings tape
        turn(turn_time, turn_power, m_port_l);
        msleep(1000);
	   
	//backs up for clearance
        drive(200, r_reg_speed, r_reg_speed);
        msleep(1000);
	
        //readies the sweeper to grab the people
	servo_change(sweeper_down, sweeper_grab, servo_port_sweeper, 30);
        msleep(1000);
        
	//finishes the turn onto the buildings tape
        turn(turn_time-400, turn_power, m_port_l);
        msleep(1000);
	
	//line follows up to the first row of people
        line_follower(20, black_tape);
        msleep(1000);
        reverse_line_follower(15, black_tape);
	    
	//turns partially left to face the people
        turn(turn_time-300, turn_power, m_port_r);
        msleep(1000);
	
	//drives under the skybridge to hover over the people
        drive(1200, reg_speed, reg_speed);
	    
	//puts the sweeper down
        servo_change(sweeper_grab, sweeper_down, servo_port_sweeper, 30);
	
	//scooper knocks down the people on top
        servo_change(scooper_up, scooper_down, servo_port_scooper, 30);
	
	//backs up to the buildings tape
	//top row of people is knocked over and falls into the sweeper
        drive(2400, r_reg_speed, r_reg_speed);
	
	//turns to face center 1
        turn(turn_time-100, turn_power, m_port_r);
	
	//drives so sweeper is in center 1
	drive(700, reg_speed, reg_speed);
        msleep(1000);
	    
        //raises sweeper clear of the ambulance
        servo_change(sweeper_down, sweeper_up, servo_port_sweeper, 30);
	  
	//drive back to prepare to push people
	reverse_line_follower(11, black_tape);
	
	//puts sweeper down
        servo_change(sweeper_up, sweeper_down, servo_port_sweeper, 30);
	    
	//pushes the people deeper into the zone
        drive(1400, reg_speed, reg_speed);
	    
	//backs up slightly
        drive(200, r_reg_speed, r_reg_speed);
    }
}
