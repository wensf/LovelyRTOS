#include "LRT_Driver.h"
#include "OSType.h"
#include "Driver_LED.h"

static LRT_Driver LRT_Driver_Table[] = 
{
	Driver_LED_init,
};

int LRT_Driver_Init(void)
{
	for ( int i = 0; i < SIZEOF(LRT_Driver_Table); i++ )
	{
		if ( 0 != LRT_Driver_Table[i].init )
		{
			LRT_Driver_Table[i].init(i);
		}
	}
	
	return 0;
}

