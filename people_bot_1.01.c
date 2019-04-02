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

int main()
{
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
