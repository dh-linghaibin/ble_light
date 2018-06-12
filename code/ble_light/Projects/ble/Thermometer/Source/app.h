

#ifndef __APP_H
#define __APP_H

#include "OSAL.h"
#include "ioCC2541.h"

/*********************�궨��************************/
#define GUA_START_DEVICE_EVT 0x0001
#define GUA_LED_TEST_EVT 0x0002
/*******************��Ϻ���************************/
extern void GUA_Init( uint8 task_id );
extern uint16 GUA_ProcessEvent( uint8 task_id, uint16 events );

#endif

