#ifndef CC2541_SOFTI2C_H_
#define CC2541_SOFTI2C_H_

#include <iocc2541.h>
#include "common.h"
#include "hal_types.h"

#define SCL                             P1_0                //定义模拟IIC的时钟线
#define SDA                             P1_2                //定义模拟IIC的数据线

#define SDA_0                           (P1_2=0) //SDA=0
#define SDA_1                           (P1_2=1)  //SDA=1
#define SCL_0                           (P1_0=0) //SCL=0
#define SCL_1                           (P1_0=1)  //SCL=1

#define SDA_IN                          P1DIR &= ~BIT2 //SDA INPUT
#define SDA_OUT                         P1DIR |= BIT2  //SDA OUTPUT
#define SCL_IN                          P1DIR &= ~BIT0 //SCL INPUT
#define SCL_OUT                         P1DIR |= BIT0  //SCL OUTPUT

#define I2C_SDA_READ			(P1 & BIT2)

void DelayMS(uint16 msec);
void delay_1ms(void);
void IIC_Init(void);
void stop(void);
void start(void);
void send_noack(void);
void send_ack(void);
void iic_write(uint8 datIn);
uint8 check_ack(void);
uint8 iic_read(void);




#endif
