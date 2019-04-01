#include <kipr/botball.h>
#include <stdbool.h>

//notes
//

void line_follower();

void reverse_line_follower();

bool fire_scan(double red_benchmark);

void drive();

void turn();

void drive_until_line();

void servo_change(int op, int wp, int servoport, int speed);

void scan_buildings();

void raise_claw();

void lower_claw();

void deliver_object();

//the servo port values 
int base_port = 2;
int mid_port = 1;
int claw_port = 0;

//the values for servo positions 
int base_down = 1850;
int base_up = 800;
int mid_down = 2000;
int mid_up = 50;
int claw_close = 80;
int claw_open = 80;

//variables for camera use
int red_benchmark = 0.2;
int numpics = 30;

int main()
{
	enable_servos();
	set_servo_position(base_port, base_down);
	set_servo_position(mid_port, mid_down);
	set_servo_position(claw_port, claw_close);
	servo_change(claw_close, claw_open, claw_port, 30);
	raise_claw();
	servo_change(claw_open, claw_close, claw_port, 30);
	lower_claw();
	servo_change(claw_close, claw_open, claw_port, 30);
	disable_servos();
	return 0;
}

//uses the create's cliff sensors to follow a line forwards
void line_follower()
{
	
}

//uses the create's cliff sensors to follow a line backwards
void reverse_line_follower()
{
	
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


//drives create at an input speed for a given time
//SHOULD BE IN ONE NOTE FUNCTION LIBRARY
void drive()
{
	
}

//turns a creats at a certain speed for a certain time
//SHOULD BE IN ONE NOTE FUNCTION LIBRARY
void turn()
{
	
}

//drives until sensors hit a line
void drive_until_line()
{
	
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

//determines on fire building and stops create in front of it
void scan_buildings()
{
	
}

//completely raises the appendage up from the down position
void raise_claw()
{
	servo_change(base_down, base_up, base_port, 30);
	servo_change(mid_down, mid_up, mid_port, 30);
}

//completely lowers the appendage down from the up position
void lower_claw()
{
	servo_change(base_up, base_down, base_port, 30);
	servo_change(mid_up, mid_down, mid_port, 30);
}

//drops an object to the side of the create
void deliver_object()
{
	
}
