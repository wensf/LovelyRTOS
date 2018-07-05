#include "LRT_LED.h"
#include "LRT_Driver.h"
#include "LRT_Object.h"

#include "BSP_LED.h"

LRT_Object led_object;

/**
 * LED Low Level Implementation
 */

/**
 * Implementation of Object Operation Function Set (OOFS)
 */

LRT_Object *LED_open( const char *name, int method, int attr )
{
	return &led_object;
}

int LED_read( LRT_Object *object, int len, char *buf )
{
	return 0;
}

int LED_write( LRT_Object *object, int len, const char *buf )
{
	return 0;
}

int LED_ioctl( LRT_Object *object, int cmd, int param )
{
	switch ( cmd )
	{
		case 0:
			LED_set(0);
			break;
		case 1:
			LED_set(1);
			break;
		default:
			break;
	}
	
	return 0;
}

int LED_close( LRT_Object *object )
{
	LED_set(0);
	
	return 0;
}

/**
 * void Driver_LED_init( int identifier )
 * LED Driver Initialization
 */

void Driver_LED_init( int identifier )
{
	LED_GPIO_Init();
	
	led_object.oofs->f_open  = LED_open;
	led_object.oofs->f_read  = LED_read;
	led_object.oofs->f_write = LED_write;
	led_object.oofs->f_ioctl = LED_ioctl;
	led_object.oofs->f_close = LED_close;
	led_object.object_type   = OBJECT_TYPE_IO;
	strcpy(led_object.object_name, "LED1");
	
	LRT_Object_Register(&led_object);
}

