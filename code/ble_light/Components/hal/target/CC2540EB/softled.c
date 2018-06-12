
#include "softled.h"

void softled_init(void) {
    P0DIR |= (BIT1 | BIT2 | BIT3 | BIT4 | BIT5);
    P0_1 = 0;
    P0_2 = 0;
    P0_3 = 0;
    P0_4 = 0;
    P0_5 = 0;
    
}

