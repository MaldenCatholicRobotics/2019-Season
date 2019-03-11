#include <kipr/botball.h>

#include <stdbool.h>

void line_follower(int runtime, int slow_speed, int reg_speed, int fast_speed, int tape_benchmark);

bool fire_scan(double red_benchmark);


//creates global variables to hold the ports of the motors and ir sensors

    int m_port_l = 0;

    int m_port_r = 1;

    int ir_port_l = 1;

    int ir_port_r = 0;

    //holds the channel the red object is held in

    int red_channel = 0;

int main()

{
    line_follower(18, 800, 1200, 1600, 3000);
    //scans the first building for fire

    if(fire_scan(0.1))

    {

        //building 1 is on fire so breaks out of loop

        printf("Building 1 is on fire!\n");

    }

    else

    {

        //building one is not on fire

        //change runtime to figure out distance between first and second buildings

        line_follower(12, 800, 1200, 1600, 3000);

        //checks if building two is on fire

        if(fire_scan(0.1))

        {

            printf("Building 2 is on fire!\n");

        }
        else

        {

            //assumes the third building is on fire so drives from the second to third buildings

            line_follower(12, 800, 1200, 1600, 3000);

            printf("Building 3 is on fire!\n");

        }

    }

    ao();

    return 0;

}

//runtime- the number of tenths of seconds the bot should follow the line

//slow_speed, reg_speed and fast_speed- the motor speeds for going straight and turning

//tape_benchmark- the ir value that decides if the sensor is on the tape or not

void line_follower(int runtime, int slow_speed, int reg_speed, int fast_speed, int tape_benchmark)

{

    //creates a temporary counter to control the while loop

    int counter = 0;

    //keeps the program running for a specified time

    while(counter <= runtime)

    {

        //if both sensors are off tape

        if(analog(ir_port_l) <= tape_benchmark && analog(ir_port_r) <= tape_benchmark)

        {

            //move straight

            mav(m_port_l, reg_speed);

            mav(m_port_r, reg_speed);

            msleep(100);



            //printf("straight");

            // printf("analog left: %i /n", analog(0));

            //printf("analog right: %i /n", analog(1));

        }

        //if the left sensor is off tape but the rigth sensor is on

        else if(analog(ir_port_l) <= tape_benchmark && analog(ir_port_r) > tape_benchmark)

        {

            //turn left

            mav(m_port_l, fast_speed);

            mav(m_port_r, slow_speed);

            msleep(100);



            //printf("turn right");

            //printf("analog left: %i /n", analog(0));

            //printf("analog right: %i /n", analog(1));

        }



        //assumed that the right sensor is off tape but the left is on

        else

        {

            //turn right

            mav(m_port_l, slow_speed);

            mav(m_port_r, fast_speed);

            msleep(100);



            //printf("move left");

            //printf("analog left: %i /n", analog(0));

            //printf("analog right: %i /n", analog(1));

        }

        counter++;

    }

    //shuts motors off

    ao();

}



//red_channel- the channel that the red object is stored in (usually 0)

//red_benchmark- the minimum average confidence for the building to be on fire

bool fire_scan(double red_benchmark) 

{ 

    //create a counter variable for the while loop

    int counter_1 = 0; 



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



    //one snapshot and about one tenth of a second per iteration

    while(counter_1 < 5) 

    { 

        int counter_2 = 0;

        while(counter_2 < 10)

        {

            //creates a counter variable



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



        //drives the bot forward very slowly for one tenth of a second

        //this allows a larger margin of error for the camera

        //comment the following three lines to make code simpler but less affective 

        //mav(m_port_l, 100);

        //mav(m_port_r, 100);

        //msleep(100);

        counter_1++;

    } 



    red_average = red_total/50; 

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

    return 0;

} 
