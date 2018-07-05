
#include "OSCore.h"
#include "OSTask.h"
#include "Task_LED.h"
#include "Draw.h"

unsigned long Task_LED_tick;

#include "Task_GUI.h"
#include "LRT_Object.h"

void Task_LED(void)
{
	int id = LRT_Open("LED1", 0, 0 );
	
	if ( id != -1 )
	{
		while ( 1 )
		{
			Task_LED_tick++;

			OS_Delay(1000);
			
			LRT_IOCtl ( id, 0, 0 );
			
			OS_Delay(1000);
			
			LRT_IOCtl ( id, 1, 0 );
		}
		
		LRT_Close( id );
		
	}else{
		
		LRT_Printf("Open LED1 Failed\n");
	}
}
