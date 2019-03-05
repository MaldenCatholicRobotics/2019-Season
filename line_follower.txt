#include <kipr/botball,h>
//Programmed by Daniel Rodriguez
//analog port0=left sensor
//analog port1=right sensor
//motor port 0=left motor
//motor port 1=right motor


int main()

{
	
	
	
	
	motor( 0, 50);//robots moves fwd for a given amount of time
	motor (1, 50);
	msleep();
			
	
	while (analog(0) == 0)//lines 20-30 are code that will detect the black tape from the left side of the robot 
	{						
		motor( 0, 50);			
		msleep();			
		
		if (analog(0) >=x)//given the value of the  tape
		{
			motor( 0, -50);
			msleep(1500); 
		}				
	}
	
	
		
	while (analog(1) == 0)//lines 34-44 are code that will detect the black tape from the right side of the robot 
	{
		motor( 1, 50);
		msleep();
		
		if (analog(1) >= x)// given the value of the tape
		{
			motor( 1, -50);
			msleep(1500);
		}
	}
	
	
			
	return 0;
	
}
