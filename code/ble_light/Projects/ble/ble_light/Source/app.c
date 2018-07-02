
#include "app.h"

#include "OSAL.h"
#include "ioCC2541.h"
#include "nrf24l01.h"
#include "softled.h"
#include "hal_temp112.h"
#include "OSAL_Clock.h"
#include "hal_adc.h"
#include "osal_snv.h"
#include "peripheral.h"
#include "hal_i2c.h"
#include "stdlib.h"

#define VOID (void)
#define GUA_LED_TEST_EVT_PERIOD 100
static uint8 GUA_TaskID; // Task ID for internal task/event processing
static void GUA_ProcessOSALMsg(osal_event_hdr_t *pMsg);

#define BLE_NVID_USER_CFG_START         0x80  //!< Start of the USER Configuration NV IDs
#define BLE_NVID_USER_CFG_END           0x89  //!< End of the USER Configuration NV IDs

uint8 snvData = 0x00;

void GUA_Init(uint8 task_id)
{
    GUA_TaskID = task_id;
    nRF24L01P_Init();
    nRF24L01P_RX_Mode();
    
    //Initiallize temp112 chip
    softled_init();
    xw09a_init();
    
    //P0DIR &= ~BIT7; 
    HalAdcSetReference(HAL_ADC_REF_AVDD);
    HalAdcInit();
    
    osal_snv_read(BLE_NVID_USER_CFG_START, 1, &snvData);
    if(snvData != 0x55) {
        snvData = 0x55;
        osal_snv_write(BLE_NVID_USER_CFG_START, 1, &snvData);
    }
    
    
    //softled_mode_set(0);

    osal_set_event(GUA_TaskID, GUA_START_DEVICE_EVT);
}

typedef enum _device_name {
    BACT_LIGHT = 1, //杀菌+照明
    LIGHT = 2, //照明
    SQUARE_LAMP = 3, //方灯
    BLACK_LIGHT  = 4, //黑板灯
} device_name;

static uint8 pair_device[][7] = { 
    {BACT_LIGHT,0x02,0x00,0x00,0x00,0x00,0x00}, /* 类型 地址 值 是否需要更新 */
    {BLACK_LIGHT,0x01,0x00,0x00,0x00,0x00,0x00},
    {BACT_LIGHT,0x03,0x00,0x00,0x00,0x00,0x00},
};

//static uint8 work_num = 0xff;

static uint8 open_flag = 0;
static uint8 light_mode = 4;
static uint16 last_light_intensty = 8000;
static uint8 kill_flag = 0;
static uint8 light_set = 0;

static uint8 work_num = 0;

