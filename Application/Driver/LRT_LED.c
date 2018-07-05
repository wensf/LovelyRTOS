#include "LRT_LED.h"
#include "LRT_Driver.h"
#include "LRT_Object.h"

#include "BSP_LED.h"

static LRT_Object led_object;
static LRT_oofs led_oofs;

/**
 * Implementation of Object Operation Function Set (OOFS)
 */

int LED_open( const char *name, int method, int attr )
{
	return led_object.identifier;
}

int LED_read( int identifier, int len, char *buf )
{
	return 0;
}

int LED_write( int identifier, int len, const char *buf )
{
	return 0;
}

int LED_ioctl( int identifier, int cmd, int param )
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

int LED_close( int identifier )
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
	
	led_oofs.f_open  = LED_open;
	led_oofs.f_read  = LED_read;
	led_oofs.f_write = LED_write;
	led_oofs.f_ioctl = LED_ioctl;
	led_oofs.f_close = LED_close;
	
	led_object.oofs  = &led_oofs;
	led_object.object_type   = OBJECT_TYPE_IO;
	strcpy(led_object.object_name, "LED1");
	
	LRT_Object_Register(&led_object);
}

