#include <stdio.h>

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#include "OSTask.h"
#include "OSCore.h"

#include "Task_GUI.h"
#include "Task_LED.h"

static OS_TCB TCB_LED;
static OS_TCB TCB_GUI;
static OS_TCB TCB_Demo;

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

void PT_OSTask_Create(void)
{
	
}

int __attribute__((noreturn)) main(void)
{
	OS_Init();
	
	OS_Task_Create(&TCB_Demo,Task_Demo,&TASK_1_STK[TASK_1_STK_SIZE-1],2);
    OS_Task_Create(&TCB_LED,Task_LED, &TASK_2_STK[TASK_2_STK_SIZE-1],5);
	OS_Task_Create(&TCB_GUI,Task_GUI, &TASK_Timer_STK[TASK_Timer_STK_SIZE-1],9);
	
	OS_Start();
	
	for ( ; ; )
	{
		__asm("WFI");
	}
}
