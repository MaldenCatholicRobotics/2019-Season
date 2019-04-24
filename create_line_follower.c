#include <kipr/botball.h>

void line_follower(int runtime, int tape_value, int speed)

int main()

{
    line_follower(10, black_tape);
    return 0;
}

void line_follower(int runtime, int tape_value, int speed)
{
  int counter = 0;
  while(runtime < counter)
  {
    int lcliffamt = get_create_lcliff_amt();
    int rcliffamt = get_create_rcliff_amt();
    
    if(lcliffamt >  tape_value && rcliffamt > tape_value)
    {
      create_drive_connect(speed, speed);
    }
    if(lcliffamt >  tape_value && rcliffamt > tape_value)
    {
      create_drive_connect(speed, speed);
    }
    
