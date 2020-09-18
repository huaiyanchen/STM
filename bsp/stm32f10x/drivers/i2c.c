#include "i2c.h"


void I2C_GPIOInitConfig(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_I2C_PORT,ENABLE); 
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE); //Disable jtag

    GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN | I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIO_PORT_I2C, &GPIO_InitStructure);

    GPIO_ResetBits(GPIO_PORT_I2C, I2C_SCL_PIN | I2C_SDA_PIN);
}

void SDA_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIO_PORT_I2C, &GPIO_InitStructure);

    GPIO_SetBits(GPIO_PORT_I2C,I2C_SDA_PIN);
}

void SDA_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIO_PORT_I2C, &GPIO_InitStructure);

    GPIO_SetBits(GPIO_PORT_I2C,I2C_SDA_PIN);
}
/**************************************
**************************************/
void I2C_Start(void)
{
    SDA_OUT();
    I2C_SDA_0();
    
    I2C_Delay();
    I2C_SCL_1();
    I2C_Delay();
    I2C_SDA_1();
    
    I2C_Delay();     
    I2C_SDA_0(); 
    I2C_Delay();   
    I2C_SCL_0();  
}

/**************************************
**************************************/
void I2C_Stop(void)
{
    SDA_OUT();
    I2C_SDA_0();
    I2C_Delay();
    
    I2C_SCL_1();
    I2C_Delay();
    I2C_SDA_1();
    I2C_Delay();
}

/**************************************
sendACK and stop
**************************************/
void I2C_SendACK(u8 ack)
{
    SDA_OUT();
    I2C_SCL_0(); //��д������ʱ���� scl = 0
//		delay_us(8);
    if(ack)
        I2C_SDA_1();
    else    
        I2C_SDA_0();
    
    I2C_SCL_1(); 
    I2C_Delay();
    I2C_SCL_0();
    I2C_Delay();
}

/**************************************
**************************************/
u8 I2C_RecvACK(void)
{
    u8 ucErrTime=0;

    SDA_IN();
//		I2C_Delay(); //��д������ʱ���� scl = 0	   
    I2C_SCL_1();
    I2C_Delay();
     
    while(I2C_SDA_READ())
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            I2C_Stop();
            return 1;
        }
    }
    I2C_SCL_0();
    I2C_Delay();
    return 0;  
}

/**************************************
**************************************/
void I2C_SendByte(u8 dat)
{
	u8 t,temp; 
	SDA_OUT(); 	    
    I2C_SCL_0();
    for(t=0;t<8;t++)
    {              
        temp=(dat&0x80); // do it and delay
		dat <<= 1;
		if(temp)
			 I2C_SDA_1();
		else I2C_SDA_0(); 	  
		I2C_Delay();
		I2C_SCL_1();
		I2C_Delay(); 
		I2C_SCL_0();
//		I2C_Delay();
    }	
    I2C_Delay();
}

/**************************************
**************************************/	

u8 I2C_RecvByte(void)
{
	int i = 0;
	u8 byte = 0;
	SDA_IN();
	for(i = 0;i < 8;i++)
	{
		I2C_Delay();
		I2C_SCL_1();
		I2C_Delay();
		byte <<= 1;
		if(I2C_SDA_READ())
		{
			byte |= 0x01;
		}
		I2C_SCL_0();
	}
	I2C_Delay();
	return byte;
}


/****************************************************************************
*	�� �� ��: i2c_CheckDevice
*	����˵��: ���I2C�����豸��CPU�����豸��ַ��Ȼ���ȡ�豸Ӧ�����жϸ��豸�Ƿ����
*	��    �Σ�_Address���豸��I2C���ߵ�ַ
*	�� �� ֵ: ����ֵ 0 ��ʾ��ȷ�� ����1��ʾδ̽�⵽
****************************************************************************/
u8 I2C_CheckDevice(uint8_t _Address)
{
    uint8_t ucAck;

    I2C_Start();		/* ���������ź� */

    /* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
    I2C_SendByte(_Address | I2C_WR);
    ucAck = I2C_RecvACK();	/* ����豸��ACKӦ�� */

    I2C_Stop();			    /* ����ֹͣ�ź� */

    return ucAck;
}



