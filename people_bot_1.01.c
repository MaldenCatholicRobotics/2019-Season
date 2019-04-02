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

void scan_centers();

void collect_people();

void deliver_people();

void centers_to_buildings();

void buildings_to_centers();

void opening_sequence();

void first_row();

void second_row();

void third_row();

void fourth_row();

  //VARIABLES
  //creates global variables to hold the ports of the servos, motors and ir sensors
  int m_port_l = 0;
  int m_port_r = 1;
  int ir_port = 0;
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

  //holds the benchmarks for the camera
  int black_tape = 3000;
  double fire_benchmark = 0.2;
  
  //speeds for the line follower
  int reg_speed = 1200;
  int fast_speed = 1600;
  int slow_speed = 800;

  //speeds for the reverse line follower
  int r_reg_speed = (0-1200);
  int r_fast_speed = (0-1600);
  int r_slow_speed = (0-800);

  //the values for the claw and arm positions
  int claw_open = 1400;
  int claw_close = 550;
  int arm_up = 800;
  int arm_down = 250;
  int claw_slightly_open = 1200;
  int arm_very_high = 1100;
  
  //variables to store data collected by the bot
  int center_on_fire;
  int safe_center;

int main()
{
  opening_sequence();
  first_row();
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
    while(counter <= runtime)
    {
        //if sensor on tape
        if(analog(ir_port) >= 3000)
        {
            //move left
            mav(m_port_l, 1200);
            mav(m_port_r, 1050);
            msleep(100);
        }
        //assumed that sensor is off tape
        else
        {
            //turn right
            mav(m_port_l, 1050);
            mav(m_port_r, 1200);
            msleep(100);
        }
        counter++;
    }
	ao();
}

//follows the edge of a line using a single ir sensor
//runtime- the number of tenths of seconds the bot should follow the line
//tape_benchmark- the ir value that decides if the sensor is on the tape or not
void reverse_line_follower(int runtime, int tape_benchmark)
{
     //creates a temporary counter to control the while loop
    int counter = 0;
    //keeps the program running for a specified time
    while(counter <= runtime)
    {
        //if sensor on tape
        if(analog(r_ir_port) >= 3000)
        {
            //move left
            mav(m_port_l, (0-1200));
            mav(m_port_r, (0-1050));
            msleep(100);
        }
        //assumed that the sensor is off tape
        else
        {
            //turn right
            mav(m_port_l, (0-1050));
            mav(m_port_r, (0-1200));
            msleep(100);
        }
        counter++;
    }
	ao();
}

//determines if a building or medical center is on fire and returns a boolean value
//precondition: the camera is pointed at the building
//red_benchmark- the minimum average confidence for the building to be on fire
bool fire_scan(double red_benchmark) 
{ 
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
    //prints number of channels
    printf("%i\n", get_channel_count()); 
    msleep(100); 
    camera_update(); 

    //prints number of objects in channel
    printf("%d\n", get_object_count(0));    
    msleep(100); 

    //one picture and about one tenth of a second per iteration
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
//power- the power with which the wheel turns
//time- the larger the time the larger the degree of the turn
//port- the port of the motor turning
void turn(int time, int power, int port) 
{ 
  mav(port, power); 
  msleep(time); 
  ao();
} 

//a simple function to drive the bot forwards
//USE NEGATIVE POWER TO DRIVE BACKWARDS
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
		if(analog(ir_port) <= tape_benchmark)
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
		ao();
	}
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

//picks up a row of people using the claw
//preconditions: the claw is up and closed
void collect_people()
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


void deliver_people()
{
	
}

void centers_to_buildings()
{
	
}

void buildings_to_centers()
{
	
}

//brings the bot from the box to the buildings
void opening_sequence()
{
    //drives to left end of starting box
    drive_until_line(black_tape, reg_speed, reg_speed);
    //turn onto left end of starting box
    turn(turn_time+150, turn_power, m_port_l);
    //drives past starting box edge
    line_follower(3, black_tape);
    //drive to the buildings tape
    drive(750, reg_speed, reg_speed);
    //turn onto the building tape
    turn(turn_time+250, turn_power, m_port_l);
    msleep(1000);
    //drive to the starting point of scan_buildings
    reverse_line_follower(20, black_tape);
    msleep(1000);
}

void first_row()
{
	//drive up to row of people
	line_follower(5, black_tape);
	//turn left to face people
	turn(turn_time, turn_power, m_port_r);
	//drive up to people
	drive(300, reg_speed, reg_speed);
	//picks up the row of people
	collect_people();
	//drive back to line
	drive(300, r_reg_speed, r_reg_speed);
	//turns back onto line
	turn(turn_time, turn_power, m_port_l);
	//drives back to start of buildings
	reverse_line_follower(5, black_tape);
	buildings_to_centers();
	centers_scan();
	deliver_people();
	centers_to_buildings();
 }

void second_row()
{
	
}

void third_row()
{
	
}

void fourth_row()
{
	
}
