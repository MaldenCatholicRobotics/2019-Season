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

//finds which of the three buildings is on fire, stops in front of that building and saves the building number to the global variable
void scan_buildings();

//finds out which medical center is on fire and stores the center number in the variable
void scan_centers();

//picks up an object using the claw
void collect_object();

//puts a firefighter onto the skybridge
void deliver_bridge();

//puts an object in front of a building or medical center
void deliver_ground();
	
//puts a firefighter on top of the medical center's roof
void deliver_roof();

//pulls out the bottom firefighter from the pole
void pull_firefighter();

//drives from the pole to the medical centers
void pole_to_centers();

//drives the bot from the firepole to the buildings
void pole_to_buildings()

//drives bot from medical centers to the firepole
void centers_to_pole();

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

//the values for the claw positions
int claw_open = 1400;
int claw_close = 550;
int claw_release = 800;

//the values for the arm positions
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
        if(analog(r_ir_port) <= 3000)
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

//finds which of the three buildings is on fire, stops in front of that building and saves the building number to the global variable
//Starts: beginning of buildings tape
//Ends: In front of on fire building
void scan_buildings()
{
    //drives to building 1 with camera centered on where the fire patch could be
    line_follower(18, black_tape);

    //scans building 1 for fire
    if(fire_scan(fire_benchmark))

    {
        //building 1 is on fire
        printf("Building 1 is on fire!\n");
	    
	//sets variables regarding the buildings
        building_on_fire = 1;
        safe_building_left = 2;
        safe_building_right = 3;
    }

    else
    {
        //building one is not on fire so bot drives to building 2
        line_follower(12, black_tape);

        //checks if building 2 is on fire
        if(fire_scan(fire_benchmark))
        {
	    //building 2 is on fire
            printf("Building 2 is on fire!\n");

	    //sets variables regarding the buildings
            building_on_fire = 2;
            safe_building_left = 1;
            safe_building_right = 3;
        }
        else
        {
            //assumes building 3 is on fire so bot drives to building 3
            line_follower(12, black_tape);
		
            //building 3 is on fire
            printf("Building 3 is on fire!\n"); 
		
	    //sets variables regarding the buildings
            building_on_fire = 3;
            safe_building_left = 1;
            safe_building_right = 2;
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

//picks up an object using the claw
//Starts: Arm up and claw closed
//Ends: Arm up and claw closed
void collect_object()
{
    //opens claw
    servo_change(claw_close, claw_open, servo_port_claw, 30);
	
    //arms descends
    servo_change(arm_up, arm_down, servo_port_arm, 30);
	
    //claw closes
    servo_change(claw_open, claw_close, servo_port_claw, 30);
	
    //arm ascends
    servo_change(arm_down, arm_up, servo_port_arm, 30);
}

//puts a firefighter onto the skybridge
//Starts: in front of the on fire building
//Ends: in front of the on fire building
void deliver_bridge()
{
    //turn left so the bot is facing the skybridge
    turn(turn_time, turn_power, m_port_r);
    msleep(1000);
	
    //drive until the firefighter is just over the skybridge
    drive(200, reg_speed, reg_speed);
    msleep(1000);
	
    //slowly opens the claw to drop the firefighter onto the skybrdige
    servo_change(claw_close, claw_release, servo_port_claw, 10);
	
    //drives back to the tape
    drive(200, r_reg_speed, r_reg_speed);
    msleep(1000);
	
    //turns back onto tape
    turn(turn_time, turn_power, m_port_l);
	
    //closes claw
    servo_change(claw_release, claw_close, servo_port_claw, 10);
}

//puts an object in front of a building or medical center
//Starts: in front of dropoff location
//Ends: in front of dropoff location
void deliver_ground()
{
    //follows the line backwards shortly
    reverse_line_follower(5, black_tape);
    msleep(1000);
	
    //turns to face the center
    turn(turn_time, turn_power, m_port_r);	    
    
    //lowers claw to a down position
    servo_change(arm_up, arm_down, servo_port_arm, 30);
	
    //inches up into middle of zone
    drive(75, reg_speed, reg_speed);
	
    //slowly and slightly opens claw to release object
    servo_change(claw_close, claw_release, servo_port_claw, 20);

    //drives back up
    drive(50, r_reg_speed, r_reg_speed);

    //lifts up arm
    servo_change(arm_down, arm_up, servo_port_arm, 30);
    
    //closes claw 
    servo_change(claw_release, claw_close, servo_port_claw, 20);
	
    //fully backs up
    drive(350, r_reg_speed, r_reg_speed);
    msleep(1000);
	
    //turns back onto tape
    turn(turn_time-100, turn_power, m_port_l);   
}

//puts a firefighter on top of the medical center's roof
//Starts: start of centers tape
//Ends: start of centers tape
void deliver_roof()
{
     //follows the line backwards shortly
     reverse_line_follower(6, black_tape);
     msleep(1000);
	
     //turns to face the center
     turn(turn_time, turn_power, m_port_r);
     msleep(1000);
	
     //drives until the firefighter is just above the center
     drive(100, reg_speed, reg_speed);
     msleep(1000);
	
     //slowly and slightly opens claw to release firefighter onto roof
     servo_change(claw_close, claw_release, servo_port_claw, 10);
     msleep(1000);
	
     //drives backwards the same dstance it drove forwards
     drive(250, r_reg_speed, r_reg_speed);
     msleep(1000);
     
     //turns back onto centers tape
     turn(turn_time, turn_power, m_port_l);
     msleep(1000);
	
     //puts the claw back to the closed position
     servo_change(claw_release, claw_close, servo_port_claw, 30);
   
     //drives back to start of centers tape
     reverse_line_follower(1, black_tape);
     msleep(1000);
}

//pulls out the bottom firefighter from the pole
//Starts: in front of pole
//Ends: in front of pole
void pull_firefighter()
{
    //opens claw
    servo_change(claw_close, claw_open, servo_port_claw, 30);
	
    //lowers arm to ground level
    servo_change(arm_up, arm_down, servo_port_arm, 30);
	
    //drives up to surround firefighter
    drive(250, reg_speed, reg_speed);
    msleep(1000);
	
    //claw closes around firefighter, securing it
    servo_change(claw_open, claw_close, servo_port_claw, 20);
    msleep(1000);
	
    //bot drives backwards with firefighter past the pole tape
    //possibly make some of it line following for corrections
    drive(1000, r_reg_speed, (r_reg_speed);
	
    //arm lifts up with firefighter in it
    servo_change(arm_down, arm_up, servo_port_arm, 20);
	  
    //drives back up to the pole tape
    drive_to_line(black_tape, reg_speed, reg_speed);
}
	 
//drives from the pole to the medical centers
//Starts: start of pole tape
//Ends: start of centers tape
void pole_to_centers()
{
    //turns right to face tape in front of centers
    turn(turn_time, turn_power, m_port_l);
    msleep(1000);
	
    //drives up to tape in front of centers in position to scan centers/ deliver firefighter/go to buildings
    //use a combo of line following and straight up driving
    drive(300, reg_speed, reg_speed);
    line_follower(2, black_tape);
    msleep(1000);
}

//drives the bot from the firepole to the buildings
//Starts: start of pole tape
//Ends: start of buildings tape
void pole_to_buildings()
{
    //turns right to face tape in front of centers
    turn(turn_time, turn_power, m_port_l);
    msleep(1000);
	
    //drives up to tape in front of centers in position to scan centers/ deliver firefighter/go to buildings
    //use a combo of line following and straight up driving
    drive(400, reg_speed, reg_speed);
    line_follower(4, black_tape);
    msleep(1000);
	
    //turn onto building tape
    turn(turn_time, turn_power, m_port_l);
    msleep(1000);
}
	  
//drives bot from medical centers to the firepole
//Starts: start of centers tape
//Ends: start of pole tape
void centers_to_pole()
{
    //turns right to strattle centers tape
    turn(turn_time, turn_power, m_port_l);
    msleep(1000);

    //drives forward slightly
    drive(200, reg_speed, reg_speed);
    msleep(1000);
	
    //turns perpendicular to pole
    turn(turn_time, turn_power, m_port_l);
    msleep(1000);
	
    //drives to be straight across to pole
    drive(500, reg_speed, reg_speed);
    msleep(1000);
	
    //turns to face the pole
    turn(turn_time, turn_power, m_port_l);
    msleep(1000);

    //drives up to the start of the pole tape
    drive(250, reg_speed, reg_speed);
    msleep(1000);
}

//drives bot from the buildings to the firepole
void buildings_to_pole()
{
    //turns left to strattle centers tape
    turn(turn_time, turn_power, m_port_r);
    msleep(1000);
	
    //drive to start of fire station tape
    drive(1400, r_reg_speed, r_reg_speed);
    msleep(1000);
	
    //turns left to face the firepole
    turn(turn_time, turn_power, m_port_r);	
    msleep(1000);
  
    //drive up to start of buildings tape
    drive(150, reg_speed, reg_speed);
    msleep(1000);
}
