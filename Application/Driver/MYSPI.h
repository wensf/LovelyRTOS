#ifndef _MYSPI_H
#define _MYSPI_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* M25P FLASH SPI Interface pins  */  
#define SPIX                           	SPI1
#define SPIX_CLK                       	RCC_APB2Periph_SPI1
#define SPIX_CLK_INIT                  	RCC_APB2PeriphClockCmd

#define SPIX_SCK_PIN                   	GPIO_Pin_5
#define SPIX_SCK_GPIO_PORT             	GPIOA
#define SPIX_SCK_GPIO_CLK              	RCC_AHB1Periph_GPIOA
#define SPIX_SCK_SOURCE                	GPIO_PinSource5
#define SPIX_SCK_AF                    	GPIO_AF_SPI1
 
#define SPIX_MOSI_PIN                  	GPIO_Pin_7
#define SPIX_MOSI_GPIO_PORT            	GPIOA
#define SPIX_MOSI_GPIO_CLK             	RCC_AHB1Periph_GPIOA
#define SPIX_MOSI_SOURCE               	GPIO_PinSource7
#define SPIX_MOSI_AF                   	GPIO_AF_SPI1

int SPI_Read_Write(uint8_t data);
void SPI_Configuration(void);
extern int SPI_write(unsigned char val);

#endif

/*******************************************************************************
      END FILE
*******************************************************************************/

