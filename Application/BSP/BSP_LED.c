
#include "BSP_LED.h" 

#include "stm32f4xx.h"

void LED_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_AHB1PeriphClockCmd ( LED1_CLK, ENABLE); 			
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;	 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_InitStructure.GPIO_Pin = LED1_PIN;	 
	GPIO_Init(LED1_PORT, &GPIO_InitStructure);		
	
	GPIO_ResetBits(LED1_PORT,LED1_PIN);  
}

void LED_set( int enable )
{
	if ( enable == 1 )
	{
		GPIO_SetBits(LED1_PORT,LED1_PIN); 
	}else{
		GPIO_ResetBits(LED1_PORT,LED1_PIN); 
	}
}

