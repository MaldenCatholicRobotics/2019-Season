#include <kipr/botball.h>

void servo_change(int op, int wp, int servoport, int speed);
void moveFor(int speed, float seconds);
void moveUntilBump(int speed);
void turn(int degrees);
void lineUp(int speed);
void createConnectDetailed();
void printBatteryInfo();
void grab_valve();

//the port of the light sensor used to start the bot
int start_light = ;

//the servo port for the claw
int claw_port = ;

//the servo port for moving the claw up and down
int lift_port = ;

//the servo port for turning the claw
int turn_port = ;

claw_open = ;
claw_close = ;

lift_up = ;
lift_down = ;

turn_vert = ;
turn_horiz = ;

int main()

{
    //wait_for_light(start_light);
    shut_down_in(119); 
    enable_servos();
    create_connect_detailed();
    printBatteryInfo();
	
    //sets inital claw position to up, closed and vertical	 
    set_servo_position(claw_port, claw_close);
    set_servo_position(lift_port, lift_up);
    set_servo_position(turn_port, turn_vert);
	
    disable_servos();
    return 0;
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

void moveFor(int speed, float seconds)
{
    create_stop();
    create_drive_straight(speed);
    msleep(seconds * 1000);
    create_stop();
}

void moveUntilBump(int speed)
{
    create_drive_straight(speed);
    printf("Moving until bump . . .\n");
    while ( !(get_create_lbump() || get_create_rbump())) { msleep(10); }
    printf("Bump detected, reversing . . .\n");
    create_stop();
    create_drive_straight(-(speed/abs(speed)) * 100);
    msleep(1000);
    create_stop();
}

void turn(int degrees)
{
    create_stop();
    if (degrees < 1)
    {
        create_spin_CCW(100);
    }
    else
    {
        create_spin_CW(100);
    }
    msleep(abs(1625 / 90 * degrees));
    create_stop();
}

void lineUp(int speed)
{
    while (1)
    {
        int lcliffamt = get_create_lcliff_amt();
        int rcliffamt = get_create_rcliff_amt();
        
        printf("lcliffamt: %d\trcliffamt: %d\n", lcliffamt, rcliffamt);
        
        if (lcliffamt > 1600 && rcliffamt > 1600)
        {
            create_drive_straight(speed);
        }
        else if (lcliffamt < 1600 && rcliffamt > 1600)
        {
            create_drive_direct(0, speed);
        }
        else if (lcliffamt > 1600 && rcliffamt < 1600)
        {
            create_drive_direct(speed, 0);
        }
        else
        {
            create_stop();
            break;
        }
        
        msleep(10);
    }
}

void createConnectDetailed()
{
    printf("Connecting to create . . .\n");
    create_connect();
    int state = get_create_battery_charging_state();
    int tempC = get_create_battery_temp();
    int tempF = 1.8*tempC + 32;
    int charge = get_create_battery_charge();
    int capacity = get_create_battery_capacity();
    printf("Create Battery Information:\n");
    printf("\tState: %d\n\tTemperature: %dC (%dF)\n\tCharge: %d/%d\n", state, tempC, tempF, charge, capacity);
}

void printBatteryInfo()
{
    int powerLevel = power_level();
    printf("Battery level: %d\n", powerLevel);
}

void grab_valve()
{
	//opens claw
	servo_change(claw_close, claw_open, claw_port, 30);
	//lowers claw
	servo_change(lift_up, lift_down, lift_port, 30);
	//closes claw
	servo_change(claw_open, claw_close, claw_port, 30);
	//lifts claw up
	servo_change(lift_down, lift_up, lift_port, 30);
	//turns the valve horizontally
	servo_change(turn_vert, turn_horiz, turn_port, 30);
}
