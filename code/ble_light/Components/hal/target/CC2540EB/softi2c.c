
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <softi2c.h>


void delay_1ms(void)    //��� 0us  ��ʱ1ms
{
    uint8 a,c;    
    for(c=4;c>0;c--)
    {
        //for(b=142;b>0;b--)
        {
            for(a=2;a>0;a--)
            {
            }
        }
    }
}

/****************************************************************************
* ��    ��: DelayMS()
* ��    ��: �Ժ���Ϊ��λ��ʱ��ϵͳʱ�Ӳ�����ʱĬ��Ϊ16M(��ʾ���������൱��ȷ)
* ��ڲ���: msec ��ʱ������ֵԽ����ʱԽ��
* ���ڲ���: ��
****************************************************************************/
void DelayMS(uint16 msec)
{ 
    uint16 i,j;
    
    for (i=0; i<msec; i++)
        for (j=0; j<535; j++);
}

void IIC_Init(void)//IIC��ʼ��
{
    P1DIR |= 0x01;      //P1.5��P1.6����Ϊ���
    P0DIR |= 0x80;

    SDA_0;
    SCL_0;
    delay_1ms();
    SCL_1;
    SDA_1;
    delay_1ms();
    
}

//***********************************************************************************
//*���ƣ�i2c_send_noack()              												*
//*���ܣ���Ӧ��I2C����                  											*
//*��������                             											*
//*���أ�1 ��NOACK�ź� 0 ��NOACK�ź�                             					*
//***********************************************************************************
void send_noack(void){
	SDA_OUT;	//��·���,
	SDA_1;  //SDA = 1; NO ACK
	delay_1ms();
	SCL_1;  //SCL = 1;
	delay_1ms();
	SCL_0;  //SCL = 0;	//START
}
// iic Ӧ��  for slaver
void send_ack(void) {
	SDA_OUT;	//��·���,
	SDA_0; //OUT 0 ACK
	delay_1ms();
	SCL_1;
	delay_1ms();
	SCL_0;
}

/*
 *ֹͣiic
 */
void stop(void){
	SDA_OUT;	//��·���,�����0.
	SCL_0;  //SCL = 0;
	delay_1ms();
	SCL_1;  //SCL = 1;	STOP
	delay_1ms();
	SDA_1;  //SDA = 1;
        SDA_IN;
        SCL_IN;
}

/*
 * ����iic
 */
//	static void start(void) {
void start(void){
	SDA_OUT;	//��·���,�����0.
        SCL_OUT;
	SDA_1;  //SDA = 1;
	SCL_1;  //SCL = 1;
	delay_1ms();
	SDA_0;  //SDA = 0;
	delay_1ms();
	SCL_0;  //SCL = 0;	//START
}

/*
 * iicдһ���ֽ�
 */
void iic_write(uint8 datIn) {
	uint8 dat, j;
	dat = datIn;
        SDA_OUT;
        SCL_0;
	for (j = 0; j < 8; j++) {
            if((dat & 0x80)) SDA_1;
            else SDA_0;
            delay_1ms();
            delay_1ms();
            SCL_1;	//write TDOS_SDA begin
            delay_1ms();
            dat <<= 1;
            SCL_0;	//write TDOS_SDA end
            delay_1ms();
	}
}

uint8 check_ack(void) {
	uint8 ack_flag;

	SDA_IN;	//��·����,
	delay_1ms();
	SCL_1;	//read ask begin
	delay_1ms();
	
    if(I2C_SDA_READ == BIT7){ //if (SDA==1)
		ack_flag = 0; //1: err
	}else{
		ack_flag = 1; //0: ok
	}
	SCL_0;	//read ask end
	return ack_flag;
}

/*
 * iic��һ���ֽ�
 */
uint8 iic_read(void) {
	uint8 j, dat = 0;
	SDA_IN;	//��·����,
	for (j = 0; j < 8; j++) {
		SCL_1;	//read TDOS_SDA begin. delay 0.7us
		dat <<= 1;
                delay_1ms();
                delay_1ms();
                if(I2C_SDA_READ == BIT7){ //if (SDA==1)
			dat |= 0x01; //input TDOS_SDA
		}
		SCL_0;	//read TDOS_SDA end. delay 1.4us
	}
	return dat;
}




//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------

