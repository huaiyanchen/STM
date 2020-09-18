#ifndef _I2C_H_
#define _I2C_H_

#include <stm32f10x.h>
extern void Delay500ns(void);

#define I2C_WR	       0		  /* д����bit  */
#define I2C_RD	       1		  /* ������bit  */
#define I2C_ADDRESS    0xA0   /* ���豸��ַ */
#define ACK            0
#define NO_ACK         1

/********************* delay time configuration ************************/
#define I2C_Delay()    Delay500ns()

/********************* hardware configuration **************************/
#define GPIO_PORT_I2C	GPIOB			              /* GPIO�˿� */
#define RCC_I2C_PORT 	RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define I2C_SCL_PIN		GPIO_Pin_3			        /* ���ӵ�SCLʱ���ߵ�GPIO */
#define I2C_SDA_PIN		GPIO_Pin_5		        /* ���ӵ�SDA�����ߵ�GPIO */
/*******************end hardware configuration**************************/

#if 0	/* �������룺 1 ѡ��GPIO�Ŀ⺯��ʵ��IO��д */
	#define I2C_SCL_1()  GPIO_SetBits(GPIO_PORT_I2C, I2C_SCL_PIN)		  /* SCL = 1 */
	#define I2C_SCL_0()  GPIO_ResetBits(GPIO_PORT_I2C, I2C_SCL_PIN)		/* SCL = 0 */
	
	#define I2C_SDA_1()  GPIO_SetBits(GPIO_PORT_I2C, I2C_SDA_PIN)		  /* SDA = 1 */
	#define I2C_SDA_0()  GPIO_ResetBits(GPIO_PORT_I2C, I2C_SDA_PIN)		/* SDA = 0 */
	
	#define I2C_SDA_READ()  GPIO_ReadInputDataBit(GPIO_PORT_I2C, I2C_SDA_PIN)	/* ��SDA����״̬ */
#else	/* �����֧ѡ��ֱ�ӼĴ�������ʵ��IO��д */
    /*��ע�⣺����д������IAR��߼����Ż�ʱ���ᱻ�����������Ż� */
	#define I2C_SCL_1()  GPIO_PORT_I2C->BSRR = I2C_SCL_PIN				    /* SCL = 1 */
	#define I2C_SCL_0()  GPIO_PORT_I2C->BRR = I2C_SCL_PIN				      /* SCL = 0 */
	
	#define I2C_SDA_1()  GPIO_PORT_I2C->BSRR = I2C_SDA_PIN				    /* SDA = 1 */
	#define I2C_SDA_0()  GPIO_PORT_I2C->BRR = I2C_SDA_PIN				      /* SDA = 0 */
	
	#define I2C_SDA_READ()    ((GPIO_PORT_I2C->IDR & I2C_SDA_PIN) != 0)	/* ��SDA����״̬ */
#endif

void I2C_GPIOInitConfig(void);
void SDA_OUT(void);
void SDA_IN(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_SendACK(u8 ack);
u8 I2C_RecvACK(void);
void I2C_SendByte(u8 dat);
u8 I2C_RecvByte(void);
u8 I2C_CheckDevice(uint8_t _Address);

#endif
