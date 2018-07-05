#include "OSType.h"
#include "OSTask.h"
#include "OSCore.h"
#include "CPU_Port.h"

OS_TCB TCB_IDLE;
OS_STK TASK_IDLE_STK[TASK_IDLE_STK_SIZE];

OS_STK OS_CPU_ExceptStk[OS_EXCEPT_STK_SIZE];


OS_TCBP OS_TCB_TABLE[OS_TASK_MAX_NUM];
OS_TCBP g_OS_Tcb_CurP; 
OS_TCBP g_OS_Tcb_HighRdyP;
OS_U32 OS_TimeTick;
OS_U8 g_Prio_Cur; 
OS_U8 g_Prio_HighRdy;

unsigned long task_idle_tick;

void OS_Task_Idle(void)
{
	while(1)
	{ 
		task_idle_tick++;
		CPU_TO_SLEEP();		
	}
}

void OS_Task_Switch(void)
{
	int i;
	OS_TCBP tcb_p;
	OS_USE_CRITICAL
	for ( i = 0; i < OS_TASK_MAX_NUM; i++ )    	
	{
		tcb_p = OS_TCB_TABLE[i];
		if( tcb_p == NULL ) continue;
		if( tcb_p->State == TASK_READY ) break;
	}
	OS_ENTER_CRITICAL();
	g_OS_Tcb_HighRdyP = tcb_p;
	g_Prio_HighRdy    = i;
	OS_EXIT_CRITICAL();
}

void OS_Task_Delete(OS_U8 prio)
{
    if ( prio >= OS_TASK_MAX_NUM ) return;
    OS_TCB_TABLE[prio] = 0;
}

void OS_Task_End(void)
{
    OS_Task_Delete(g_Prio_Cur);
    OS_Task_Switch();
    OS_PendSV_Trigger();
}

void LRT_OSTask_Create(OS_TCB *tcb,OS_TASK task,OS_STK *stk,OS_U8 prio)
{   
    OS_USE_CRITICAL
    OS_STK  *p_stk; 
    if(prio >= OS_TASK_MAX_NUM) return;
  
    OS_ENTER_CRITICAL();

    p_stk      = stk;
    p_stk      = (OS_STK *)((OS_STK)(p_stk) & 0xFFFFFFF8u);
    
    *(--p_stk) = (OS_STK)0x01000000uL;                          // xPSR
    *(--p_stk) = (OS_STK)task;                                  // Entry Point
    *(--p_stk) = (OS_STK)OS_Task_End;                                     // R14 (LR)
    *(--p_stk) = (OS_STK)0x12121212uL;                          // R12
    *(--p_stk) = (OS_STK)0x03030303uL;                          // R3
    *(--p_stk) = (OS_STK)0x02020202uL;                          // R2
    *(--p_stk) = (OS_STK)0x01010101uL;                          // R1
    *(--p_stk) = (OS_STK)0x00000000u;                           // R0
    
    *(--p_stk) = (OS_STK)0x11111111uL;                          // R11
    *(--p_stk) = (OS_STK)0x10101010uL;                          // R10
    *(--p_stk) = (OS_STK)0x09090909uL;                          // R9
    *(--p_stk) = (OS_STK)0x08080808uL;                          // R8
    *(--p_stk) = (OS_STK)0x07070707uL;                          // R7
    *(--p_stk) = (OS_STK)0x06060606uL;                          // R6
    *(--p_stk) = (OS_STK)0x05050505uL;                          // R5
    *(--p_stk) = (OS_STK)0x04040404uL;                          // R4
    
    tcb->StkAddr       = p_stk;
    tcb->TimeDly       = 0;
    tcb->State         = TASK_READY;
    OS_TCB_TABLE[prio] = tcb; 

    OS_EXIT_CRITICAL();
}

