#include <kipr/botball.h>
#include <stdbool.h>

//notes
//

//FUNCTION DECLARATIONS
void line_follower(int runtime, int tape_benchmark);

void reverse_line_follower(int runtime, int tape_benchmark);

bool fire_scan(double red_benchmark);

void servo_change(int op, int wp, int servoport, int speed);

void turn(int time, int power, int port) ;

void drive(int time, int l_power, int r_power); 

void drive_until_line(int tape_benchmark, int l_power, int r_power);

void scan_buildings();

void scan_centers();

void collect_object();

void deliver_bridge();

void deliver_ground();

void deliver_roof();

void pull_firefighter();

void pole_to_centers()

  //creates global variables to hold the ports of the servos, motors and ir sensors
  int m_port_l = 0;
  int m_port_r = 1;
  int ir_port_l = 1;
  int ir_port_r = 0;
  int servo_port_arm = 0;
  int servo_port_claw = 2;
  
  //the current values needed to turn approximately 90 degrees
  int turn_power = 1200;
  int turn_time = 900;
  
  //holds the channel the red object is held in
  int red_channel = 0;

  //the number of pictures to be taken in the fire_scan function
  int num_pics = 40;

  //holds the color benchmarks 
  int black_tape = 3000;
  double fire_benchmark = 0.1;
  
  //speeds for the line follower
  int reg_speed = 1200;
  int fast_speed = 1600;
  int slow_speed = 800;

  //speeds for the reverse line follower
  int r_reg_speed = (0-1200);
  int r_fast_speed = (0-1600);
  int r_slow_speed = (0-800);

  //the values for the claw and arm positions
  int claw_open = 1450;
  int claw_close = 1075;
  int arm_up = 802;
  int arm_down = 250;
  int claw_slightly_open = 1400;
  int arm_very_high = 1100;
  
  //variables to store data collected by the bot
  int building_on_fire;
  int center_on_fire;
  int safe_center;
  int safe_building_right;
  int safe_building_left;
  
//MAIN
int main()

{
    
    enable_servos();
    //sets inital claw position to up and closed	  
    set_servo_position(servo_port_arm, arm_up);
    set_servo_position(servo_port_claw, claw_close);
    pull_firefighter();
    pole_to_centers();
    scan_centers();
    deliver_roof();
    disable_servos();
    return 0;
}

//FUNCTIONS

//follows the tape using two ir sensors
//precodnitions: the sensor must be either on either side of the tape or one needs to be touching the tape.
//runtime- the number of tenths of seconds the bot should follow the line
//tape_benchmark- the ir value that decides if the sensor is on the tape or not
void line_follower(int runtime, int tape_benchmark)
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

            //printf("straight");
            // printf("analog left: %i /n", analog(0));
            //printf("analog right: %i /n", analog(1));

        }
        else if(analog(ir_port_l) >= tape_benchmark && analog(ir_port_r) >= tape_benchmark)
        {
            //move straight
            mav(m_port_l, reg_speed);
            mav(m_port_r, reg_speed);
            msleep(100);

            //printf("straight");
            // printf("analog left: %i /n", analog(0));
            //printf("analog right: %i /n", analog(1));

        }
        //if the left sensor is off tape but the rigth sensor is on
        else if(analog(ir_port_l) <= tape_benchmark && analog(ir_port_r) > tape_benchmark)

        {
            //turn left
            mav(m_port_l, fast_speed);
            mav(m_port_r, slow_speed);
            msleep(100);
            
            //printf("turn right");
            //printf("analog left: %i /n", analog(0));
            //printf("analog right: %i /n", analog(1));
        }
        //assumed that the right sensor is off tape but the left is on
        else
        {
            //turn right
            mav(m_port_l, slow_speed);
            mav(m_port_r, fast_speed);
            msleep(100);

            //printf("move left");
            //printf("analog left: %i /n", analog(0));
            //printf("analog right: %i /n", analog(1));
        }
        counter++;
    }
    //shuts motors off
    ao();
}

