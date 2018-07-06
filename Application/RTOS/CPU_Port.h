#ifndef __CPU_PORT_H__
#define __CPU_PORT_H__

#include "stm32f4xx.h"

#define CPU_IRQ_DISABLE() __asm("CPSID   I")
#define CPU_TO_SLEEP() __asm("WFI") 

extern void LRT_OS_Clock_Init(void);

#endif

