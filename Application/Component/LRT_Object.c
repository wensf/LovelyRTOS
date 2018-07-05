
#include "OSType.h"
#include "LRT_Object.h"

LRT_Object* Object_Table[LRT_OBJECT_NR];

LRT_RET LRT_Object_Init(void)
{
	LRT_Object* op = Object_Table[0];
	
	for ( int i = 0; i < SIZEOF(Object_Table); i++ )
	{
		*(&op) = (LRT_Object*)0;
	}
	
	return LRT_OK;
}

int LRT_Object_Register( LRT_Object *object )
{
	for ( int i = 0; i < SIZEOF(Object_Table); i++ )
	{
		if ( 0 == Object_Table[i] )
		{
			Object_Table[i] = object;
			object->identifier = i;
			return LRT_OK;
		}
	}	
	
	return LRT_ERR;
}

/**
 * Application Layer API implements
 */

int LRT_Open( const char *object_name, int method, int attr )
{
	for ( int i = 0; i < SIZEOF(Object_Table); i++ )
	{
		if ( 0 == Object_Table[i] )
		{
			if ( !strncmp( Object_Table[i]->object_name,object_name, LRT_OBJECT_NAME_SIZE) )
			{
				return Object_Table[i]->identifier;
			}
		}
	}	
	
	return (-1);
}

int LRT_Read( int object_id, int len, char *buf )
{
	int idx = -1;
	int ret = -1;
	
	for ( int i = 0; i < SIZEOF(Object_Table); i++ )
	{
		if ( 0 == Object_Table[i] )
		{
			if ( Object_Table[i]->identifier == object_id )
			{
				idx = i;
				break;
			}
		}
	}	
	
	if ( idx != -1 )
	{
		if ( Object_Table[idx]->oofs->f_read != LRT_NULL )
		{
			LRT_Object *object = Object_Table[idx];
			ret = object->oofs->f_read( object->identifier, len, buf);
		}
	}
	
	return ret;
}

int LRT_Write( int object_id, int len, const char *buf )
{
	int idx = -1;
	int ret = -1;
	
	for ( int i = 0; i < SIZEOF(Object_Table); i++ )
	{
		if ( 0 == Object_Table[i] )
		{
			if ( Object_Table[i]->identifier == object_id )
			{
				idx = i;
				break;
			}
		}
	}	
	
	if ( idx != -1 )
	{
		if ( Object_Table[idx]->oofs->f_read != LRT_NULL )
		{
			LRT_Object *object = Object_Table[idx];
			ret = object->oofs->f_write( object->identifier, len, buf);
		}
	}
	
	return ret;
}

int LRT_IOCtl( int object_id, int cmd, int param )
{
	int idx = -1;
	int ret = -1;
	
	for ( int i = 0; i < SIZEOF(Object_Table); i++ )
	{
		if ( 0 == Object_Table[i] )
		{
			if ( Object_Table[i]->identifier == object_id )
			{
				idx = i;
				break;
			}
		}
	}	
	
	if ( idx != -1 )
	{
		if ( Object_Table[idx]->oofs->f_read != LRT_NULL )
		{
			LRT_Object *object = Object_Table[idx];
			ret = object->oofs->f_ioctl( object->identifier, cmd, param);
		}
	}
	
	return ret;
}

int LRT_Close( int object_id )
{
	int idx = -1;
	int ret = -1;
	
	for ( int i = 0; i < SIZEOF(Object_Table); i++ )
	{
		if ( 0 == Object_Table[i] )
		{
			if ( Object_Table[i]->identifier == object_id )
			{
				idx = i;
				break;
			}
		}
	}	
	
	if ( idx != -1 )
	{
		if ( Object_Table[idx]->oofs->f_read != LRT_NULL )
		{
			LRT_Object *object = Object_Table[idx];
			ret = object->oofs->f_close( object->identifier );
		}
	}
	
	return ret;
}
