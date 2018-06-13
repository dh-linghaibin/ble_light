

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

#define MISO_IN                          P1DIR &= ~BIT7 //SDA INPUT
#define MISO_OUT                         P1DIR |= BIT7  //SDA OUTPUT

#define SPI_MISO_READ			(P1 & BIT7)
#define SPI_IRQ_READ			(P2 & BIT0)

void Delay_Us(uint16 nUs);
void spftspi_init(void);
uint8 spftspi_rw(uint8 byte);

#endif