//follows the tape backwards using two ir sensors
//precodnitions: the sensor must be either on either side of the tape or one needs to be touching the tape.
//runtime- the number of tenths of seconds the bot should follow the line
//tape_benchmark- the ir value that decides if the sensor is on the tape or not
void reverse_line_follower(int runtime, int tape_benchmark)
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
            mav(m_port_l, r_reg_speed);
            mav(m_port_r, r_reg_speed);
            msleep(100);
        }
        
        //if both sensors are off tape
        else if(analog(ir_port_l) >= tape_benchmark && analog(ir_port_r) >= tape_benchmark)
        {
            //move straight
            mav(m_port_l, r_reg_speed);
            mav(m_port_r, r_reg_speed);
            msleep(100);
        }
        //if the left sensor is off tape but the rigth sensor is on
        else if(analog(ir_port_r) <= tape_benchmark && analog(ir_port_l) > tape_benchmark)

        {
            //turn left
            mav(m_port_l, r_fast_speed);
            mav(m_port_r, r_slow_speed);
            msleep(100);
        }
        //assumed that the right sensor is off tape but the left is on
        else
        {
            //turn right
            mav(m_port_l, r_slow_speed);
            mav(m_port_r, r_fast_speed);
            msleep(100);

            //printf("move left");
            //printf("analog left: %i /n", analog(0));
            //printf("analog right: %i /n", analog(1));
        }
        counter++;
    }
    //shuts motors off
    ao();
}

