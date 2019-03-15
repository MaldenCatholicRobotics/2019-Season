#include <kipr/botball.h>
#include <stdbool.h>

//This program follows the half black, half grey line while using the camera to determine which building is on fire

//notes
//servoport for up and down -
//servoport for open and close - 

//FUNCTION DECLARATIONS
void line_follower(int runtime, int slow_speed, int reg_speed, int fast_speed, int tape_benchmark);

bool fire_scan(double red_benchmark);

void claw_change(int op, int wp, int servoport, int speed);

void turn(int time, int power, int port) ;

  //creates global variables to hold the ports of the servos, motors and ir sensors
  int m_port_l = 0;
  int m_port_r = 1;
  int ir_port_l = 1;
  int ir_port_r = 0;
  
  //the current values needed to turn approximately 90 degrees
	int turn_power = 1200;
	int turn_time = 1000;
  
  //holds the channel the red object is held in
  int red_channel = 0;

  //the values of the up, down, open and close positions for the claw servos
	int claw_open = 1450;
	int claw_close = 600;
	int claw_up = 600;
	int claw_down = 0;
  
//MAIN
int main()

{
    //sets inital claw position to down and closed
    enable_servos();
    set_servo_position(0, claw_down);
    set_servo_position(1, claw_close);
    
    //drives to the building1
    line_follower(18, 800, 1200, 1600, 3000);
    
    //scans building 1 for fire
    if(fire_scan(0.1))

    {
        //building 1 is on fire so breaks out of loop
        printf("Building 1 is on fire!\n");
    }

    else
    {
        //building one is not on fire so bot drives to building 2
        //change runtime to figure out distance between buildings 1 and 2
        line_follower(12, 800, 1200, 1600, 3000);

        //checks if building 2 is on fire
        if(fire_scan(0.1))
        {
            printf("Building 2 is on fire!\n");
        }
        else
        {
            //assumes the building 3 is on fire so bot drives to building 3
            //change runtime to figure out the distance between buildings 2 and 3
            line_follower(12, 800, 1200, 1600, 3000);
            printf("Building 3 is on fire!\n");            
        }
    }
    ao();
    return 0;
}

//FUNCTIONS

//runtime- the number of tenths of seconds the bot should follow the line
//slow_speed, reg_speed and fast_speed- the motor speeds for going straight and turning
//tape_benchmark- the ir value that decides if the sensor is on the tape or not
void line_follower(int runtime, int slow_speed, int reg_speed, int fast_speed, int tape_benchmark)
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

//red_benchmark- the minimum average confidence for the building to be on fire
bool fire_scan(double red_benchmark) 
{ 
    //create a counter variable for the while loop
    int counter_1 = 0; 

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

    //one snapshot and about one tenth of a second per iteration
    //adjust the number to number of sets of movement 
    //the number of snaphsots taken will be the numbers used in the conditions of the two while loops multiplied
    while(counter_1 < 5) 
    { 
        //creates a counter variable
        int counter_2 = 0;
        
        //adjust the number to the number of snapshots taken before moving 
        while(counter_2 < 10)
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
        
        //drives the bot forward very slowly for one tenth of a second
        //this allows a larger margin of error for the camera
        //comment the following three lines to make code simpler but less affective 
        //mav(m_port_l, 100);
        //mav(m_port_r, 100);
        //msleep(100);
        counter_1++;
    } 
    red_average = red_total/50; 
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
    return 0;
} 

//op is the original position, wp is the wanted position 
//servoport is the port of the servo and speed is the increment by which the servo position is repeatedly moved 
void claw_change(int op, int wp, int servoport, int speed) 
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

//to turn left, call the right servo port 
//to turn right, call the left servo port 
void turn(int time, int power, int port) 
{ 
  mav(port, power); 
  msleep(time); 
  ao();
} 
