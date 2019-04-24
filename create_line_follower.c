#include <kipr/botball.h>

void line_follower(int runtime, int tape_value, int speed)

int main()

{
    line_follower(10, black_tape, 1500);
    return 0;
}

void line_follower(int runtime, int tape_value, int speed)
{
  int counter = 0;
  while(runtime < counter)
  {
    int lcliffamt = get_create_lcliff_amt();
    int rcliffamt = get_create_rcliff_amt();
    
    if(lcliffamt <  tape_value && rcliffamt < tape_value)
    {
      create_drive_connect(speed, speed);
    }
    else if(lcliffamt <  tape_value && rcliffamt > tape_value)
    {
      create_drive_connect(speed/2, speed);
    }
    else 
    {
      create_drive_connect(speed, speed/2);
    }
    msleep(10);
    counter += 10;
  }
}
