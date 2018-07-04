#include "MYSPI.h"
#include <stdio.h>

/*******************************************************************************
* Function Name  : MSD0_spi_read_write
* Description    : None
* Input          : - data:
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
int SPI_Read_Write(uint8_t data)
{
	//< Loop while DR register in not emplty  
	while (SPI_I2S_GetFlagStatus(SPIX, SPI_I2S_FLAG_TXE) == RESET);

	//< Send byte through the SPI1 peripheral  
	SPI_I2S_SendData(SPIX, data);

	//< Wait to receive a byte  
	while (SPI_I2S_GetFlagStatus(SPIX, SPI_I2S_FLAG_RXNE) == RESET);

	// < Return the byte read from the SPI bus  
	return SPI_I2S_ReceiveData(SPIX);  
}

int SPI_write(unsigned char val)
{
	//< Loop while DR register in not emplty  
	// while (SPI_I2S_GetFlagStatus(SPIX, SPI_I2S_FLAG_TXE) == RESET);
	while (SPI_I2S_GetFlagStatus(SPIX, SPI_I2S_FLAG_BSY) != RESET);                      	
	//SPI_I2S_ClearFlag(SPIX,SPI_I2S_FLAG_TXE);
	//< Send byte through the SPI1 peripheral  
	SPI_I2S_SendData(SPIX, val);

	while (SPI_I2S_GetFlagStatus(SPIX, SPI_I2S_FLAG_BSY) != RESET);
	
	return 0;
}

/*******************************************************************************
* Function Name  : MSD0_SPI_Configuration
* Description    : SD Card SPI Configuration
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void SPI_Configuration(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure;

	// SPI PeriphClock Enable
	SPIX_CLK_INIT(SPIX_CLK, ENABLE);

	//IO PeriphClock Enable
	RCC_AHB1PeriphClockCmd(SPIX_SCK_GPIO_CLK | //SPIX_MISO_GPIO_CLK | 
						 SPIX_MOSI_GPIO_CLK/* | SPIX_CS_GPIO_CLK*/, ENABLE);

	// SPI Fonction Configuration
	GPIO_PinAFConfig(SPIX_SCK_GPIO_PORT, SPIX_SCK_SOURCE, SPIX_SCK_AF);
	GPIO_PinAFConfig(SPIX_MOSI_GPIO_PORT, SPIX_MOSI_SOURCE, SPIX_MOSI_AF);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
		
	/*!< SPI SCK pin configuration */
	GPIO_InitStructure.GPIO_Pin = SPIX_SCK_PIN;
	GPIO_Init(SPIX_SCK_GPIO_PORT, &GPIO_InitStructure);

	/*!< SPI MOSI pin configuration */
	GPIO_InitStructure.GPIO_Pin =  SPIX_MOSI_PIN;
	GPIO_Init(SPIX_MOSI_GPIO_PORT, &GPIO_InitStructure);

	SPI_InitTypeDef SPI_InitStructure;

	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;	
    SPI_Init(SPIX, &SPI_InitStructure);
	
	SPI_Cmd(SPIX, ENABLE); 
}

