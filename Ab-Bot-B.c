#include <kipr/botball.h>

const int leftWheelPort = 3;
const int rightWheelPort = 2;
const int clawServoPort = 0;
const int distanceSensorPort = 1;
const int lightSensorPort = 0;

void setSpeeds(int speedLeft, int speedRight);
void setSpeed(int speed);

void move(int leftMotorSpeed, int rightMotorSpeed, int milliseconds);
void moveStraight(int speed, int milliseconds);

void clawOpen();
void clawClose();
void clawPosition(int position);

int ms(float seconds); // Converts seconds to milliseconds

int main()
{
    
    while( analog(lightSensorPort) < 2000)
    {
        setSpeed(1000);
    }
    
    setSpeed(0);
    
    move(1000,-1000,0.5);
    return 0;
}

void setSpeeds(int speedLeft, int speedRight)
{
    mav(leftWheelPort, speedLeft);
    mav(rightWheelPort, speedRight);
}

void setSpeed(int speed)
{
	setSpeeds(speed, speed);
}

void move (int leftWheelSpeed, int rightWheelSpeed, int milliseconds)
{
    setSpeeds(leftWheelSpeed, rightWheelSpeed);
    msleep(milliseconds);
    setSpeed(0);
}

void moveStraight(int speed, int milliseconds)
{
    move(speed, speed, milliseconds);
}

void clawPosition(int position)
{
    enable_servo(clawServoPort);
    set_servo_position(clawServoPort, position);
    msleep(1000);
    disable_servo(clawServoPort);
}

void clawOpen()
{
    clawPosition(2047);
}

void clawClose()
{
    clawPosition(0);
}

int ms(float seconds)
{
    return seconds * 1000;
}