#ifndef __OSTASK_H__
#define __OSTASK_H__

#include "OSType.h"

#define OS_EXCEPT_STK_SIZE 1024
#define TASK_1_STK_SIZE 512
#define TASK_2_STK_SIZE 512
#define TASK_3_STK_SIZE 512

#define TASK_IDLE_STK_SIZE 1024
#define OS_TASK_MAX_NUM 4
#define OS_HZ 100
#define OS_A_TICK (1000/OS_HZ)

extern void OSStart_Asm(void);
extern void OSCtxSw(void);
extern void OS_CPU_SR_Restore(OS_U32);
extern unsigned long OS_CPU_SR_Save(void);

#define  OS_USE_CRITICAL    OS_U32 cpu_sr;
#define  OS_ENTER_CRITICAL()  cpu_sr = OS_CPU_SR_Save()
#define  OS_EXIT_CRITICAL()   OS_CPU_SR_Restore(cpu_sr)
#define  OS_PendSV_Trigger() OSCtxSw()

typedef void (*OS_TASK)(void);

typedef enum OS_TASK_STA
{
	TASK_READY,
	TASK_DELAY,
} OS_TASK_STA;

typedef struct OS_TCB
{
	OS_STK *StkAddr;
	OS_S32 TimeDly;
	OS_TASK_STA State;
}OS_TCB,*OS_TCBP;

extern OS_TCBP OS_TCB_TABLE[OS_TASK_MAX_NUM];
extern OS_TCBP g_OS_Tcb_CurP; 
extern OS_TCBP g_OS_Tcb_HighRdyP;
extern OS_U32 OS_TimeTick;
extern OS_U8 g_Prio_Cur; 
extern OS_U8 g_Prio_HighRdy;


extern OS_STK OS_CPU_ExceptStk[OS_EXCEPT_STK_SIZE];
extern OS_TCB TCB_IDLE;
extern OS_STK TASK_IDLE_STK[TASK_IDLE_STK_SIZE];

void OS_Task_Switch(void);

extern void OS_Task_Idle(void);
void OS_Task_Delete(OS_U8 prio);
void LRT_OSTask_Create(OS_TCB *tcb,OS_TASK task,OS_STK *stk,OS_U8 prio);

#endif
