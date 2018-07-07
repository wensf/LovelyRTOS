#ifndef __OSTYPE_H__
#define __OSTYPE_H__

#include <stdio.h>
#include <string.h>

typedef signed char OS_S8;
typedef signed short OS_S16;
typedef signed int OS_S32;
typedef unsigned char OS_U8;
typedef unsigned short OS_U16;
typedef unsigned int OS_U32;
typedef unsigned int OS_STK;
typedef int LRT_RET;

#define LRT_OK             0
#define LRT_ERR           -1

#define LRT_NULL           0

#define SIZEOF(x)          (sizeof(x)/sizeof(x[0]))
    
#endif
