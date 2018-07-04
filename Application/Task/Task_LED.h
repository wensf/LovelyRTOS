#ifndef __TASK_LED_H__
#define __TASK_LED_H__

#define LED1_PIN            GPIO_Pin_7       
#define LED1_PORT           GPIOG                   
#define LED1_CLK            RCC_AHB1Periph_GPIOG	  

#define LED2_PIN            GPIO_Pin_12                      
#define LED2_PORT           GPIOD                       
#define LED2_CLK            RCC_AHB1Periph_GPIOD    

/*---------------------- LED Pin Definition ------------------------*/
					
#define LED1_ON() 	  		GPIO_ResetBits(LED1_PORT,LED1_PIN);	 
#define LED1_OFF() 	 	 	GPIO_SetBits(LED1_PORT,LED1_PIN);		 

#define LED2_ON 	  		GPIO_ResetBits(LED2_PORT,LED2_PIN);	 
#define LED2_OFF 	  		GPIO_SetBits(LED2_PORT,LED2_PIN);		 

extern void Task_LED(void);

#endif
