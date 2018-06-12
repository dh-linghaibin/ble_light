
#include "softspi.h"

void spftspi_init(void)
{
    P1DIR |= BIT1;
    P1DIR |= BIT4;
    P1DIR |= BIT5;
    P1DIR |= BIT6;
    
    P1DIR &= ~BIT7;
    P2DIR &= ~BIT0;
        
    SCK = 0;//SPI ±÷”œﬂ¿≠µÕ
	CSN = 1;				
	CE 	= 0;				
	IRQ = 1;
}

uint8 spftspi_rw(uint8 byte)
{
	uint8 bit_ctr;
	for(bit_ctr=0; bit_ctr<8; bit_ctr++)
	{
		if(byte & 0x80)
			MOSI = 1;
		else
			MOSI = 0;																 
		byte = (byte << 1);                      
		SCK = 1;                                   
		byte |= SPI_MISO_READ;                             
		SCK = 0;                                
	}
	return(byte);                              
}
