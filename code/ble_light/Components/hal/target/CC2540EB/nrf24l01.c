

#include "nrf24l01.h"

const uint8 TX_ADDRESS[TX_ADR_WIDTH] = {0x0A,0x01,0x07,0x0E,0x01};  // 定义一个静态发送地址


/********************************************************
函数功能：写寄存器的值（单字节）                
入口参数：reg:寄存器映射地址（格式：WRITE_REG｜reg）
					value:寄存器的值
返回  值：状态寄存器的值
*********************************************************/
uint8 nRF24L01P_Write_Reg(uint8 reg, uint8 value)
{
	uint8 status;

	CSN = 0;                 
    status = spftspi_rw(reg);				
	spftspi_rw(value);
	CSN = 1;  
	
	return(status);
}


/********************************************************
函数功能：写寄存器的值（多字节）                  
入口参数：reg:寄存器映射地址（格式：WRITE_REG｜reg）
					pBuf:写数据首地址
					bytes:写数据字节数
返回  值：状态寄存器的值
*********************************************************/
uint8 nRF24L01P_Write_Buf(uint8 reg, const uint8 *pBuf, uint8 bytes)
{
    uint8 status,byte_ctr;

  CSN = 0;                                  			
  status = spftspi_rw(reg);                          
  for(byte_ctr=0; byte_ctr<bytes; byte_ctr++)     
    spftspi_rw(*pBuf++);
  CSN = 1;                                      	

  return(status);       
}							  					   


/********************************************************
函数功能：读取寄存器的值（单字节）                  
入口参数：reg:寄存器映射地址（格式：READ_REG｜reg）
返回  值：寄存器值
*********************************************************/
uint8 nRF24L01P_Read_Reg(uint8 reg)
{
 	uint8 value;

	CSN = 0;    
  spftspi_rw(reg);			
	value = spftspi_rw(0);
	CSN = 1;              

	return(value);
}


/********************************************************
函数功能：读取寄存器的值（多字节）                  
入口参数：reg:寄存器映射地址（READ_REG｜reg）
					pBuf:接收缓冲区的首地址
					bytes:读取字节数
返回  值：状态寄存器的值
*********************************************************/
uint8 nRF24L01P_Read_Buf(uint8 reg, uint8 *pBuf, uint8 bytes)
{
	uint8 status,byte_ctr;

  CSN = 0;                                        
  status = spftspi_rw(reg);                           
  for(byte_ctr=0;byte_ctr<bytes;byte_ctr++)
    pBuf[byte_ctr] = spftspi_rw(0);                   //读取数据，低字节在前
  CSN = 1;                                        

  return(status);    
}


/********************************************************
函数功能：nRF24L01+接收模式初始化                      
入口参数：无
返回  值：无
*********************************************************/
void nRF24L01P_RX_Mode(void)
{
	CE = 0;
	nRF24L01P_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // 接收设备接收通道0使用和发送设备相同的发送地址
	nRF24L01P_Write_Reg(WRITE_REG + EN_AA, 0x01);               						// 使能接收通道0自动应答
	nRF24L01P_Write_Reg(WRITE_REG + EN_RXADDR, 0x01);           						// 使能接收通道0
	nRF24L01P_Write_Reg(WRITE_REG + RF_CH, 40);                 						// 选择射频通道0x40
	nRF24L01P_Write_Reg(WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);  						// 接收通道0选择和发送通道相同有效数据宽度
	nRF24L01P_Write_Reg(WRITE_REG + RF_SETUP, 0x0f);            						// 数据传输率2Mbps，发射功率0dBm，低噪声放大器增益(nRF24L01+忽略该位）
	nRF24L01P_Write_Reg(WRITE_REG + CONFIG, 0x0f);              						// CRC使能，16位CRC校验，上电，接收模式
	nRF24L01P_Write_Reg(WRITE_REG + STATUS, 0xff);  												//清除所有的中断标志位
	CE = 1;                                            											// 拉高CE启动接收设备
}						


