

#include "nrf24l01.h"

const uint8 TX_ADDRESS[TX_ADR_WIDTH] = {0x0A,0x01,0x07,0x0E,0x01};  // ����һ����̬���͵�ַ


/********************************************************
�������ܣ�д�Ĵ�����ֵ�����ֽڣ�                
��ڲ�����reg:�Ĵ���ӳ���ַ����ʽ��WRITE_REG��reg��
					value:�Ĵ�����ֵ
����  ֵ��״̬�Ĵ�����ֵ
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
�������ܣ�д�Ĵ�����ֵ�����ֽڣ�                  
��ڲ�����reg:�Ĵ���ӳ���ַ����ʽ��WRITE_REG��reg��
					pBuf:д�����׵�ַ
					bytes:д�����ֽ���
����  ֵ��״̬�Ĵ�����ֵ
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
�������ܣ���ȡ�Ĵ�����ֵ�����ֽڣ�                  
��ڲ�����reg:�Ĵ���ӳ���ַ����ʽ��READ_REG��reg��
����  ֵ���Ĵ���ֵ
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
�������ܣ���ȡ�Ĵ�����ֵ�����ֽڣ�                  
��ڲ�����reg:�Ĵ���ӳ���ַ��READ_REG��reg��
					pBuf:���ջ��������׵�ַ
					bytes:��ȡ�ֽ���
����  ֵ��״̬�Ĵ�����ֵ
*********************************************************/
uint8 nRF24L01P_Read_Buf(uint8 reg, uint8 *pBuf, uint8 bytes)
{
	uint8 status,byte_ctr;

  CSN = 0;                                        
  status = spftspi_rw(reg);                           
  for(byte_ctr=0;byte_ctr<bytes;byte_ctr++)
    pBuf[byte_ctr] = spftspi_rw(0);                   //��ȡ���ݣ����ֽ���ǰ
  CSN = 1;                                        

  return(status);    
}


/********************************************************
�������ܣ�nRF24L01+����ģʽ��ʼ��                      
��ڲ�������
����  ֵ����
*********************************************************/
void nRF24L01P_RX_Mode(void)
{
	CE = 0;
	nRF24L01P_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // �����豸����ͨ��0ʹ�úͷ����豸��ͬ�ķ��͵�ַ
	nRF24L01P_Write_Reg(WRITE_REG + EN_AA, 0x01);               						// ʹ�ܽ���ͨ��0�Զ�Ӧ��
	nRF24L01P_Write_Reg(WRITE_REG + EN_RXADDR, 0x01);           						// ʹ�ܽ���ͨ��0
	nRF24L01P_Write_Reg(WRITE_REG + RF_CH, 40);                 						// ѡ����Ƶͨ��0x40
	nRF24L01P_Write_Reg(WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);  						// ����ͨ��0ѡ��ͷ���ͨ����ͬ��Ч���ݿ��
	nRF24L01P_Write_Reg(WRITE_REG + RF_SETUP, 0x0f);            						// ���ݴ�����2Mbps�����书��0dBm���������Ŵ�������(nRF24L01+���Ը�λ��
	nRF24L01P_Write_Reg(WRITE_REG + CONFIG, 0x0f);              						// CRCʹ�ܣ�16λCRCУ�飬�ϵ磬����ģʽ
	nRF24L01P_Write_Reg(WRITE_REG + STATUS, 0xff);  												//������е��жϱ�־λ
	CE = 1;                                            											// ����CE���������豸
}						


/********************************************************
�������ܣ�nRF24L01+����ģʽ��ʼ��                      
��ڲ�������
����  ֵ����
*********************************************************/
void nRF24L01P_TX_Mode(void)
{
	CE = 0;
	nRF24L01P_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);     // д�뷢�͵�ַ
	nRF24L01P_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // Ϊ��Ӧ������豸������ͨ��0��ַ�ͷ��͵�ַ��ͬ

	nRF24L01P_Write_Reg(WRITE_REG + EN_AA, 0x01);       										// ʹ�ܽ���ͨ��0�Զ�Ӧ��
	nRF24L01P_Write_Reg(WRITE_REG + EN_RXADDR, 0x01);   										// ʹ�ܽ���ͨ��0
	nRF24L01P_Write_Reg(WRITE_REG + SETUP_RETR, 0x0a);  										// �Զ��ط���ʱ�ȴ�250us+86us���Զ��ط�10��
	nRF24L01P_Write_Reg(WRITE_REG + RF_CH, 40);         										// ѡ����Ƶͨ��0x40
	nRF24L01P_Write_Reg(WRITE_REG + RF_SETUP, 0x0f);    										// ���ݴ�����2Mbps�����书��0dBm���������Ŵ�������(nRF24L01+���Ը�λ��
	nRF24L01P_Write_Reg(WRITE_REG + CONFIG, 0x0e);      										// CRCʹ�ܣ�16λCRCУ�飬�ϵ�
	//CE = 1;
}


/********************************************************
�������ܣ���ȡ��������                       
��ڲ�����rxbuf:�������ݴ���׵�ַ
����  ֵ��0:���յ�����
          1:û�н��յ�����
*********************************************************/
uint8 nRF24L01P_RxPacket(uint8 *rxbuf)
{
	uint8 state;
	state = nRF24L01P_Read_Reg(STATUS);  			                 //��ȡ״̬�Ĵ�����ֵ    	  
	nRF24L01P_Write_Reg(WRITE_REG+STATUS,state);               //���RX_DS�жϱ�־

	if(state & RX_DR)								                           //���յ�����
	{
		nRF24L01P_Read_Buf(RD_RX_PLOAD,rxbuf,TX_PLOAD_WIDTH);     //��ȡ����
		nRF24L01P_Write_Reg(FLUSH_RX,0xff);					              //���RX FIFO�Ĵ���
		return 0; 
	}	   
	return 1;                                                   //û�յ��κ�����
}


/********************************************************
�������ܣ�����һ�����ݰ�                      
��ڲ�����txbuf:Ҫ���͵�����
����  ֵ��0x10:�ﵽ����ط�����������ʧ�� 
          0x20:���ͳɹ�            
          0xff:����ʧ��                  
*********************************************************/
uint8 nRF24L01P_TxPacket(uint8 *txbuf)
{
	uint8 state;
	CE=0;																										//CE���ͣ�ʹ��24L01����
  nRF24L01P_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);	//д���ݵ�TX FIFO,32���ֽ�
 	CE=1;																										//CE�øߣ�ʹ�ܷ���	   
	
	while(IRQ == 1);																				//�ȴ��������
	state=nRF24L01P_Read_Reg(STATUS);  											//��ȡ״̬�Ĵ�����ֵ	   
	nRF24L01P_Write_Reg(WRITE_REG+STATUS,state); 						//���TX_DS��MAX_RT�жϱ�־
	if(state&MAX_RT)																			  //�ﵽ����ط�����
	{
		nRF24L01P_Write_Reg(FLUSH_TX,0xff);										//���TX FIFO�Ĵ��� 
		return MAX_RT; 
	}
	if(state&TX_DS)																			    //�������
	{
		return TX_DS;
	}
	return 0XFF;																						//����ʧ��
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
