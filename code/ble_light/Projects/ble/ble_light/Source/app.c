
#include "app.h"

#include "OSAL.h"
#include "ioCC2541.h"
#include "nrf24l01.h"
#include "softled.h"
#include "hal_temp112.h"
#include "OSAL_Clock.h"

#define VOID (void)
#define GUA_LED_TEST_EVT_PERIOD 100
static uint8 GUA_TaskID; // Task ID for internal task/event processing
static void GUA_ProcessOSALMsg(osal_event_hdr_t *pMsg);

void GUA_Init(uint8 task_id)
{
    GUA_TaskID = task_id;
    nRF24L01P_Init();
    nRF24L01P_RX_Mode();
    
    //Initiallize temp112 chip
    softled_init();
    xw09a_init();
    
    osal_set_event(GUA_TaskID, GUA_START_DEVICE_EVT);
}

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
        uint16 val = xw09a_get();
        if( (val&0x8000) == 0x8000)
        {
            P0_1 = 1;
        }
        else
        {
            P0_1 = 0;
        }
        
        if( (val&0x4000) == 0x4000)
        {
            P0_2 = 1;
        }
        else
        {
            P0_2 = 0;
        }
        
        if( (val&0x2000) == 0x2000)
        {
            P0_3 = 1;
        }
        else
        {
            P0_3 = 0;
        }
        
        if( (val&0x1000) == 0x1000)
        {
            //P0_4 = 1;
        }
        else
        {
            //P0_4 = 0;
        }
        
//        if( (val&0x0002) == 0x0002)
//        {
//            P0_4 = 1;
//        }
//        else
//        {
//            P0_4 = 0;
//            uint8 buf[32] = {0};
//            buf[0] = 0x55;
//            nRF24L01P_TX_Mode();
//            nRF24L01P_TxPacket(buf);
//			buf[0] = 0;
//			nRF24L01P_RX_Mode();
//        }
        
//        {
//            uint8 buf[32] = {0};
//            buf[0] = 0x55;
//            nRF24L01P_TX_Mode();
//            nRF24L01P_TxPacket(buf);
//			buf[0] = 0;
//			nRF24L01P_RX_Mode();
//        }
        
        
        uint8 buf[32] = {0};
        if(!nRF24L01P_RxPacket(buf))
		{
            static uint8 dr = 0;
            dr = dr==0 ? 1:0;
            P0_5 = dr;
			switch(buf[0])
			{
				case 0x55:

					break;
				case 0xAA:

					break;
				default:
					break;
			}
			buf[0] = 0;	
		}
        
        static uint16 count = 0; 
        if(count < 10) {
            count++;
            P0_4 = 1;
        } else {
            count = 0;
            P0_4 = 0;
            uint8 buf[32] = {0};
            buf[0] = 0x55;
            nRF24L01P_TX_Mode();
            nRF24L01P_TxPacket(buf);
			buf[0] = 0;
			nRF24L01P_RX_Mode();
        }
        
        uint8 tempbuf[10]={0};  
        UTCTimeStruct *Ti;  

        osalTimeUpdate();    
        osal_ConvertUTCTime(Ti,osal_getClock());  
        tempbuf[0]=(Ti->hour)/10;  
        tempbuf[1]=(Ti->hour)%10;  
        tempbuf[2]=(Ti->minutes)/10;  
        tempbuf[3]=(Ti->minutes)%10+'0';  
        tempbuf[4]=(Ti->seconds)/10+'0';  
        tempbuf[5]=(Ti->seconds)%10+'0';  
        
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