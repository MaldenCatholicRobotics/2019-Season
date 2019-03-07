#include <kipr/botball.h>
#include <stdbool.h>
bool fire_scan(int red_channel, double red_benchmark);  



int main()   

{   

    fire_scan(0, .1);

    return 0;   

}   

bool fire_scan(int red_channel, double red_benchmark) 

{ 

    int counter = 0; 

    double red_confidence; 

    double red_total; 

    double red_average; 

    camera_open_black(); 

    camera_load_config("fire"); 

    camera_update(); 

    printf("%i\n", get_channel_count()); 

    msleep(100); 

    camera_update(); 

    printf("%d\n", get_object_count(0));    

    msleep(100); 



    while(counter < 50) 

    { 

        camera_update(); 

        red_confidence = get_object_confidence(red_channel, 0); 
        //printf("red_confidence: %f\n", red_confidence);

        red_total += red_confidence; 

        msleep(100); 

        counter++; 

    } 

    red_average = red_total/50; 

    camera_update(); 

    camera_close(); 

    printf("red average:%f\n", red_average);   

    if (red_average >= red_benchmark) 

    { 

        printf("FIRE\n");
        return true; 

    } 

    else 

    { 
        printf("SAFE\n");
        return false; 

    } 
    return 0;
} 

