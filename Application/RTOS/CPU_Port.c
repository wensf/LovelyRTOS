#include "OSType.h"
#include "OSCore.h"
#include "OSTask.h"
#include "CPU_Port.h"

#define    NVIC_INT_CTRL        0xE000ED04                        //      ; Interrupt control state register.
#define    NVIC_SYSPRI14        0xE000ED22                        //      ; System priority register (priority 14).
#define    NVIC_PENDSV_PRI            0xFF                        //      ; PendSV priority value (lowest).
#define    NVIC_PENDSVSET       0x10000000                        //      ; Value to trigger PendSV exception.

void LRT_OS_Clock_Init(void)
{
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/OS_HZ);
}

void OS_CPU_PendSVHandler_nosave (void);

__ASM void PendSV_Handler(void)
{	
    extern g_OS_Tcb_CurP
    CPSID   I                                                  ; /* Prevent interruption during context switch  */
    MRS     R0, PSP                                            ; /* PSP is process stack pointer  */
    CBZ     R0, OS_CPU_PendSVHandler_nosave                    ; /* Skip register save the first time  */

    SUBS    R0, R0, #0x20                                    ;   /* Save remaining regs r4-11 on process stack  */
    STM     R0, {R4-R11}

    LDR     R1, =g_OS_Tcb_CurP                                ;  /* OSTCBCur->OSTCBStkPtr = SP;  */
    LDR     R1, [R1]
    STR     R0, [R1]                                         ; /* R0 is SP of process being switched out   At this point, entire context of process has been saved */
}

__ASM void OS_CPU_PendSVHandler_nosave (void)
{ 
    extern g_Prio_Cur
    extern g_Prio_HighRdy
    extern g_OS_Tcb_HighRdyP

    LDR     R0, =g_Prio_Cur;                                     /* OSPrioCur = OSPrioHighRdy;*/
    LDR     R1, =g_Prio_HighRdy;
    LDRB    R2, [R1]
    STRB    R2, [R0]

    LDR     R0, =g_OS_Tcb_CurP;                                  /* OSTCBCur  = OSTCBHighRdy;*/
    LDR     R1, =g_OS_Tcb_HighRdyP;
    LDR     R2, [R1]
    STR     R2, [R0]

    LDR     R0, [R2]                                      	    /*  ; R0 is new process SP; SP = OSTCBHighRdy->OSTCBStkPtr;*/

    LDM     R0, {R4-R11};                                         /* ; Restore r4-11 from new process stack*/
    ADDS    R0, R0, #0x20;
        
    MSR     PSP, R0;                                              /* ; Load PSP with new process SP*/
    ORR     LR, LR, #0x04;                                        /* ; Ensure exception return uses process stack*/

    CPSIE   I
    BX      LR
}

__ASM void OSStart_Asm(void)
{				    
	extern 	g_OS_CPU_ExceptStkBase		
		
    LDR     R0, =NVIC_SYSPRI14                                  ; Set the PendSV exception priority
    LDR     R1, =NVIC_PENDSV_PRI
    STRB    R1, [R0]

    MOVS    R0, #0                                              ; Set the PSP to 0 for initial context switch call
    MSR     PSP, R0

    LDR     R0, =g_OS_CPU_ExceptStkBase                           ; Initialize the MSP to the OS_CPU_ExceptStkBase
    LDR     R1, [R0]
    MSR     MSP, R1    

    LDR     R0, =NVIC_INT_CTRL                                  ; Trigger the PendSV exception (causes context switch)
    LDR     R1, =NVIC_PENDSVSET
    STR     R1, [R0]

    CPSIE   I                                                   ; Enable interrupts at processor level
}

__ASM unsigned long OS_CPU_SR_Save(void)
{
    MRS     R0, PRIMASK
    CPSID   I
    BX      LR
}

__ASM void OSCtxSw (void)
{
    LDR     R0, =NVIC_INT_CTRL
    LDR     R1, =NVIC_PENDSVSET
    STR     R1, [R0]
    BX      LR 
    ALIGN
}

__ASM void OS_CPU_SR_Restore(OS_U32 sr)  
{
    MSR     PRIMASK, R0
    BX      LR
}

