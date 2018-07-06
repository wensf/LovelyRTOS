#include "stm32f4xx.h"

#include "OSCore.h"
#include "OSTask.h"

#include "Task_GUI.h"

OS_STK TASK_Timer_STK[TASK_Timer_STK_SIZE];

unsigned long task_gui_ticks = 0;


unsigned short fb[LCD_H][LCD_W];

#define SPI_CLK        			RCC_APB2Periph_SPI1
#define SPI_PORT            	 	 GPIOA         
#define SPI_DC_PIN     			GPIO_Pin_3
#define SPI_CS_PIN     			GPIO_Pin_4
#define SPI_MOSI_PIN   			GPIO_Pin_7
#define SPI_MISO_PIN   			GPIO_Pin_6
#define SPI_RST_PIN    			GPIO_Pin_2
#define SPI_SCK_PIN    			GPIO_Pin_5

#define CS_ENABLE()   			GPIO_ResetBits(SPI_PORT,SPI_CS_PIN)
#define CS_DISABLE()  			GPIO_SetBits(SPI_PORT,SPI_CS_PIN)
#define DC_SET() 				GPIO_SetBits(SPI_PORT,SPI_DC_PIN)
#define DC_CLEAR() 				GPIO_ResetBits(SPI_PORT,SPI_DC_PIN)
#define RST_SET() 				GPIO_SetBits(SPI_PORT,SPI_RST_PIN)
#define RST_CLEAR() 			GPIO_ResetBits(SPI_PORT,SPI_RST_PIN)

#define SPI_CLK_GPIO_CLK 		RCC_AHB1Periph_GPIOA
#define SPI_MISO_GPIO_CLK 		RCC_AHB1Periph_GPIOA
#define SPI_MOSI_GPIO_CLK 		RCC_AHB1Periph_GPIOA
#define SPI_CS_GPIO_CLK 		RCC_AHB1Periph_GPIOA


#if 0
static unsigned char SPI_write( unsigned char byte)
{
  /*!< Loop while DR register in not empty */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

  /*!< Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(SPI1, byte);

  /*!< Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

  /*!< Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI1);
}
#else

static void SPI_write( unsigned char byte)
{
  while((SPI1->SR & SPI_I2S_FLAG_BSY) != RESET);
	SPI1->DR = byte;
	
  /*!< Loop while DR register in not empty */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
}
#endif

void LCD_WrCmd(unsigned char val)
{
	CS_ENABLE();
	DC_CLEAR();
	SPI_write(val);
	DC_SET();
	CS_DISABLE();
}

void LCD_WrDat(unsigned char val)
{
	CS_ENABLE();
	DC_SET();
	SPI_write(val);
	DC_CLEAR();
	CS_DISABLE();
}

void LCD_Pin_Configuration(void)
{
	/*!< SPI pins configuration *************************************************/

	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable SPI Clock */

	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_SPI1, ENABLE);	
	
	/*!< Enable GPIO clocks */
						 
	RCC_AHB1PeriphClockCmd ( RCC_AHB1Periph_GPIOA, ENABLE);
	
	/*!< Connect SPI pins to AF5 */  
	GPIO_PinAFConfig(SPI_PORT, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(SPI_PORT, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(SPI_PORT, GPIO_PinSource7, GPIO_AF_SPI1);	

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
		
	/*!< SPI SCK pin configuration */
	GPIO_InitStructure.GPIO_Pin = SPI_SCK_PIN | SPI_MOSI_PIN | SPI_MISO_PIN;
	GPIO_Init(SPI_PORT, &GPIO_InitStructure);

	/*!< Configure CS RST D/C pin in output pushpull mode ********************/

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	
	GPIO_InitStructure.GPIO_Pin   = SPI_CS_PIN | SPI_DC_PIN | SPI_RST_PIN;	
	GPIO_Init(SPI_PORT, &GPIO_InitStructure);	
				
	GPIO_SetBits(SPI_PORT,SPI_RST_PIN);
	GPIO_SetBits(SPI_PORT,SPI_DC_PIN);	
	
	/*!< SPI configuration */

	SPI_Cmd( SPI1,DISABLE );
	SPI_I2S_DeInit(SPI1);
	
					 	
	SPI_InitTypeDef  SPI_InitStructure;

	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
	SPI_InitStructure.SPI_Mode      = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize  = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL      = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA      = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS       = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;

	SPI_InitStructure.SPI_FirstBit  = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);

	/*!< Enable the sFLASH_SPI  */
	SPI_Cmd(SPI1, ENABLE);	
	
	CS_DISABLE();
}

