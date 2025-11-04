//intro 
//titulo: boton_azul_1.0
//autor: erick cs
//descripcion: Fichero para el boton azul, (user), que se encuentra nativo en la tarjeta stm32. 
//             Esta version es ideal para examenes, es rapida de usar y sencilla, no se tiene en cuenta
//             rebotes y gestiones de pulsaciones cortas y largas. pin PC13 stm32 f4 39zi

//libreria

#include "boton_azul.h"

#include "RTE_Components.h"
#include "stm32f4xx_hal.h"
#include "stdbool.h"

//macros
#define GPIO_BOTON_AZUL GPIO_PIN_13
#define GPIO_BTN_PORT GPIOC

//variable

static GPIO_InitTypeDef GPIO_InitStruct;

volatile bool gb_btn_azul_corto = false;

//funtion prototye
void boton_azul_init(void);

void boton_azul_callback(void);

//void boton_azul_irq_handler(void); --> si usas mas pines con numero
//                                       10 al 15, debes habilitar esto y tocar en el stm32f4xx_it.c



//funtion
//*****************************************************
//*****************************************************

				//Inicializar el boton azul(user) debe ir en el main

//*****************************************************
//*****************************************************

void boton_azul_init(void){
  
  __HAL_RCC_GPIOC_CLK_ENABLE(); 
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn); 	

  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN; 
  GPIO_InitStruct.Pin = GPIO_BOTON_AZUL;
	
  HAL_GPIO_Init(GPIO_BTN_PORT, &GPIO_InitStruct);  

}
//*****************************************************
//*****************************************************

				//IRQ_Handler para el boton azul

//*****************************************************
//*****************************************************
/*
void boton_azul_irq_handler(void){

  HAL_GPIO_EXTI_IRQHandler(GPIO_BOTON_AZUL); si usas mas pines con numero
//                                           10 al 15, debes habilitar esto y tocar en el stm32f4xx_it.c
}
*/
void EXTI15_10_IRQHandler(void){

  HAL_GPIO_EXTI_IRQHandler(GPIO_BOTON_AZUL);

}                                       

//*****************************************************
//*****************************************************

				//Callbacks

//*****************************************************
//*****************************************************
void boton_azul_callback(void){

	gb_btn_azul_corto = true;

}
/*
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
    if(GPIO_Pin == GPIO_BOTON_AZUL){
        boton_azul_callback();
    }
  }
	*/
