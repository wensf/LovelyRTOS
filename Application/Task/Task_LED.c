
#include "stm32f4xx.h"

#include "OSCore.h"
#include "OSTask.h"
#include "Task_LED.h"
#include "Draw.h"

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

unsigned long Task_LED_tick;

#include "Task_GUI.h"
#include <string.h>

void Task_LED(void)
{
	// LED_GPIO_Init();

	while(1)
	{
		Task_LED_tick++;
	//	LED1_ON();

		OS_Delay(500);
	//	LED1_OFF();
		OS_Delay(500);
	}	
	
}
