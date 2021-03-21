void line_follower(float runtime, int tape_benchmark)
{
    //creates temporary counters to control the while loops
    int counter1 = 0;
    int counter2 = 0;
    //keeps the program running for a specified time
    while(counter1 <= quick_correct*runtime)
    {
        //if sensor on tape
        if(analog(f_ir_port) >= black_tape)
        {
            //veer slightly left
            mav(m_port_l, 1200);
            mav(m_port_r, 900);
	    //checks position every 1/100 seconds
            msleep(10);
        }
        //assumed that sensor is off tape
        else
        {
            //veer slightly right
            mav(m_port_l, 900);
            mav(m_port_r, 1200);
            //checks position every 1/100 seconds
            msleep(10);
        }
	//increments counter
        counter1++;
    }
	
    //keeps the program running for a specified time
    while(counter2 <= straighten_correct*runtime)
    {
        //if sensor on tape
        if(analog(f_ir_port) >= black_tape)
        {
            //veer slightly left
            mav(m_port_l, 1200);
            mav(m_port_r, 1150);
	    //checks position every 1/100 seconds
            msleep(10);
        }
        //assumed that sensor is off tape
        else
        {
            //veer slightly right
            mav(m_port_l, 1150);
            mav(m_port_r, 1200);
            //checks position every 1/100 seconds
            msleep(10);
        }
	//increments counter
        counter2++;
    }
    //turns off motors
     freeze(m_port_l);
    freeze(m_port_r);   
}