/********************************************************
函数功能：nRF24L01+发送模式初始化                      
入口参数：无
返回  值：无
*********************************************************/
void nRF24L01P_TX_Mode(void)
{
	CE = 0;
	nRF24L01P_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);     // 写入发送地址
	nRF24L01P_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // 为了应答接收设备，接收通道0地址和发送地址相同

	nRF24L01P_Write_Reg(WRITE_REG + EN_AA, 0x01);       										// 使能接收通道0自动应答
	nRF24L01P_Write_Reg(WRITE_REG + EN_RXADDR, 0x01);   										// 使能接收通道0
	nRF24L01P_Write_Reg(WRITE_REG + SETUP_RETR, 0x0a);  										// 自动重发延时等待250us+86us，自动重发10次
	nRF24L01P_Write_Reg(WRITE_REG + RF_CH, 40);         										// 选择射频通道0x40
	nRF24L01P_Write_Reg(WRITE_REG + RF_SETUP, 0x0f);    										// 数据传输率2Mbps，发射功率0dBm，低噪声放大器增益(nRF24L01+忽略该位）
	nRF24L01P_Write_Reg(WRITE_REG + CONFIG, 0x0e);      										// CRC使能，16位CRC校验，上电
	//CE = 1;
}


/********************************************************
函数功能：读取接收数据                       
入口参数：rxbuf:接收数据存放首地址
返回  值：0:接收到数据
          1:没有接收到数据
*********************************************************/
uint8 nRF24L01P_RxPacket(uint8 *rxbuf)
{
	uint8 state;
	state = nRF24L01P_Read_Reg(STATUS);  			                 //读取状态寄存器的值    	  
	nRF24L01P_Write_Reg(WRITE_REG+STATUS,state);               //清除RX_DS中断标志

	if(state & RX_DR)								                           //接收到数据
	{
		nRF24L01P_Read_Buf(RD_RX_PLOAD,rxbuf,TX_PLOAD_WIDTH);     //读取数据
		nRF24L01P_Write_Reg(FLUSH_RX,0xff);					              //清除RX FIFO寄存器
		return 0; 
	}	   
	return 1;                                                   //没收到任何数据
}


/********************************************************
函数功能：发送一个数据包                      
入口参数：txbuf:要发送的数据
返回  值：0x10:达到最大重发次数，发送失败 
          0x20:发送成功            
          0xff:发送失败                  
*********************************************************/
uint8 nRF24L01P_TxPacket(uint8 *txbuf)
{
	uint8 state;
	CE=0;																										//CE拉低，使能24L01配置
  nRF24L01P_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);	//写数据到TX FIFO,32个字节
 	CE=1;																										//CE置高，使能发送	   
	
	while(IRQ == 1);																				//等待发送完成
	state=nRF24L01P_Read_Reg(STATUS);  											//读取状态寄存器的值	   
	nRF24L01P_Write_Reg(WRITE_REG+STATUS,state); 						//清除TX_DS或MAX_RT中断标志
	if(state&MAX_RT)																			  //达到最大重发次数
	{
		nRF24L01P_Write_Reg(FLUSH_TX,0xff);										//清除TX FIFO寄存器 
		return MAX_RT; 
	}
	if(state&TX_DS)																			    //发送完成
	{
		return TX_DS;
	}
	return 0XFF;																						//发送失败
}


void nRF24L01P_Init(void) {
    spftspi_init();
}



//void main(void)
//{
//	KEY1 = 1;
//	KEY2 = 1;
//	nRF24L01P_Init();
//	nRF24L01P_RX_Mode();
//	while(1)
//	{
//		u8 buf[32] = {0};
//		KEY1 = 1;
//		KEY2 = 1;
//		if(!KEY1 || !KEY2)
//		{
//			delay_ms(10);
//			if(!KEY1)
//			{
//				buf[0] = 0x55;
//				nRF24L01P_TX_Mode();
//				nRF24L01P_TxPacket(buf);
//				delay_ms(200);
//			}
//			if(!KEY2)
//			{
//				buf[0] = 0xAA;
//				nRF24L01P_TX_Mode();
//				nRF24L01P_TxPacket(buf);
//				delay_ms(200);
//			}
//			buf[0] = 0;
//			nRF24L01P_RX_Mode();
//		}	
//		
//		if(!nRF24L01P_RxPacket(buf))
//		{
//			switch(buf[0])
//			{
//				case 0x55:
//					LED3 = 0;
//					delay_ms(100);
//					LED3 = 1;
//					delay_ms(100);
//					break;
//				case 0xAA:
//					LED4 = 0;
//					delay_ms(100);
//					LED4 = 1;
//					delay_ms(100);
//					break;
//				default:
//					break;
//			}
//			buf[0] = 0;	
//		}
//		
//	}
//}
