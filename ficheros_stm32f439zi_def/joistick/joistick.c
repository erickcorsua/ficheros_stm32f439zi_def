//joistick

#include "RTE_Components.h"
#include "stm32f4xx_hal.h"
#include "stdbool.h"

#include "joistick.h"

static GPIO_InitTypeDef GPIO_InitStruct;

bool volatile gb_flag_up   = false; 
bool volatile gb_flag_down = false;


void gpio_joistick_init(void);

void EXTI9_5_IRQHandler(void);
void EXTI3_IRQHandler(void);

void gpio_joistick_init(void){

  __HAL_RCC_GPIOB_CLK_ENABLE();
  
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
  
 
  GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	
    
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

            
void EXTI3_IRQHandler(void){

  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);

}

void EXTI9_5_IRQHandler(void){

  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);

}




