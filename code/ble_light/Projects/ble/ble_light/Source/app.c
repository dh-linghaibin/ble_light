
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
        
        
        //四个按钮
        if( (val&0x4000) == 0x4000)
        {
            
        }
        else
        {
            softled_mode_set(1);
        }
        
        if( (val&0x8000) == 0x8000)
        {
            
        }
        else
        {
            softled_mode_set(2);
        }
        
        if( (val&0x1) == 0x1)
        {
            
        }
        else
        {
            softled_mode_set(3);
        }
        
        if( (val&0x2) == 0x2)
        {
            
        }
        else
        {
            softled_mode_set(4);
        }
        
        
        if( (val&0x10) == 0x10)
        {
            
        }
        else
        {
            static uint8 dr = 0;
            dr = dr==0 ? 1:0;
            softled_enable_set(dr);
        }
        
        if(softled_get_key()) {
            static uint8 dr = 0;
            dr = dr==0 ? 1:0;
            softled_kill_set(dr);
        } else {
            
        }
     
//        uint8 buf[32] = {0};
//        if(!nRF24L01P_RxPacket(buf))
//		{
//            static uint8 dr = 0;
//            dr = dr==0 ? 1:0;
//            P0_5 = dr;
//			switch(buf[0])
//			{
//				case 0x55:
//
//					break;
//				case 0xAA:
//
//					break;
//				default:
//					break;
//			}
//			buf[0] = 0;	
//		}
//        
        static uint16 count = 0; 
        static uint8 ld = 0; 
        if(count < 10) {
            count++;
            P0_4 = 1;
            
            I2CIO = 0x03;
            
        } else {
            ld++;
            count = 0;
            P0_4 = 0;
            
            I2CIO = 0x00;
            
            uint16 nGUA_ConnHandle;
            //获得连接句柄
            GAPRole_GetParameter(GAPROLE_CONNHANDLE, &nGUA_ConnHandle);
            uint8 send[2] = {0x55,0x51};
            GUA_SimpleGATTprofile_Char4_Notify(nGUA_ConnHandle,send,2);
            
//            uint8 buf[32] = {0};
//            buf[0] = 0x3a;
//            buf[1] = 0x01;
//            buf[2] = 0x00;
//            buf[3] = 0x01;
//            buf[4] = ld;
//            buf[5] = 0x00;
//            buf[6] = buf[1] + buf[2] + buf[3] + buf[4] + buf[5];
//            buf[7] = 0x0a;
//            nRF24L01P_TX_Mode();
//            nRF24L01P_TxPacket(buf);
//			buf[0] = 0;
//			nRF24L01P_RX_Mode();
        }
        
        uint8 tempbuf[10]={0};  
        UTCTimeStruct *Ti;  
        osalTimeUpdate();    
        osal_ConvertUTCTime(Ti,osal_getClock());  
        
        
        int v = 0;
        v = HalAdcRead ( HAL_ADC_CHANNEL_7, HAL_ADC_RESOLUTION_14 );
        if(v == 100) {
            tempbuf[0]=(Ti->hour)/10;  
            tempbuf[1]=(Ti->hour)%10;  
            tempbuf[2]=(Ti->minutes)/10;  
            tempbuf[3]=(Ti->minutes)%10+'0';  
            tempbuf[4]=(Ti->seconds)/10+'0';  
            tempbuf[5]=(Ti->seconds)%10+'0';  
        }
        
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