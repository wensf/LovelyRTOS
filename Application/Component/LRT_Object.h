#ifndef __LRT_OBJECT_H__
#define __LRT_OBJECT_H__

#include "OSType.h"

#define LRT_OBJECT_NR        		1
#define LRT_OBJECT_NAME_NR         16

#define OBJECT_TYPE_IO              0

typedef struct Object_tag LRT_Object;
typedef struct LRT_oofs_tag LRT_oofs;

struct Object_tag
{
	char object_name[LRT_OBJECT_NAME_NR];
	int object_type;
	int identifier;
	LRT_oofs *oofs;	
};

/**
 * The oofs means 'Object Operation Function Set'
 */

struct LRT_oofs_tag
{
	int (*f_open)( const char *object_name, int method, int attr );
	int (*f_read)( int identifier, int len, char *buf );
	int (*f_write)( int identifier, int len, const char *buf );
	int (*f_ioctl)(int identifier, int cmd, int param );
	int (*f_close)( int identifier );
};

extern LRT_RET LRT_Object_Init(void);
extern int LRT_Object_Register( LRT_Object *object );


/**
 * Application Layer API declaration
 */
extern int LRT_Open( const char *object_name, int method, int attr );
extern int LRT_Read( int object_id, int len, char *buf );
extern int LRT_Write( int object_id, int len, const char *buf );
extern int LRT_IOCtl( int object_id, int cmd, int param );
extern int LRT_Close( int object_id );

#endif
