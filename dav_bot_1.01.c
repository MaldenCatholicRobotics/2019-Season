#include <kipr/botball,h>

void line_follower();

void reverse_line_follower();

void servo_change(int op, int wp, int servoport, int speed);

void raise_claw();

void lower_claw();

//the servo port values 
int base_port = ;
int mid_port = ;
int claw_port = ;

//the values for servo positions 
int base_down = ;
int base_up = ;
int mid_down = ;
int mid_up = ;
int claw_close = ;
int claw_open = ;

int main()

{	
	set_servo_position(
	raise_claw();
	return 0;
}

//uses the create's cliff sensors to follow a line backwards
void line_follower()
{
	
}

//uses the create's cliff sensors to follow a line backwards
void reverse_line_follower()
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

//completely raises the appendage up from the down position
void raise_claw()
{
	servo_change(base_down, base_up, base_port, 30);
	servo_change(mid_down, mid_up, mid_port, 30);
	servo_change(claw_close, claw_open, claw_port, 30);
}

void lower_claw()
{
	
}
