#include <stdio.h>

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#include "OSTask.h"
#include "OSCore.h"
#include "LRT_Object.h"
#include "LRT_Driver.h"
#include "LRT_Input.h"
#include "LRT_Fb.h"

#include "Task_GUI.h"
#include "Task_LED.h"

OS_TCB TCB_LED;
OS_TCB TCB_GUI;
OS_TCB TCB_Demo;

OS_STK TASK_1_STK[TASK_1_STK_SIZE];
OS_STK TASK_2_STK[TASK_2_STK_SIZE];
OS_STK TASK_3_STK[TASK_3_STK_SIZE];

unsigned long task_demo_tick;

void Task_Demo(void)
{	
	while(1)
	{
		OS_Delay(100);
		task_demo_tick++;
	}
}	

#include "BSP_UART.h"

void BSP_Init(void)
{
	Usart_Config();
}

int __attribute__((noreturn)) main(void)
{
	BSP_Init();
	
	/**
	 * LovelyRTOS Kernel Component Initialize
	 */
	LRT_OS_Int();
	LRT_Object_Init();
	LRT_Input_Init();
	LRT_Fb_Init();
	LRT_Driver_Init();
	
	/**
	 * User Layer Task ( Thread ) Create
	 */
	LRT_OSTask_Create(
					&TCB_Demo,Task_Demo,
					&TASK_1_STK[TASK_1_STK_SIZE-1],
					0);
	
    LRT_OSTask_Create(
					&TCB_LED,Task_LED, 
					&TASK_2_STK[TASK_2_STK_SIZE-1],
					1);
	
	LRT_OSTask_Create(
					&TCB_GUI,Task_GUI, 
					&TASK_Timer_STK[TASK_Timer_STK_SIZE-1],
					2);
	
	/**
	 * LovelyRTOS Start To Run
	 */
	
	LRT_OS_Start();
	
	/**
	 * Should be never reach here
	 */
	for ( ; ; )
	{
		__asm("WFI");
	}
}

