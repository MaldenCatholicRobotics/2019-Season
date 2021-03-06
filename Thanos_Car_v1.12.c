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

void deliver_bridge_left();

void deliver_ground();

void deliver_roof();

void pull_firefighter();

void pole_to_centers();

void pole_to_buildings();

void centers_to_pole();

void buildings_to_pole();

void firefighter_roof();

void opening_sequence();

void firefighter_right_bridge();

void firetruck();

void fire_fighter_left_bridge();

void firefighter_ground();


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
    opening_sequence();
	scan_buildings();
    deliver_ground();
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
    while(counter <= 2*runtime)
    {
        //if sensor on tape
        if(analog(ir_port) >= 3000)
        {
            //move left
            mav(m_port_l, 1200);
            mav(m_port_r, 1000);
            msleep(50);
        }
        //assumed that sensor is off tape
        else
        {
            //turn right
            mav(m_port_l, 1000);
            mav(m_port_r, 1200);
            msleep(50);
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
    while(counter <= 2*runtime)
    {
        //if sensor on tape
        if(analog(r_ir_port) >= 3000)
        {
            //move left
            mav(m_port_l, (0-1200));
            mav(m_port_r, (0-1000));
            msleep(50);
        }
        //assumed that the sensor is off tape
        else
        {
            //turn right
            mav(m_port_l, (0-900));
            mav(m_port_r, (0-1000));
            msleep(50);
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
    //shuts off motors
    ao();
}

//finds out which medical center is on fire, stops in front of it and stores the center number in the variable
void scan_centers()	
{
    //drives to center
    line_follower(3, black_tape);

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
    servo_change(claw_close, claw_open, servo_port_claw, 30);
    //arms descends
    servo_change(arm_up, arm_down, servo_port_arm, 30);
    //claw closes
    servo_change(claw_open, claw_close, servo_port_claw, 30);
    //arm ascends
    servo_change(arm_down, arm_up, servo_port_arm, 30);
}

//puts a firefighter onto the skybridge
void deliver_bridge()
{
    //turn left so the bot is facing the skybridge
    turn(turn_time+50, turn_power, m_port_r);
    //drive until the firefighter is just over the skybridge
    drive(200, reg_speed, reg_speed);
    //slowly and slightly opens the claw to drop the firefighter onto the skybrdige
    servo_change(claw_close, claw_slightly_open, servo_port_claw, 10);
    //drives back the same distance as it drove forwards earlier
    drive(575, r_reg_speed, r_reg_speed);
    msleep(1000);
    //turns back onto tape
    turn(turn_time-200, turn_power, m_port_l);
    msleep(1000);
    //drives back to start of scan_buildings()
    if(building_on_fire == 1)
    {
        reverse_line_follower(29, black_tape);
        printf("drive back 1");
        msleep(1000);
    }
    else if(building_on_fire == 2)
    {
        reverse_line_follower(45, black_tape);
        msleep(1000);
    }
    else
    {
        reverse_line_follower(53, black_tape);
        msleep(1000);
    }
}

void deliver_bridge_left()
{
    //turn left so the bot is facing the skybridge
    turn(turn_time, turn_power, m_port_r);
    //drive until the firefighter is just over the skybridge
    drive(200, reg_speed, reg_speed);
    //slowly and slightly opens the claw to drop the firefighter onto the skybrdige
    servo_change(claw_close, claw_slightly_open, servo_port_claw, 10);
    //drives back the same distance as it drove forwards earlier
    drive(585, r_reg_speed, r_reg_speed);
    msleep(1000);
    //turns back onto tape
    turn(turn_time-200, turn_power, m_port_l);
    msleep(1000);
    if(building_on_fire == 1)
    {
        reverse_line_follower(7, black_tape);
    }
    else if(building_on_fire == 2)
    {
        reverse_line_follower(27, black_tape);
    }
    else
    {
        reverse_line_follower(39, black_tape);
    }
}
//puts an object in front of a building or medical center
void deliver_ground()
{
    //follows the line backwards shortly
    reverse_line_follower(5, black_tape);
    msleep(1000);
    //turns to face the center
    turn(turn_time, turn_power, m_port_r);	    
    
    //lowers claw to a down position
    servo_change(arm_up, arm_down, servo_port_arm, 20);
    drive(75, reg_speed, reg_speed);
    //slowly and slightly opens claw to release object
    servo_change(claw_close, claw_slightly_open, servo_port_claw, 20);
    drive(50, r_reg_speed, r_reg_speed);
    servo_change(arm_down, arm_up, servo_port_arm, 20);
    drive(350, r_reg_speed, r_reg_speed);
    msleep(1000);
    turn(turn_time-100, turn_power, m_port_l);
    
}

//puts a firefighter on top of the medical center's roof
void deliver_roof()
{
    //if center 1 is on fire
    if(center_on_fire == 1)
    {
        //set the arm to an extremelt high position to avoid hitting the center
        msleep(1000);
        //follows the line backwards shortly
        reverse_line_follower(6, black_tape);
        msleep(1000);
        //turns to face the center
        turn(turn_time+100, turn_power, m_port_r);
        //drives until the firefighter is just above the center
        drive(100, reg_speed, reg_speed);
        msleep(1000);
        //slowly and slightly opens claw to release firefighter onto roof
        servo_change(claw_close, claw_slightly_open, servo_port_claw, 10);
        msleep(1000);
        //drives backwards the same dstance it drove forwards
        drive(250, r_reg_speed, r_reg_speed);
        msleep(3000);
        turn(turn_time-200, turn_power, m_port_l);
        msleep(3000);
        reverse_line_follower(1, black_tape);
        msleep(3000);
    }
    //center 2 is on fire
    else
    {
        //same things as above but different adjustments and values to account for different center position
        set_servo_position(servo_port_arm, arm_very_high);
        msleep(3000);
        reverse_line_follower(4, black_tape);
        msleep(3000);
        turn(turn_time+100, turn_power, m_port_r);
        drive(350, reg_speed, reg_speed);
        msleep(3000);
        servo_change(claw_close, claw_slightly_open, servo_port_claw, 10);
        msleep(1000);
        drive(350, r_reg_speed, r_reg_speed);
        turn(turn_time+100, turn_power, m_port_l);
        reverse_line_follower(9, black_tape);
    }
}

//pulls out the bottom firefighter from the pole
void pull_firefighter()
{
    //opens claw
    //servo_change(claw_close, claw_open, servo_port_claw, 20);
    //lowers arm to ground level
    servo_change(arm_up, arm_down, servo_port_arm, 20);
    //drives up to surround firefighter
    drive(250, 1200, 1200);
    //claw closes around firefighter, securing it
    servo_change(claw_open, claw_close, servo_port_claw, 20);
    msleep(2000);
    //bot drives backwards with firefighter
    reverse_line_follower(2, black_tape);
    drive(100, (0-1200), (0-1200));
    //arm lifts up with firefighter in it
    servo_change(arm_down, arm_up, servo_port_arm, 20);
}

//drives from the pole to the medical centers
void pole_to_centers()
{
    //turns right to face tape in front of centers
    turn(turn_time-100, turn_power, m_port_l);
    msleep(1000);
    //drives up to tape in front of centers in position to scan centers/ deliver firefighter/go to buildings
    //drive(500, reg_speed, reg_speed);
    line_follower(5, black_tape);
    msleep(1000);
}

//drives the bot from the firepole to the buildings
void pole_to_buildings()
{
    //turns right to face tape in front of centers
    turn(turn_time-100, turn_power, m_port_l);
    msleep(1000);
    //drives up to tape in front of centers in position to scan centers/ deliver firefighter/go to buildings
    //drive(500, reg_speed, reg_speed);
    line_follower(8, black_tape);
    msleep(1000);
    //turn onto building tape
    turn(turn_time-200, turn_power, m_port_l);
    msleep(1000);
}

//drives bot from medical centers to the firepole
void centers_to_pole()
{
    //turns right to strattle centers tape
    turn(turn_time-75, turn_power, m_port_l);
    msleep(1000);
    drive(200, reg_speed, reg_speed);
    msleep(1000);
    turn(turn_time-200, turn_power, m_port_l);
    msleep(1000);
    drive(500, reg_speed, reg_speed);
    msleep(1000);
    turn(turn_time-50, turn_power, m_port_l);
    msleep(1000);
    drive(250, reg_speed, reg_speed);
    msleep(1000);
}

//drives bot from the buildings to the firepole
void buildings_to_pole()
{
    msleep(1000);
    //turns left to strattle centers tape
    turn(turn_time, turn_power, m_port_r);
    msleep(1000);
    //drive to start of fire_station tape
    drive(1400, r_reg_speed, r_reg_speed);
    msleep(1000);
    //turns left to face the firepole
    turn(turn_time, turn_power, m_port_r);	
    msleep(1000);
    drive(150, reg_speed, reg_speed);
    msleep(1000);
}


//brings a firefighter from the pole to a medical center roof
void firefighter_roof()
{
    //collects a firefighter
    pull_firefighter();
    //drives to the medical centers
    pole_to_centers();
    //finds which medical center is on fire
    scan_centers();
    //delivers the firefighter to the roof of the on fire center
    deliver_roof();
    centers_to_pole();
}

//brings the bot from the box to the buildings
void opening_sequence()
{
    //drives to left end of starting box
    drive_until_line(3000, 1200, 1200);
    //turn onto left end of starting box
    turn(turn_time+150, turn_power, m_port_l);
    //drives back
    drive(100, r_reg_speed/2, r_reg_speed/2);
    //picks up the firefighter
    collect_object();
    //drives past starting box edge
    line_follower(3, black_tape);
    //drive to the buildings tape
    drive(800, 1200, 1200);
    //turn onto the building tape
    turn(turn_time+250, turn_power, m_port_l);
    msleep(1000);
    //drive to the starting point of scan_buildings
    reverse_line_follower(22, black_tape);
    msleep(1000);
}

//brings a firefighter and drops it on the skybridge to the right of the on fire building then returns to the firepole
void firefighter_right_bridge()
{
    //determines which building is on fire and stops in front of it
    scan_buildings();
    //puts firefighter onto that bridge
    deliver_bridge();
    //drives to the firepole
    buildings_to_pole();
}

void firetruck()
{	

}

void firefighter_left_bridge()
{
    pull_firefighter();
    pole_to_buildings();
    //drives up to the left bridge
    if(building_on_fire == 1)
    {
        line_follower(4, black_tape);
        msleep(1000);
        line_follower(15, black_tape);
        msleep(1000);
        reverse_line_follower(15, black_tape);
    }
    else if(building_on_fire == 2)
    {
        line_follower(27, black_tape);
    }
    else
    {
        line_follower(39, black_tape);
    }
    msleep(1000);
    //puts firefighter onto that bridge
    deliver_bridge_left();
    //drives to the firepole
    buildings_to_pole();
}

//takes a firefighter from the pole and delivers it to the ground of the on fire medical centers
void firefighter_ground()
{
    pull_firefighter();
    pole_to_centers();
    deliver_ground();
    centers_to_pole();
}
