#include "OSType.h"
#include "OSCore.h"
#include "CPU_Port.h"

void LRT_OS_Clock_Init(void)
{
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/OS_HZ);
}
