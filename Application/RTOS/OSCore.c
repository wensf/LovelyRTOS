
#include <stdio.h>
#include "stm32f4xx.h"
#include "OSTask.h"
#include "OSCore.h"

extern OS_U32 SystemCoreClock;

void SysTick_Handler(void)
{
	OS_TCBP tcb_p;
	OS_S32 i;
	OS_USE_CRITICAL

	OS_ENTER_CRITICAL();
	++OS_TimeTick;
	for ( i = 0; i < OS_TASK_MAX_NUM; i++ )
	{
		tcb_p = OS_TCB_TABLE[i];
		if ( tcb_p == NULL) continue;
		if ( tcb_p->State == TASK_DELAY ) 
		{
			--tcb_p->TimeDly;
			if ( tcb_p->TimeDly == 0){ 
			  tcb_p->State=TASK_READY;
			}
		}
	}
	OS_EXIT_CRITICAL();
	
    OS_Task_Switch();
    OS_PendSV_Trigger();	
}

OS_STK *g_OS_CPU_ExceptStkBase;

void LRT_OS_Int(void)
{
	g_OS_CPU_ExceptStkBase = OS_CPU_ExceptStk + OS_EXCEPT_STK_SIZE - 1;
	
	__asm("CPSID   I"); 
	for( int i = 0; i < OS_TASK_MAX_NUM; i++ )
	{
		OS_TCB_TABLE[i] = 0;
	}
	
	OS_TimeTick = 0;
	OS_Task_Create(&TCB_IDLE,OS_Task_Idle,&TASK_IDLE_STK[TASK_IDLE_STK_SIZE-1],OS_TASK_MAX_NUM-1);
}

void OS_Start(void)
{
	OS_Task_Switch();
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/OS_TICKS_PER_SECOND);
	OSStart_Asm();
}

void OS_Delay(OS_U32 ticks)
{
    OS_USE_CRITICAL
    
    OS_ENTER_CRITICAL();
    g_OS_Tcb_CurP->State   = TASK_DELAY;
    g_OS_Tcb_CurP->TimeDly = ticks/10;
    OS_EXIT_CRITICAL();
    OS_Task_Switch();
    OS_PendSV_Trigger();
}
