#ifndef __LRT_OBJECT_H__
#define __LRT_OBJECT_H__

#include "OSType.h"

#define LRT_OBJECT_NR        		1
#define LRT_OBJECT_NAME_SIZE       16

#define OBJECT_TYPE_IO              0

typedef struct Object_tag LRT_Object;
typedef struct LRT_oofs_tag LRT_oofs;

struct Object_tag
{
	char object_name[LRT_OBJECT_NAME_SIZE];
	LRT_oofs *oofs;
	int object_type;
};

/**
 * The oofs means 'Object Operation Function Set'
 */

struct LRT_oofs_tag
{
	LRT_Object* (*f_open)( const char *object_name, int method, int attr );
	int (*f_read)( LRT_Object *object, int len, char *buf );
	int (*f_write)( LRT_Object *object, int len, const char *buf );
	int (*f_ioctl) (LRT_Object *object, int cmd, int param );
	int (*f_close)( LRT_Object *object );
};

extern LRT_RET LRT_Object_Init(void);
extern int LRT_Object_Register( LRT_Object *object );

#endif
