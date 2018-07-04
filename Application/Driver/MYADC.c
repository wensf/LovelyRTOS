#include <stm32f4xx.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_adc.h>
#include <stm32f4xx_conf.h>

#include "MYADC.h"

GPIO_InitTypeDef GPIO_InitStructure;
ADC_CommonInitTypeDef ADC_CommonInitStructure;
ADC_InitTypeDef ADC_InitStructyre;

void myADC_Init(void)
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
  //PA1 PA2 PA3,模拟输入
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  // GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_PinSource1;//GPIO_PinSource1 | GPIO_PinSource2 | GPIO_PinSource3;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
  //ADC通用配置
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;  //不用DMA
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;  //独立采样
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8;  //速度尽可能低
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);
  
  //ADC1配置
  ADC_InitStructyre.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructyre.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructyre.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructyre.ADC_NbrOfConversion = 1;
  ADC_InitStructyre.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructyre.ADC_ScanConvMode = DISABLE;
  ADC_Init(ADC1,&ADC_InitStructyre);
    
  ADC_Cmd(ADC1,ENABLE);
  ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_144Cycles);
  
  //ADC_SoftwareStartConvCmd(ADC1,ENABLE);
  ADC_SoftwareStartConv(ADC1);
}


int getConvResult(void)
{     
	int adcvalue1;

	while(!(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==SET));
	
	adcvalue1 = ADC_GetConversionValue(ADC1);

	return adcvalue1;
}

