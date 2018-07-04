#ifndef __TASK_GUI_H__
#define __TASK_GUI_H__

#define TASK_Timer_STK_SIZE 256

#include "OSTask.h"

#define LCD_W 240
#define LCD_H 320

extern unsigned short fb[LCD_H][LCD_W];

#define put_pixel(x,y,color) \
do{\
    if ( ((x) < LCD_W) && ((y) < LCD_H) )\
    {\
        fb[y][x] = color;\
    }\
}while(0)

extern OS_STK TASK_Timer_STK[TASK_Timer_STK_SIZE];

extern void Task_GUI(void);


extern const unsigned char gImage_girl[147200];

#endif
