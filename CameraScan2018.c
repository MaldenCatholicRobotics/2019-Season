#include <kipr/botball.h>
//Programmed by Gavin Sadler
#define LIGHT 0 //Port of the light sensor
#define LIGHTTHRESHOLD 2000 //When the line follower should turn left/right

//Process function

void follow_line(int speed, int distance); //Follows the line for the given distance at the given speed
int analyze_cube(); //Returns 1, 2, or 3 if the camera sees a yellow, red, or green

//Sequence functions

void scan_cubes();
void grab_cube();

//Variables

int yellow_cube_zone = 1; //Zone that the yellow cube is in. Defaults to 1

int main()
{
    
  /*Yellow  |  1
	Red     |  2
	Green   |  3
	|---------|----|
	|         | C3 |
	|         |----|
	|  GAME   | B2 |
	|  BOARD  |----|
	|         | A1 |
	|         |----|
	|         | R  |
	|---------|----| */
    
	int zone_a_color;
	int zone_b_color;
	
	int yellow_cube_zone;
    
    create_disconnect();
    create_connect();
    create_full();
    
    create_drive_straight(100);
    msleep(2000);
    
    follow_line(100, 250);
    
    //scan_cubes();
    
	zone_a_color = analyze_cube();
    printf("%d\n", zone_a_color);
    
    follow_line(100, 600);
    
	zone_b_color = analyze_cube();
    printf("%d\n", zone_b_color);
	
	if (zone_a_color == 1)
	{
		yellow_cube_zone = 1;
	}
	else if (zone_b_color == 1)
	{
		yellow_cube_zone = 2;
	}
	else 
	{
		yellow_cube_zone = 3;
	}
    
    printf("%i\n", yellow_cube_zone);
	
    //turn_left(1000, 3000);*/
               
    return 0;
}

//Sequences

void scan_cubes()
{
    printf("Begin cube scan\n");
    
}

void grab_cube()
{
    
}

//Process function

void follow_line(int speed, int distance)
{
    printf("Line follow begin\n");
    
    create_drive_straight(speed);
    set_create_distance(0);
    
    while (get_create_distance() < distance)
    {
        msleep(10);
    	//printf("%i\t%i\t%i\n", get_create_rfcliff_amt(), get_create_rcliff_amt(), get_create_distance());
        if (get_create_rcliff_amt() < 2000)
        {
            create_drive_direct(speed, speed - speed / 10);
        }
        else if (get_create_rfcliff_amt() < 2000)
        {
            create_drive_direct(speed - speed / 10, speed);
        }
        else
        {
            create_drive_straight(speed);
        }
    }
    
    create_stop();
    
    printf("Line follow end\n");
}

int analyze_cube()
{
	double yellow_confidence;
	double red_confidence;
	double green_confidence;
    
    camera_open_black();
    camera_load_config("db");
    camera_update();
	
    printf("%i\n", get_channel_count());
    
	msleep(100);
    camera_update();
	yellow_confidence = get_object_confidence(0, 0);
    printf("%d\n", get_object_count(0));
	
	msleep(100);
    camera_update();
	red_confidence = get_object_confidence(1, 0);
    printf("%d\n", get_object_count(1));
	
	msleep(100);
    camera_update();
	green_confidence = get_object_confidence(2, 0);
    printf("%d\n", get_object_count(2));
    
    msleep(100);
    camera_update();
    camera_close();
	
    printf("%f\t%f\t%f\n", yellow_confidence, red_confidence, green_confidence);
    
	if ((yellow_confidence > red_confidence) && (yellow_confidence > green_confidence))
	{
		return 1;
	}
	else if (red_confidence > green_confidence)
	{
		return 2;
	}
	else
	{
		return 3;
    }
}
