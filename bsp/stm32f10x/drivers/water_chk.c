/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 */
#include <stm32f10x.h>
#include <water_chk.h>
#include "app_main.h"

// adc define
#ifdef ADC_1
#define water_chk_rcc                    RCC_APB2Periph_GPIOA
#define water_chk_gpio                   GPIOA
#define water_chk_pin                    GPIO_Pin_6
#endif

#define ADC1_DR_Address    ((u32)0x4001244C)
// __IO u16 WATER_ADC_ConvertedValue;  

void rt_hw_adc_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
#ifdef ADC_1
	//gpio config
    RCC_APB2PeriphClockCmd(water_chk_rcc,ENABLE);
    GPIO_InitStructure.GPIO_Pin = water_chk_pin;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN; //GPIO����Ϊģ������
    GPIO_Init(water_chk_gpio, &GPIO_InitStructure);
	
	//mode config
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //ʹ��MDA1ʱ��
	/* DMA channel1 configuration */
	DMA_DeInit(DMA1_Channel1);  //ָ��DMAͨ��
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;//����DMA�����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&_td_main.dev.stt.WATER_ADC_ConvertedValue;// WATER_ADC_ConvertedValue;	//����DMA�ڴ��ַ��ADCת�����ֱ�ӷ���õ�ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //����Ϊ����Ϊ���ݴ������Դ
	DMA_InitStructure.DMA_BufferSize = 1;	//DMA����������Ϊ1��
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	/* Enable DMA channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);  //ʹ��DMAͨ��

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	//ʹ��ADC1ʱ��
	 
	/* ADC1 configuration */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ʹ�ö���ģʽ��ɨ��ģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //������Ӵ�����
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //ʹ�������Ҷ���
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;  // ֻ��1��ת��ͨ��
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ADC1 regular channel11 configuration */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_55Cycles5); //ͨ��1��������55.5��ʱ������

	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);	 //ʹ��ADC��DMA

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE); //ʹ��ADC1

	/* Enable ADC1 reset calibaration register */   
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));

	/* Start ADC1 calibaration */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));
	 
	/* Start ADC1 Software Conversion */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);  //��ʼת��
#endif
}


