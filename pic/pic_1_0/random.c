#include "random.h"

int random_init()
{
    //Initialize timer
    //ON - timer on
    //TCKPS prescaler
    //PR1 - count to this value before triggerin the event
    //TMR - timern
    //80 mhz / 256 = 0.3125 mhz = 312.5 khz = 31.25 k svängningar / 0.1 sec
    //This processor does seems to be just a little bit faster...
    //Changing to 29120
    PR2     =   0xfff;
    
    //prescaler connect empty source empty
    //     000        0     0      0     0
    T2CON  =  (T2CON & ~0x7f);
    
    //Starting timer
    T2CON  |= 1 << 15;
    return 0;
    
}

int get_random_int(int max_val)
{
    return TMR2 % max_val;
    
}
