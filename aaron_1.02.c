#include <kipr/botball.h>
#include <stdbool.h>




//VARIABLE2
//motor ports
int r_port = 3;
int l_port = 2;
//sensor port
int front_ir = 0;
int back_ir = 1;
//value for the black tape
int black_tape = 2000;
//stuff for fire scan
int num_pics = 20;
double fire_benchmark = 0.2;
int red_channel = 0;
//VARIABLES
int reg_speed = 1200;
int r_reg_speed = -1200;
int slow_speed = 600;
int r_slow_speed = -600;
//servo ports
int lower_pt = 0;
//servo movement 
int lowerptup = 800;










//FUNCTIONS


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


void turnRight(int time, int rightm_port, int power)
{
    mav(rightm_port, power);
    msleep(time);
    ao();
}

void turnLeft(int time, int leftm_port, int power)
{
    mav(leftm_port, power);
    msleep(time);
    ao();
}

void driveStraight( int time, int l_power, int r_power)
{
    mav(3, l_power);
    mav(2, r_power);
    msleep(time);
    ao();
}

void driveReverse( int time, int r_l_power, int r_r_power)
{
	mav(3, r_l_power);
    mav(2, r_r_power);
    msleep(time);
    ao();
}

void turnArd( int time, int l_power, int r_r_power)
{
    mav( 3, l_power);
    mav( 2, r_r_power);
    msleep(time);
    ao();
}




void line_follower(int runtime, int tape_benchmark)
{
    //creates a temporary counter to control the while loop
    int counter = 0;
    //keeps the program running for a specified time
    //runtime is multiplied by ten to account for 1/100 second iterations but 1/10 second inputs
    while(counter <= 10*runtime)
    {
        //if sensor on tape
        if(analog(front_ir) >= 2000)
        {
            //veer slightly left
            mav(l_port, 1200);
            mav(r_port, 1000);
	    //checks position every 1/100 seconds
            msleep(10);
        }
        //assumed that sensor is off tape
        else
        {
            //veer slightly right
            mav(l_port, 1000);
            mav(r_port, 1200);
            //checks position every 1/100 seconds
            msleep(10);
        }
	//increments counter
        counter++;
    }
    //turns off motors
    ao();
}


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
        turnArd(4700, 500, -500);
        line_follower(12, black_tape);
        turnLeft(1400, r_port, reg_speed);
        driveReverse(2000, r_slow_speed, r_slow_speed); 
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
    enable_servo(0);
    set_servo_position(0, 800);
    //makes robot go to tpe
    driveStraight(2800, reg_speed, reg_speed);



    //makes robot go to the med. cent.
    turnLeft(1300, r_port, reg_speed);
    ao();
    msleep(2000);
    
    line_follower(25, black_tape);
    
    turnRight(1300, l_port, reg_speed);
    
    fire_scan(0.2);
    
    printf("program over");
    
    
    
    
    
    



    //makes robot get into position for scn med. cent.
    //turnLeft(900, l_port, reg_speed);
    //makes robot get into position for scn med. cent.
    //driveStraight(10500, reg_speed, reg_speed);
    //makes robot get into position for scn med. cent. 
    //turnArd(1200, reg_speed, r_reg_speed);



    return 0;
}




















