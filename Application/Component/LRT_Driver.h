#ifndef __LRT_DRIVER_H__
#define __LRT_DRIVER_H__

typedef struct LRT_Driver_tag
{
	void (*init)( int identifier);
}LRT_Driver;

extern int LRT_Driver_Init(void);

#endif
