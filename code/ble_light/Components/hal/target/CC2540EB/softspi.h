

#ifndef __SOFTSPI_H
#define __SOFTSPI_H

#include <iocc2541.h>
#include "common.h"
#include "hal_types.h"

#define SCK                             P1_5
#define CSN                             P1_4
#define CE                              P1_1
#define IRQ                             P2_0
#define MOSI                            P1_6
#define MISO                            P1_7

//#define SCK_0                           (P0_7=0) 
//#define SCK_1                           (P0_7=1) 
//#define CSN_0                           (P0_7=0)
//#define CSN_1                           (P0_7=1)
//#define CE_0                            (P0_7=0)
//#define CE_1                            (P0_7=1)
//#define IRQ_0                           (P0_7=0)
//#define IRQ_1                           (P0_7=1)

#define MISO_IN                          P1DIR &= ~BIT7 //SDA INPUT
#define MISO_OUT                         P1DIR |= BIT7  //SDA OUTPUT

#define SPI_MISO_READ			(P1 & BIT7)

void spftspi_init(void);
uint8 spftspi_rw(uint8 byte);

#endif
