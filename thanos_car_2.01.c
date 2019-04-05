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
