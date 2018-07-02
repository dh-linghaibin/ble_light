/**************************************************************************************************'
  Filename:       hal_temp112.c
  Revised:        $Date: 2014-09-19 07:58:08 -0700 (Mon, 25 Mar 2013) $
  Revision:       $Revision: 000 $

  Description:    Driver for the temp112 sensor

**************************************************************************************************/

/* ------------------------------------------------------------------------------------------------
*                                          Includes
* ------------------------------------------------------------------------------------------------
*/
#include "hal_temp112.h"
#include <softi2c.h>

/**************************************************************************************************
* @fn          HalHumiInit
*
* @brief       Initialise the humidity sensor driver
*
* @return      none
**************************************************************************************************/
void xw09a_init(void)
{
    IIC_Init();
}

uint16 xw09a_get(void)
{  
    uint16 val;
    start();
    iic_write(0x81); //WRITE i2c
    if (check_ack()==0) {

    }
    val = iic_read();
    val |= iic_read() << 8;
    send_noack();
    stop();
    return val;
}  

touch_name get_touch(void) {
    uint16 val = xw09a_get();
    static uint8 flag[9] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,};
    //四个按钮
    if( (val&0x4000) == 0x4000) { //上课
        flag[0] = 0;
    } else {
        if(flag[0] == 0) {
            flag[0] = 1;
            return ATTEND_CLASS;
        }
    }
    
    if( (val&0x8000) == 0x8000) { //投影
        flag[1] = 0;
    } else {
        if(flag[1] == 0) {
            flag[1] = 1;
            return PROJECTION;
        }
    }
    
    if( (val&0x1) == 0x1) { //自习
        flag[2] = 0;
    } else {
        if(flag[2] == 0) {
            flag[2] = 1;
            return SELF_STUDY;
        }
    }
    
    if( (val&0x2) == 0x2) { //活动
        flag[3] = 0;
    } else {
        if(flag[3] == 0) {
            flag[3] = 1;
            return ACTIVITY;
        }
    }
    //调亮度
    if( (val&0x2000) == 0x2000) {
        flag[4] = 0;
    } else {
        if(flag[4] < 5) {
            flag[4]++;
        } else {
            flag[4] = 0;
            return BRIGHTNESS_ADD;
        }
    }
    
    if( (val&0x1000) == 0x1000) {
        flag[5] = 0;
    } else {
        if(flag[5] < 5) {
            flag[5]++;
        } else {
            flag[5] = 0;
            return BRIGHTNESS_SUB;
        }
    }
    
    if( (val&0x8) == 0x8) {
        flag[6] = 0;
    } else {
        if(flag[6] < 5) {
            flag[6]++;
        } else {
            flag[6] = 0;
            return COLOR_TRMP_SUB;
        }
    }
    
    if( (val&0x4) == 0x4) {
        flag[7] = 0;
    } else {
        if(flag[7] < 5) {
            flag[7]++;
        } else {
            flag[7] = 0;
            return COLOR_TRMP_ADD;
        }
    }
    
    //开关
    if( (val&0x10) == 0x10) {
        flag[8] = 0;
    } else {
        if(flag[8] == 0) {
            flag[8] = 1;
            return SWITCH;
        }
    }
    return TN_NULL;
}

/* ------------------------------------------------------------------------------------------------
*                                           Private functions
* -------------------------------------------------------------------------------------------------
*/


/*********************************************************************
*********************************************************************/