uint16 GUA_ProcessEvent(uint8 task_id, uint16 events)
{
    VOID task_id;
    if (events & SYS_EVENT_MSG)
    {
        uint8 *pMsg;
        if ((pMsg = osal_msg_receive(GUA_TaskID)) != NULL)
        {
            GUA_ProcessOSALMsg((osal_event_hdr_t *)pMsg);
            VOID osal_msg_deallocate(pMsg);
        }
        return (events ^ SYS_EVENT_MSG);
    }
    if (events & GUA_START_DEVICE_EVT)
    {
        osal_start_timerEx(GUA_TaskID, GUA_LED_TEST_EVT, GUA_LED_TEST_EVT_PERIOD);
        return (events ^ GUA_START_DEVICE_EVT);
    }
    if (events & GUA_LED_TEST_EVT)
    {
        
        switch(get_touch()) {
            case ATTEND_CLASS: {
                if(open_flag == 1) {
                    light_mode = 1;
                    softled_mode_set(1);
                    
                    last_light_intensty = 8000;
                    light_set = 100;
                    
                    for(int i = 0; i < ( sizeof(pair_device)/sizeof(pair_device[0]) ); i++ ) {
                        if(pair_device[i][0] == BACT_LIGHT) {
                            pair_device[i][3] = 100;
                            pair_device[i][4] = 5;
                        }
                        if(pair_device[i][0] == BLACK_LIGHT) {
                            pair_device[i][3] = 100;
                            pair_device[i][4] = 5;
                        }
                    }
                }
            } break;
            case PROJECTION: {
                if(open_flag == 1) {
                    light_mode = 2;
                    softled_mode_set(2);
                    
                    last_light_intensty = 8000;
                    light_set = 50;
                    
                    for(int i = 0; i < ( sizeof(pair_device)/sizeof(pair_device[0]) ); i++ ) {
                        if(pair_device[i][0] == BACT_LIGHT) {
                            pair_device[i][3] = 50;
                            pair_device[i][4] = 5;
                        }
                        if(pair_device[i][0] == BLACK_LIGHT) {
                            pair_device[i][3] = 0;
                            pair_device[i][4] = 5;
                        }
                    }
                }
            } break;
            case SELF_STUDY: {
                if(open_flag == 1) {
                    light_mode =3;
                    softled_mode_set(3);
                    
                    last_light_intensty = 8000;
                    light_set = 100;
                    
                    for(int i = 0; i < ( sizeof(pair_device)/sizeof(pair_device[0]) ); i++ ) {
                        if(pair_device[i][0] == BACT_LIGHT) {
                            pair_device[i][3] = 100;
                            pair_device[i][4] = 5;
                        }
                        if(pair_device[i][0] == BLACK_LIGHT) {
                            pair_device[i][3] = 0;
                            pair_device[i][4] = 5;
                        }
                    }
                }
            } break;
            case ACTIVITY: {
                if(open_flag == 1) {
                    light_mode = 4;
                    softled_mode_set(4);
                    
                    last_light_intensty = 8000;
                    light_set = 80;
                    
                    for(int i = 0; i < ( sizeof(pair_device)/sizeof(pair_device[0]) ); i++ ) {
                        if(pair_device[i][0] == BACT_LIGHT) {
                            pair_device[i][3] = 80;
                            pair_device[i][4] = 5;
                        }
                        if(pair_device[i][0] == BLACK_LIGHT) {
                            pair_device[i][3] = 0;
                            pair_device[i][4] = 5;
                        }
                    }
                }
            } break;
            case BRIGHTNESS_ADD: {
                if(open_flag == 1) {
                    
                    if( light_set+5 < 100 ) {
                        light_set += 5;
                    } else {
                        light_set = 100;
                    }
                    
                    switch(light_mode) {
                        case 1: {
                            for(int i = 0; i < ( sizeof(pair_device)/sizeof(pair_device[0]) ); i++ ) {
                                if(pair_device[i][0] == BACT_LIGHT) {
                                    pair_device[i][3] = light_set;
                                    pair_device[i][4] = 5;
                                }
                                if(pair_device[i][0] == BLACK_LIGHT) {
                                    pair_device[i][3] = light_set;
                                    pair_device[i][4] = 5;
                                }
                            }
                        } break;
                        case 2: {
                            for(int i = 0; i < ( sizeof(pair_device)/sizeof(pair_device[0]) ); i++ ) {
                                if(pair_device[i][0] == BACT_LIGHT) {
                                    pair_device[i][3] = light_set;
                                    pair_device[i][4] = 5;
                                }
                            }
                        } break;
                        case 3: {
                            for(int i = 0; i < ( sizeof(pair_device)/sizeof(pair_device[0]) ); i++ ) {
                                if(pair_device[i][0] == BACT_LIGHT) {
                                    pair_device[i][3] = light_set;
                                    pair_device[i][4] = 5;
                                }
                            }
                        } break;
                        case 4: {
                            for(int i = 0; i < ( sizeof(pair_device)/sizeof(pair_device[0]) ); i++ ) {
                                if(pair_device[i][0] == BACT_LIGHT) {
                                    pair_device[i][3] = light_set;
                                    pair_device[i][4] = 5;
                                }
                            }
                        } break;
                    }
                }
            } break;
            case BRIGHTNESS_SUB: {
                if(open_flag == 1) {
                    
                    if( (light_set-5) > 20 ) {
                        light_set -= 5;
                    } else {
                        light_set = 20;
                    }
                    
                    switch(light_mode) {
                        case 1: {
                            for(int i = 0; i < ( sizeof(pair_device)/sizeof(pair_device[0]) ); i++ ) {
                                if(pair_device[i][0] == BACT_LIGHT) {
                                    pair_device[i][3] = light_set;
                                    pair_device[i][4] = 5;
                                }
                                if(pair_device[i][0] == BLACK_LIGHT) {
                                    pair_device[i][3] = light_set;
                                    pair_device[i][4] = 5;
                                }
                            }
                        } break;
                        case 2: {
                            for(int i = 0; i < ( sizeof(pair_device)/sizeof(pair_device[0]) ); i++ ) {
                                if(pair_device[i][0] == BACT_LIGHT) {
                                    pair_device[i][3] = light_set;
                                    pair_device[i][4] = 5;
                                }
                            }
                        } break;
                        case 3: {
                            for(int i = 0; i < ( sizeof(pair_device)/sizeof(pair_device[0]) ); i++ ) {
                                if(pair_device[i][0] == BACT_LIGHT) {
                                    pair_device[i][3] = light_set;
                                    pair_device[i][4] = 5;
                                }
                            }
                        } break;
                        case 4: {
                            for(int i = 0; i < ( sizeof(pair_device)/sizeof(pair_device[0]) ); i++ ) {
                                if(pair_device[i][0] == BACT_LIGHT) {
                                    pair_device[i][3] = light_set;
                                    pair_device[i][4] = 5;
                                }
                            }
                        } break;
                    }
                }
            } break;
            case COLOR_TRMP_ADD: {
                if(open_flag == 1) {

                    if( light_set+5 < 100 ) {
                        light_set += 5;
                    } else {
                        light_set = 100;
                    }
                    
                    switch(light_mode) {
                        case 1: {
                            for(int i = 0; i < ( sizeof(pair_device)/sizeof(pair_device[0]) ); i++ ) {
                                if(pair_device[i][0] == BACT_LIGHT) {
                                    pair_device[i][3] = light_set;
                                    pair_device[i][4] = 5;
                                }
                                if(pair_device[i][0] == BLACK_LIGHT) {
                                    pair_device[i][3] = light_set;
                                    pair_device[i][4] = 5;
                                }
                            }
                        } break;
                        case 2: {
                            for(int i = 0; i < ( sizeof(pair_device)/sizeof(pair_device[0]) ); i++ ) {
                                if(pair_device[i][0] == BACT_LIGHT) {
                                    pair_device[i][3] = light_set;
                                    pair_device[i][4] = 5;
                                }
                            }
                        } break;
                        case 3: {
                            for(int i = 0; i < ( sizeof(pair_device)/sizeof(pair_device[0]) ); i++ ) {
                                if(pair_device[i][0] == BACT_LIGHT) {
                                    pair_device[i][3] = light_set;
                                    pair_device[i][4] = 5;
                                }
                            }
                        } break;
                        case 4: {
                            for(int i = 0; i < ( sizeof(pair_device)/sizeof(pair_device[0]) ); i++ ) {
                                if(pair_device[i][0] == BACT_LIGHT) {
                                    pair_device[i][3] = light_set;
                                    pair_device[i][4] = 5;
                                }
                            }
                        } break;
                    }
                }
            } break;
            case COLOR_TRMP_SUB: {
                if(open_flag == 1) {
                    
                    if( (light_set-5) > 20 ) {
                        light_set -= 5;
                    } else {
                        light_set = 20;
                    }
                    
                    switch(light_mode) {
                        case 1: {
                            for(int i = 0; i < ( sizeof(pair_device)/sizeof(pair_device[0]) ); i++ ) {
                                if(pair_device[i][0] == BACT_LIGHT) {
                                    pair_device[i][3] = light_set;
                                    pair_device[i][4] = 5;
                                }
                                if(pair_device[i][0] == BLACK_LIGHT) {
                                    pair_device[i][3] = light_set;
                                    pair_device[i][4] = 5;
                                }
                            }
                        } break;
                        case 2: {
                            for(int i = 0; i < ( sizeof(pair_device)/sizeof(pair_device[0]) ); i++ ) {
                                if(pair_device[i][0] == BACT_LIGHT) {
                                    pair_device[i][3] = light_set;
                                    pair_device[i][4] = 5;
                                }
                            }
                        } break;
                        case 3: {
                            for(int i = 0; i < ( sizeof(pair_device)/sizeof(pair_device[0]) ); i++ ) {
                                if(pair_device[i][0] == BACT_LIGHT) {
                                    pair_device[i][3] = light_set;
                                    pair_device[i][4] = 5;
                                }
                            }
                        } break;
                        case 4: {
                            for(int i = 0; i < ( sizeof(pair_device)/sizeof(pair_device[0]) ); i++ ) {
                                if(pair_device[i][0] == BACT_LIGHT) {
                                    pair_device[i][3] = light_set;
                                    pair_device[i][4] = 5;
                                }
                            }
                        } break;
                    }
                }
            } break;
            case SWITCH: {
                if(open_flag == 0) {
                    open_flag = 1;
                    softled_mode_set(light_mode);
                    
                    switch(light_mode) {
                        case 1: {
                            for(int i = 0; i < ( sizeof(pair_device)/sizeof(pair_device[0]) ); i++ ) {
                                if(pair_device[i][0] == BACT_LIGHT) {
                                    pair_device[i][3] = 100;
                                    pair_device[i][4] = 5;
                                }
                                if(pair_device[i][0] == BLACK_LIGHT) {
                                    pair_device[i][3] = 100;
                                    pair_device[i][4] = 5;
                                }
                            }
                        } break;
                        case 2: {
                            for(int i = 0; i < ( sizeof(pair_device)/sizeof(pair_device[0]) ); i++ ) {
                                if(pair_device[i][0] == BACT_LIGHT) {
                                    pair_device[i][3] = 50;
                                    pair_device[i][4] = 5;
                                }
                                if(pair_device[i][0] == BLACK_LIGHT) {
                                    pair_device[i][3] = 0;
                                    pair_device[i][4] = 5;
                                }
                            }
                        } break;
                        case 3: {
                            for(int i = 0; i < ( sizeof(pair_device)/sizeof(pair_device[0]) ); i++ ) {
                                if(pair_device[i][0] == BACT_LIGHT) {
                                    pair_device[i][3] = 100;
                                    pair_device[i][4] = 5;
                                }
                                if(pair_device[i][0] == BLACK_LIGHT) {
                                    pair_device[i][3] = 0;
                                    pair_device[i][4] = 5;
                                }
                            }
                        } break;
                        case 4: {
                            for(int i = 0; i < ( sizeof(pair_device)/sizeof(pair_device[0]) ); i++ ) {
                                if(pair_device[i][0] == BACT_LIGHT) {
                                    pair_device[i][3] = 80;
                                    pair_device[i][4] = 5;
                                }
                            }
                        } break;
                    }
                    
                } else {
                    open_flag = 0;
                    softled_mode_set(0);
                    
                    kill_flag = 0;
                    softled_kill_set(kill_flag);
                    
                    for(int i = 0; i < ( sizeof(pair_device)/sizeof(pair_device[0]) ); i++ ) { /* 关闭所有 */
                        if(pair_device[i][0] == BACT_LIGHT) {
                            pair_device[i][3] = 0;
                            pair_device[i][4] = 5;
                        }
                        if(pair_device[i][0] == BLACK_LIGHT) {
                            pair_device[i][3] = 0;
                            pair_device[i][4] = 5;
                        }
                    }
                    
                }
                softled_enable_set(open_flag);
            } break;
        }
        //杀菌
        if(softled_get_key()) {
            if(open_flag == 1) {
                kill_flag = kill_flag==0 ? 1:0;
                softled_kill_set(kill_flag);
                
                for(int i = 0; i < ( sizeof(pair_device)/sizeof(pair_device[0]) ); i++ ) {
                    if(pair_device[i][0] == BACT_LIGHT) {
                        pair_device[i][4] = 5;
                    }
                }
                
            }
        } else {
            
        }
        
        uint8 buf[32] = {0};
        if(!nRF24L01P_RxPacket(buf)) {
            if( buf[6] == (uint8)(buf[1] + buf[2] + buf[3] + buf[4] + buf[5]) ) { /* 累加校验 */
//                kill_flag = kill_flag==0 ? 1:0;
//                softled_kill_set(kill_flag);
                for(int i = 0; i < ( sizeof(pair_device)/sizeof(pair_device[0]) ); i++ ) {
                    if( (pair_device[i][1] == buf[1]) && (pair_device[i][2] == buf[2]) ) {
                        if(buf[4] == 0x88) {
                            if(pair_device[i][4] != 5) {
                                pair_device[i][4] = 0;
                            }
                        }
                    }
                }
            }
			buf[0] = 0;	
		}
        
        if(open_flag == 1) {
            int light_intensty = HalAdcRead( HAL_ADC_CHANNEL_7, HAL_ADC_RESOLUTION_14 );
            if( (light_intensty - 2000) > 0 ) {
                light_intensty -= 2000;
            } else {
                light_intensty = 0;
            }
            
            static uint8 light_intensty_flag = 0;
            
            if(abs(light_intensty - last_light_intensty) > 400) {
                if(light_intensty_flag < 20) {
                    light_intensty_flag++;
                } else {
                    last_light_intensty = light_intensty;
                    uint8 intensty = (uint8)(light_intensty/50.00);
                    uint8 val = 0;
                    if( (100-intensty) > 0 ) {
                        val = 100-intensty;
                    }
                    if(intensty > 100) {
                        intensty = 100;
                    }
                    switch(light_mode) {
                        case 1: {
                            if(val > light_set) {
                                val = light_set;
                            }
                            
                            if(val < 16) {
                                val = 16;
                            }
                            for(int i = 0; i < ( sizeof(pair_device)/sizeof(pair_device[0]) ); i++ ) {
                                if(pair_device[i][0] == BACT_LIGHT) {
                                    pair_device[i][3] = val;
                                    pair_device[i][4] = 5;
                                }
                                if(pair_device[i][0] == BLACK_LIGHT) {
                                    pair_device[i][3] = val;
                                    pair_device[i][4] = 5;
                                }
                            }
                        } break;
                        case 2: {
                            if(val > light_set) {
                                val = light_set;
                            }
                            
                            if(val < 16) {
                                val = 16;
                            }
                            for(int i = 0; i < ( sizeof(pair_device)/sizeof(pair_device[0]) ); i++ ) {
                                if(pair_device[i][0] == BACT_LIGHT) {
                                    pair_device[i][3] = val;
                                    pair_device[i][4] = 5;
                                }
                            }
                        } break;
                        case 3: {
                            if(val > light_set) {
                                val = light_set;
                            }
                            
                            if(val < 16) {
                                val = 16;
                            }
                            for(int i = 0; i < ( sizeof(pair_device)/sizeof(pair_device[0]) ); i++ ) {
                                if(pair_device[i][0] == BACT_LIGHT) {
                                    pair_device[i][3] = val;
                                    pair_device[i][4] = 5;
                                }
                            }
                        } break;
                        case 4: {
                            if(val > light_set) {
                                val = light_set;
                            }
                            
                            if(val < 16) {
                                val = 16;
                            }
                            for(int i = 0; i < ( sizeof(pair_device)/sizeof(pair_device[0]) ); i++ ) {
                                if(pair_device[i][0] == BACT_LIGHT) {
                                    pair_device[i][3] = val;
                                    pair_device[i][4] = 5;
                                }
                            }
                        } break;
                    }
                }
            } else {
                light_intensty_flag = 0;
            }
        }
        
        uint8 cmd_flag = 0;
        for(int i = work_num; i < ( sizeof(pair_device)/sizeof(pair_device[0]) ); i++ ) {
            if(pair_device[i][4] == 5) {
                pair_device[i][4] = 4;

                uint8 t_buf[32] = {0};
                t_buf[0] = 0x3a;
                t_buf[1] = pair_device[i][1];
                t_buf[2] = pair_device[i][2];
                t_buf[3] = 0x01;
                t_buf[4] = pair_device[i][3];
                t_buf[5] = kill_flag;
                t_buf[6] = t_buf[1] + t_buf[2] + t_buf[3] + t_buf[4] + t_buf[5];
                t_buf[7] = 0x0a;
                nRF24L01P_TX_Mode();
                nRF24L01P_TxPacket(t_buf);
                t_buf[0] = 0;
                nRF24L01P_RX_Mode();
                
                cmd_flag = 1;
                break;
            } else if(pair_device[i][4] == 4) {
                if(pair_device[i][5] < 2) {
                    pair_device[i][5]++;
                } else {
                    pair_device[i][5] = 0;
                    pair_device[i][4] = 5;
                    if( work_num <  ( sizeof(pair_device)/sizeof(pair_device[0]) ) ) {
                        work_num++;
                    } else {
                        work_num = 0;
                    }
                }
                cmd_flag = 1;
                //break;
            }
        }
        
        if(cmd_flag == 0) {
            work_num = 0;
        }
        
//        for(int i = 0; i < ( sizeof(pair_device)/sizeof(pair_device[0]) ); i++ ) {
//            if(pair_device[i][4] == 5) {
//                pair_device[i][4] = 4;
//
//                uint8 t_buf[32] = {0};
//                t_buf[0] = 0x3a;
//                t_buf[1] = pair_device[i][1];
//                t_buf[2] = pair_device[i][2];
//                t_buf[3] = 0x01;
//                t_buf[4] = pair_device[i][3];
//                t_buf[5] = kill_flag;
//                t_buf[6] = t_buf[1] + t_buf[2] + t_buf[3] + t_buf[4] + t_buf[5];
//                t_buf[7] = 0x0a;
//                nRF24L01P_TX_Mode();
//                nRF24L01P_TxPacket(t_buf);
//                t_buf[0] = 0;
//                nRF24L01P_RX_Mode();
//            } else if(pair_device[i][4] == 4) {
//                if(pair_device[i][5] < 3) {
//                    pair_device[i][5]++;
//                } else {
//                    pair_device[i][5] = 0;
//                    pair_device[i][4] = 5;
//                    if(pair_device[i][6] < 3) {
//                        pair_device[i][6]++;
//                    } else {
//                        pair_device[i][6] = 0;
//                        pair_device[i][4] = 0;
//                    }
//                }
//                break;
//            }
//        }
     
//        uint8 tempbuf[10]={0};  
//        UTCTimeStruct *Ti;  
//        osalTimeUpdate();    
//        osal_ConvertUTCTime(Ti,osal_getClock());  
        
        
//        int v = 0;
//        v = HalAdcRead ( HAL_ADC_CHANNEL_7, HAL_ADC_RESOLUTION_14 );
//        if(v == 100) {
//            tempbuf[0]=(Ti->hour)/10;  
//            tempbuf[1]=(Ti->hour)%10;  
//            tempbuf[2]=(Ti->minutes)/10;  
//            tempbuf[3]=(Ti->minutes)%10+'0';  
//            tempbuf[4]=(Ti->seconds)/10+'0';  
//            tempbuf[5]=(Ti->seconds)%10+'0';  
//        }
        
        osal_start_timerEx(GUA_TaskID, GUA_LED_TEST_EVT, GUA_LED_TEST_EVT_PERIOD);
        //test
        return (events ^ GUA_LED_TEST_EVT);
    }
    return 0;
}

static void GUA_ProcessOSALMsg(osal_event_hdr_t *pMsg)
{
    switch (pMsg->event)
    {
    default:
        break;
    }
}