#include <kipr/botball.h>

const int shovelMotorPort = 0;
const int shovelButtonPort = 0;

const int shovelMotorValue = -3800;

void raiseShovel();
void raiseShovelSpeed(int speed);
void raiseShovelTo(int value);
void lowerShovel();

void moveFor(int speed, float seconds);
void moveUntilBump(int speed);
void turn(int degrees);
void lineUp(int speed);

void createConnectDetailed();
void printBatteryInfo();
void startupRoutine();

int main()
{
    startupRoutine();
    
    moveFor(-100, 1);
    
    lowerShovel();
    
    moveFor(-100, 3);
    turn(-180);
    
    moveUntilBump(200);
    turn(90);
    
    moveFor(-100, 2);
    
    turn(-180);
    
    lineUp(200);
    moveUntilBump(200);
    
    turn(87);
    
    moveFor(-200, 6.75);
    
    msleep(100 * 1000);
    
    turn(-20);
    moveFor(-100, 3);
    turn(20);
    moveFor(-100, 1.5);
    
    msleep(0.25 * 1000);
    
    moveFor(100, 2);
    moveFor(-100, 0.25);
    
    raiseShovelSpeed(750);
    moveFor(-100, 0.25);
    lowerShovel();
    
    moveFor(100, 2);
    
    turn(30);
    moveFor(100, 2);
    turn(-30);
    moveFor(100, 2);
    
    moveUntilBump(200);
    turn(-110);
    
    moveFor(-100, 3.5);
    
    return 0;
}

void raiseShovel()
{
    mav(shovelMotorPort, 1500);
    while ( !digital(shovelButtonPort) ) { } //Wait until the button is pressed ...
    mav(shovelMotorPort, 0);
    cmpc(shovelMotorPort);
}

void raiseShovelSpeed(int speed)
{
    mav(shovelMotorPort, speed);
    while ( !digital(shovelButtonPort) ) { } //Wait until the button is pressed ...
    mav(shovelMotorPort, 0);
    cmpc(shovelMotorPort);
}

void raiseShovelTo(int value)
{
    cmpc(shovelMotorPort);
    mav(shovelMotorPort, 1500);
    while ( gmpc(shovelMotorPort) < value) { } //Wait until the shovel reaches the ground ...
    mav(shovelMotorPort, 0);
}

void lowerShovel()
{
    mav(shovelMotorPort, -1500); 
    while ( gmpc(shovelMotorPort) > shovelMotorValue) { } //Wait until the shovel reaches the ground ...
    mav(shovelMotorPort, 0);
    raiseShovelTo(125);
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

void startupRoutine()
{
    //printBatteryInfo();
    printf("Press R to raise shovel.\n");
    while( !right_button() ) {  msleep(10); }
    printf("Raising shovel . . .\n");
    raiseShovel();
    printf("Shovel raised.\nPress L to connect to create.\n");
    while( !left_button() ) {  msleep(10); }
    createConnectDetailed();
    printf("Ready to operate. Set up PVC coupler and click the R button.\n");
    while( !right_button() ) {  msleep(10); }
    printf("Program commencing in 3 seconds . . .\n");
    msleep(3000);
}
