#include "Task_GUI.h"

////////////////////////////////////////////////////////////////////////////////  
//                                                                            
// 
//  
// void DrawUI_Bitmap( int x, int y, int w, int h, \
//	const unsigned char *bitmap,unsigned long F, unsigned long B);     
//	
//  x,y start coordinate                                                     
//  w,h Bitmap width and height                                   
//  bitmap Bitmap data pointer                                 
//  该函数用于绘制单色位图, xy为起始坐标,wh为宽度和高度                 
//  bitmap为图片数据指针,FB为图片数据中01对应的颜色.                    
//  需要注意的是, 图片的高度必须设置为8的整数倍                   
//                                                            
//////////////////////////////////////////////////////////////////////////////// 

void DrawUI_Bitmap_Ex( int x, int y, int w, int h,\
	const unsigned char *bitmap,unsigned long fc) 
{  
	unsigned long temp; 
	if ( h % 8 ){  return; }  

	for ( int k = 0; k < w; k++ ) 
	for ( int i = 0; i < h/8; i++ ) 
	{  
		temp = bitmap[(h/8)*k+i];  
		for ( int j = 0; j < 8; j++ )  
		{  
			if ( (temp>>j) & 0x1) 
			{  
				put_pixel(x+k, i*8+y+j, fc); 
			}			
		}      
	} 
}
