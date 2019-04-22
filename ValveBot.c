#include <kipr/botball.h>

void moveFor(int speed, float seconds);
void moveUntilBump(int speed);
void turn(int degrees);
void lineUp(int speed);
void createConnectDetailed();
void printBatteryInfo();

int main()

{
    //wait_for_light(start_light);
    shut_down_in(119); 
    enable_servos();
    //sets inital claw position to up and closed	 
    set_servo_position(servo_port_sweeper, sweeper_start);
    set_servo_position(servo_port_scooper, scooper_down);
    disable_servos();
    return 0;
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