void lcd_init(void)
{
	CS_ENABLE();
	RST_CLEAR();
	OS_Delay(20);
	RST_SET();
	OS_Delay(20);

	CS_DISABLE();
	
	//************* Start Initial Sequence **********// 
	LCD_WrCmd(0xCF);  
	LCD_WrDat(0x00); 
	LCD_WrDat(0xD9); 
	LCD_WrDat(0X30); 
	 
	LCD_WrCmd(0xED);  
	LCD_WrDat(0x64); 
	LCD_WrDat(0x03); 
	LCD_WrDat(0X12); 
	LCD_WrDat(0X81); 
	 
	LCD_WrCmd(0xE8);  
	LCD_WrDat(0x85); 
	LCD_WrDat(0x10); 
	LCD_WrDat(0x78); 
	 
	LCD_WrCmd(0xCB);  
	LCD_WrDat(0x39); 
	LCD_WrDat(0x2C); 
	LCD_WrDat(0x00); 
	LCD_WrDat(0x34); 
	LCD_WrDat(0x02); 
	 
	LCD_WrCmd(0xF7);  
	LCD_WrDat(0x20); 
	 
	LCD_WrCmd(0xEA);  
	LCD_WrDat(0x00); 
	LCD_WrDat(0x00); 
	 
	LCD_WrCmd(0xC0);    //Power control 
	LCD_WrDat(0x21);    //VRH[5:0] 
	 
	LCD_WrCmd(0xC1);    //Power control 
	LCD_WrDat(0x12);    //SAP[2:0];BT[3:0] 
	 
	LCD_WrCmd(0xC5);    //VCM control 
	LCD_WrDat(0x32); 
	LCD_WrDat(0x3C); 
	 
	LCD_WrCmd(0xC7);    //VCM control2 
	LCD_WrDat(0XC1); 
	 
	LCD_WrCmd(0x36);    // Memory Access Control 
	LCD_WrDat(0x08); 
	 
	LCD_WrCmd(0x3A);   
	LCD_WrDat(0x55); 

	LCD_WrCmd(0xB1);   
	LCD_WrDat(0x00);   
	LCD_WrDat(0x18); 
	 
	LCD_WrCmd(0xB6);    // Display Function Control 
	LCD_WrDat(0x0A); 
	LCD_WrDat(0xA2); 
	 
	LCD_WrCmd(0xF2);    // 3Gamma Function Disable 
	LCD_WrDat(0x00); 
	 
	LCD_WrCmd(0x26);    //Gamma curve selected 
	LCD_WrDat(0x01); 
	 
	LCD_WrCmd(0xE0);    //Set Gamma 
	LCD_WrDat(0x0F); 
	LCD_WrDat(0x20); 
	LCD_WrDat(0x1E); 
	LCD_WrDat(0x09); 
	LCD_WrDat(0x12); 
	LCD_WrDat(0x0B); 
	LCD_WrDat(0x50); 
	LCD_WrDat(0XBA); 
	LCD_WrDat(0x44); 
	LCD_WrDat(0x09); 
	LCD_WrDat(0x14); 
	LCD_WrDat(0x05); 
	LCD_WrDat(0x23); 
	LCD_WrDat(0x21); 
	LCD_WrDat(0x00); 
	 
	LCD_WrCmd(0XE1);    //Set Gamma 
	LCD_WrDat(0x00); 
	LCD_WrDat(0x19); 
	LCD_WrDat(0x19); 
	LCD_WrDat(0x00); 
	LCD_WrDat(0x12); 
	LCD_WrDat(0x07); 
	LCD_WrDat(0x2D); 
	LCD_WrDat(0x28); 
	LCD_WrDat(0x3F); 
	LCD_WrDat(0x02); 
	LCD_WrDat(0x0A); 
	LCD_WrDat(0x08); 
	LCD_WrDat(0x25); 
	LCD_WrDat(0x2D); 
	LCD_WrDat(0x0F); 
	 
	LCD_WrCmd(0x11);    //Exit Sleep 
	OS_Delay(120); 
	LCD_WrCmd(0x29);    //Display on		
}

#include <string.h>

void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{ 
	LCD_WrCmd(0x2a);
	LCD_WrDat(x1>>8);
	LCD_WrDat(x1);
	LCD_WrDat(x2>>8);
	LCD_WrDat(x2);
  
	LCD_WrCmd(0x2b);
	LCD_WrDat(y1>>8);
	LCD_WrDat(y1);
	LCD_WrDat(y2>>8);
	LCD_WrDat(y2);

	LCD_WrCmd(0x2C);					 						 
}

void LCD_Fill_Ex(int x, int y, int w, int h, unsigned short color)
{  	
	Address_set(x,y,x+w-1,y+h-1);
	
    for ( int xx = x; xx < x+w; xx++ )
	{
		for ( int yy = 0; yy < y+h; yy++ )
		{
			LCD_WrDat(color>>8);
			LCD_WrDat(color&0xFF);			
		}
	}
}

void LCD_Fill(unsigned short color)
{	
	LCD_Fill_Ex(0,0,LCD_W,LCD_H,color );
}

void LCD_Sync_Frame_Ex( int x, int y, int w, int h )
{
	unsigned short *p;
	
	Address_set(x,y,x+w-1,y+h-1);
	
	for ( int yy = y; yy < y+h; yy++ )
	{			
		p = &fb[yy][x];
		for ( int xx = x; xx < x+w; xx++ )
		{		
			LCD_WrDat(*p>>8);
			LCD_WrDat(*p&0xFF);
			p++;
		}
	}	
}

void LCD_Sync_Frame(void)
{
	LCD_Sync_Frame_Ex(0,0,LCD_W, LCD_H);	
}

void Draw_Bitmap( int x, int y, int w, int h, const unsigned char *bmp)
{
	const unsigned short *p;
	
	p = (unsigned short*)bmp;
	
	for ( int yy = y; yy < y+h; yy++ )
	{
		for ( int xx = x; xx < x + w; xx++ )
		{
			fb[yy][xx] = *p++;
		}
	}	
}

void LCD_Fill_FB(int x, int y, int w, int h, unsigned short bmp_data)
{
	for ( int xx = x; xx < x + w; xx++ )
	{
		for ( int yy = y; yy < y+h; yy++ )
		{
			fb[yy][xx] = bmp_data;
		}
	}	
}

int x = 0, y = 0;

void Task_GUI(void)
{
//	LCD_Pin_Configuration();
	
//	lcd_init();
	
//	LCD_Fill(0x1F);

//	unsigned short color  = 0;
	
	while(1)
	{
		OS_Delay(80);
//		LCD_Fill(color++);	
//		LCD_Sync_Frame();		
		task_gui_ticks++;
	}
}
