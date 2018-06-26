
#include "softled.h"

void softled_init(void) {
    P0DIR |= (BIT1 | BIT2 | BIT3 | BIT4 | BIT5);
    P0_1 = 1;
    P0_2 = 1;
    P0_3 = 1;
    P0_4 = 1;
    P0_5 = 1;
    
    I2CWC |= 0X83;
    I2CIO = 0x03;
    
    I2CIO = 0x03;
    
    P0DIR &= ~BIT0;//∞¥≈•
    P0INP &= ~BIT0; //P0…œ¿≠       
}


uint8 softled_get_key(void) {
    static uint8 flag = 0;
    if( (P0 & BIT0) == BIT0 ) {
        flag = 0;
    } else {
        if(flag == 0) {
            flag = 1;
            return 1;
        }
    }
    return 0;
}

void softled_enable_set(uint8 flag) {
    if(flag) {
        I2CIO = 0x03;
    } else {
        I2CIO = 0x00;
    }
}

void softled_kill_set(uint8 flag) {
    if(flag) {
        P0_1 = 0;
    } else {
        P0_1 = 1;
    }
}

void softled_mode_set(uint8 mode) {
    switch(mode) {
        case 0:{
            P0_2 = 1;
            P0_3 = 1;
            P0_4 = 1;
            P0_5 = 1;
        } break;
        case 1:{
            P0_2 = 0;
            P0_3 = 1;
            P0_4 = 1;
            P0_5 = 1;
        } break;
        case 2:{
            P0_2 = 1;
            P0_3 = 0;
            P0_4 = 1;
            P0_5 = 1;
        } break;
        case 3:{
            P0_2 = 1;
            P0_3 = 1;
            P0_4 = 0;
            P0_5 = 1;
        } break;
        case 4:{
           P0_2 = 1;
            P0_3 = 1;
            P0_4 = 1;
            P0_5 = 0;
        } break;
    }
}

