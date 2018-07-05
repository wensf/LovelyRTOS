
#include "OSType.h"
#include "LRT_Object.h"

LRT_Object* LRT_Object_Table[LRT_OBJECT_NR];

LRT_RET LRT_Object_Init(void)
{
	LRT_Object* op = LRT_Object_Table[0];
	
	for ( int i = 0; i < SIZEOF(LRT_Object_Table); i++ )
	{
		*(&op) = (LRT_Object*)0;
	}
	
	return LRT_OK;
}

int LRT_Object_Register( LRT_Object *object )
{
	for ( int i = 0; i < SIZEOF(LRT_Object_Table); i++ )
	{
		if ( 0 == LRT_Object_Table[i] )
		{
			LRT_Object_Table[i] = object;
			return LRT_OK;
		}
	}	
	
	return LRT_ERR;
}

LRT_Object * LRT_Object_Malloc(void)
{
	return (LRT_Object*)(0);
}