//determines if a building or medical center is on fire and returns a boolean value
//precondition: the camera is pointed at the building
//red_benchmark- the minimum average confidence for the building to be on fire
bool fire_scan(double red_benchmark) 
{ 
    printf("yuh");
    //create a counter variable for the while loop
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
    printf("yuh2");
    //prints number of channels
    printf("%i\n", get_channel_count()); 
    
    msleep(100); 
    camera_update(); 

    //prints number of objects in channel
    printf("%d\n", get_object_count(0));    
    msleep(100); 

    //one snapshot and about one tenth of a second per iteration
    //creates a buffer period for the camera to set up
    while(counter_1 < 10) 
    { 
           //creates a counter variable
           camera_update();
        printf("huh");
	    msleep(100);
	    counter_1++;
    }   
      //creates a running tally of the confidence in each snapshot
    while(counter_2 < num_pics)
    {
            camera_update(); 

            //gets the confidence that the building is on fire
            red_confidence = get_object_confidence(red_channel, 0);

            //print statement for debugging
            //printf("red_confidence: %f\n", red_confidence);

            //adds all snapshot confidences together
            red_total += red_confidence; 
            msleep(100); 
            counter_2++; 
      }
        
    red_average = red_total/num_pics; 
    camera_update(); 
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

//moves a servo to a desired position at a certain speed
//op- the starting position of the servo
//wp- the end positon of the servo
//servoport- the port of the servo 
//speed- the increment by which the servo position is repeatedly moved 
void servo_change(int op, int wp, int servoport, int speed) 
{   
    if (op > wp) 
    { 
        while(op > wp)  
        {   
        //increments the position
        op -= speed;  
            
        //sets the servo position to the incremented position
        set_servo_position(servoport, op);   
        
        //to debug issues with the code 
        //printf("opening or closing claw/"); 

        msleep(100); 

        }   
    }
    else 
    { 
        while(wp > op)  
        {   
            //increments the position
            op += speed; 

            //sets the servo position to the incremented position
            set_servo_position(servoport, op);   

            //to debug issues with the code 
            //printf("opening or closing claw/");//to debug issues with the code  

            msleep(100); 
         }   
    } 
}    

//turns the bot at a specified speed and time
//to turn left, call the right servo port 
//to turn right, call the left servo port 
//speed- the power with which the wheel turns
//time- the larger the time the larger the degree of the turn
//port- the port of the motor turning
void turn(int time, int power, int port) 
{ 
  mav(port, power); 
  msleep(time); 
  ao();
} 

//a simple function to drive the bot forwards
//USE NEGATIVE POWER TO DRIVE BACKWARDS1
//l_power and r_power- the speed for the two different motors (allows for different motor rates)
//time- the duration of driving
void drive(int time, int l_power, int r_power) 
{ 
    motor(m_port_l, l_power); 
    motor(m_port_r, r_power); 
    msleep(time); 
    ao(); 
} 

//drives the bot forward until the ir sensors reach tape
//l_power and r_power- the speed at which the two motors drive (allows for different motor rates)
//tape_benchmark- the analog values of the tape needed
void drive_until_line(int tape_benchmark, int l_power, int r_power)
{
	//creates a  variable to kick the code out of the loop
	int statement = 0; 
	//while the bot has not reached the tape
	while(statement == 0)
	{
		//if both sensors are off tape
		if(analog(ir_port_l) <= tape_benchmark && analog(ir_port_r) <= tape_benchmark)
		{
		    //move straight
		    mav(m_port_l, l_power);
		    mav(m_port_r, r_power);
		    msleep(100);
		}
		//the bot has reached the tape
		else
		{
			//kick the code out of the loop
			statement = 1;
			msleep(100);
		}
		ao();
	}
}

//finds which of the three buildings is on fire, stops in front of that building and saves the building number to the global variable
//precodnitions: bot starts at the cross between the tape in front of the center and the buildings tape 
//with the ball right where the tape meets
void scan_buildings()
{
    //drives to the building 1
    line_follower(18, black_tape);
    
    //scans building 1 for fire
    if(fire_scan(fire_benchmark))

    {
        //building 1 is on fire so breaks out of loop
        printf("Building 1 is on fire!\n");
	building_on_fire = 1;
	safe_building_left = 2;
	safe_building_right = 3;
    }

    else
    {
        //building one is not on fire so bot drives to building 2
        //change runtime to figure out distance between buildings 1 and 2
        line_follower(12, black_tape);

        //checks if building 2 is on fire
        if(fire_scan(fire_benchmark))
        {
            printf("Building 2 is on fire!\n");
 		building_on_fire = 2;
		safe_building_left = 1;
	        safe_building_right = 3;
        }
        else
        {
            //assumes the building 3 is on fire so bot drives to building 3
            //change runtime to figure out the distance between buildings 2 and 3
            line_follower(12, black_tape);
            printf("Building 3 is on fire!\n"); 
		building_on_fire = 3;
		safe_building_left = 1;
	        safe_building_right = 2;
        }
    }
    ao();
}

//finds out which medical center is on fire, stops in front of it and stores the center number in the variable
void scan_centers()	
{
    //drives to center
    line_follower(5, black_tape);
    
    //scans center 1 for fire
    if(fire_scan(fire_benchmark))
    {
        //center 1 is on fire so breaks out of loop
        printf("Medical Center 1 is on fire!\n");
	center_on_fire = 1;
	safe_center = 2;
    }

    else
    {
        set_servo_position(servo_port_arm, arm_very_high);
        //center 1 is not on fire so bot drives to center 2
        //change runtime to figure out distance between centers 1 and 2
        line_follower(9, black_tape);
        
        //Assumes that center 2 is on fire
        printf("Medical Center 2 is on fire!\n");   
	center_on_fire = 2;
	safe_center = 1;
        
    }
    ao();
}

//picks up an object using the claw
//preconditions: the claw is up and closed
void collect_object()
{
	//opens claw
	servo_change(claw_close, claw_open, servo_port_claw, 20);
	//arms descends
	servo_change(arm_up, arm_down, servo_port_arm, 20);
	//claw closes
	servo_change(claw_open, claw_close, servo_port_claw, 20);
	//arm ascends
	servo_change(arm_down, arm_up, servo_port_arm, 20);
}

//puts a firefighter onto the skybridge
void deliver_bridge()
{
    //turn left so the bot is facing the skybridge
    turn(turn_time, turn_power, m_port_r);
    //drive until the firefighter is just over the skybridge
    drive(300, reg_speed, reg_speed);
    msleep(1000);
    //slowly and slightly opens the claw to drop the firefighter onto the skybrdige
    servo_change(claw_close, claw_slightly_open, servo_port_claw, 10);
    msleep(1000);
    //drives back the same distance as it drove forwards earlier
    drive(300, r_reg_speed, r_reg_speed);
}

//puts an object in front of a building or medical center
void deliver_ground()
{
	//set the arm to an extremely high position to avoid hitting the center
	    set_servo_position(servo_port_arm, arm_very_high);
	    msleep(1000);
	    //follows the line backwards shortly
	    reverse_line_follower(7, black_tape);
	    msleep(1000);
	    //turns to face the center
	    turn(turn_time+200, turn_power, m_port_r);	    
         drive(350, r_reg_speed, r_reg_speed);
	//lowers claw to a down position
         servo_change(arm_very_high, arm_down, servo_port_arm, 20);
	    //slowly and slightly opens claw to release object
	    servo_change(claw_close, claw_slightly_open, servo_port_claw, 20);  
        servo_change(arm_down, arm_up, servo_port_arm, 20);
}

//puts a firefighter on top of the medical center's roof
void deliver_roof()
{
     //if center 1 is on fire
     if(center_on_fire == 1)
    {
	     //set the arm to an extremelt high position to avoid hitting the center
	    set_servo_position(servo_port_arm, arm_very_high);
	    msleep(1000);
	    //follows the line backwards shortly
	    reverse_line_follower(6, black_tape);
	    msleep(1000);
	    //turns to face the center
	    turn(turn_time+100, turn_power, m_port_r);
	    //drives until the firefighter is just above the center
	    drive(250, reg_speed, reg_speed);
	    msleep(1000);
	    //slowly and slightly opens claw to release firefighter onto roof
	    servo_change(claw_close, claw_slightly_open, servo_port_claw, 10);
	    msleep(1000);
	    //drives backwards the same dstance it drove forwards
	    drive(250, r_reg_speed, r_reg_speed);
    }
    //center 2 is on fire
    else
    {
	    //same things as above but different adjustments and values
	    set_servo_position(servo_port_arm, arm_very_high);
	    msleep(1000);
	    reverse_line_follower(4, black_tape);
	    msleep(1000);
	    turn(turn_time+100, turn_power, m_port_r);
	    drive(350, reg_speed, reg_speed);
	    msleep(1000);
	    servo_change(claw_close, claw_slightly_open, servo_port_claw, 10);
	    msleep(1000);
	    drive(350, r_reg_speed, r_reg_speed);
    }
}

//pulls out the bottom firefighter from the pole
void pull_firefighter()
{
	//opens claw
	servo_change(claw_close, claw_open, servo_port_claw, 20);
	//lowers arm to ground level
	servo_change(arm_up, arm_down, servo_port_arm, 20);
	//drives so the claw surrounds the firefighter
	line_follower(5, black_tape);
	//claw closes around firefighter, securing it
	servo_change(claw_open, claw_close, servo_port_claw, 20);
	//bot drives backwards with firefighter
	reverse_line_follower(5, black_tape);
	//arm lifts up with firefighter in it
	servo_change(arm_down, arm_up, servo_port_arm, 20);
}

//drives from the pole to the medical centers
void pole_to_centers()
{
	//turns right to face tape in front of centers
	turn(turn_time, turn_power, m_port_l);
	//drives up to tape in front of centers in position to scan centers/ deliver firefighter/go to buildings
	drive(500, reg_speed, reg_speed);
	//line_follower(5, black_tape);
}
