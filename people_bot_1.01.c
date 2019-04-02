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

void scan_centers();

void collect_people();

void deliver_people();

void centers_to_buildings();

void buildings_to_centers();

void opening_sequence();

void first_row();

void second_row();

void third_row();

void fourth_row();

int main()
{
  return 0;
}
