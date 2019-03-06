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



    while(counter < 5) 

    { 

        camera_update(); 

        red_confidence = get_object_confidence(red_channel, 0); 

        red_total += red_confidence; 

        msleep(100); 

        counter++; 

    } 

    red_average = red_total/5; 

    camera_update(); 

    camera_close(); 

    printf("%f\n", red_average);   

    if (red_average >= red_benchmark) 

    { 

        printf("THE BUILDING IS ON FIRE");
        return true; 

    } 

    else 

    { 
        printf("THE BUILDING IS NOT ON FIRE");
        return false; 

    } 
    return 0;
} 

