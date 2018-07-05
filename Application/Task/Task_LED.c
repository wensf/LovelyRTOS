
#include "OSCore.h"
#include "OSTask.h"
#include "Task_LED.h"
#include "Draw.h"

unsigned long Task_LED_tick;

#include "Task_GUI.h"
#include <string.h>

void Task_LED(void)
{
	while(1)
	{
		Task_LED_tick++;

		OS_Delay(500);
	}
}
