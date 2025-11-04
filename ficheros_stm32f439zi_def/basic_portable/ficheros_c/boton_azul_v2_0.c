//intro 
//titulo: boton_azul_2.0
//autor: erick cs
//descripcion: Fichero para el boton azul, (user), que se encuentra nativo en la tarjeta stm32. 
//             En esta version, usamos el timer global y el timer virtual para gestionar pulsaciones cortas
//             y largas.

//libreria

#include "boton_azul_v2_0.h"

#include "RTE_Components.h"
#include "stm32f4xx_hal.h"
#include "stdbool.h"

#include "timer_virtual.h"

//macros
#define GPIO_BOTON_AZUL GPIO_PIN_13
#define GPIO_BTN_PORT GPIOC

#define PULSO_LARGO_MS 1000
//variable

typedef enum{ BTN_IDLE, BTN_IN, BTN_OUT} btn_azul_state_t;
static btn_azul_state_t estado_btn_azul;


static GPIO_InitTypeDef GPIO_InitStruct;

volatile bool gb_btn_azul_rise_evnt = false;

static bool b_btn_azul_init = false;

volatile bool gb_btn_azul_corto = false;
volatile bool gb_btn_azul_larga = false;

static uint16_t count_ms_btn_azul;
TimerVirtual tim_btn_azul;

//funtion prototye
void boton_azul_init(void);
void boton_azul_callback(void);
void EXTI15_10_IRQHandler(void);

void boton_azul_fsm(void);


//void boton_azul_irq_handler(void); --> si usas mas pines con numero
//                                       10 al 15, debes habilitar esto y tocar en el stm32f4xx_it.c


//void boton_azul_callback(void);

//funtion
//*****************************************************
//*****************************************************

				//Inicializar el boton azul(user) debe ir en el main

//*****************************************************
//*****************************************************

void boton_azul_init(void){
  
	if(!b_btn_azul_init){
		
		__HAL_RCC_GPIOC_CLK_ENABLE(); 
		HAL_NVIC_EnableIRQ(EXTI15_10_IRQn); 	

		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN; 
		GPIO_InitStruct.Pin = GPIO_BOTON_AZUL;
		
		HAL_GPIO_Init(GPIO_BTN_PORT, &GPIO_InitStruct);  
		
		TimerVirtual_Iniciar(&tim_btn_azul,1);
		
		b_btn_azul_init = true;
		estado_btn_azul = BTN_IDLE;
	}
}
//*****************************************************
//*****************************************************

				//IRQ_Handler para el boton azul

//*****************************************************
//*****************************************************
/*
void boton_azul_irq_handler(void){

  HAL_GPIO_EXTI_IRQHandler(GPIO_BOTON_AZUL);

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

  gb_btn_azul_rise_evnt = true;

}

//-----------------------------

//*****************************************************
//*****************************************************

//fsm(maquina de estados finitos :-) )

//*****************************************************
//*****************************************************
void boton_azul_fsm(void){

	if(b_btn_azul_init){
		
		switch(estado_btn_azul){
		
			case BTN_IDLE:
				
				count_ms_btn_azul = 0;
			  gb_btn_azul_corto = false;
			  gb_btn_azul_larga = false;
			
			  if(gb_btn_azul_rise_evnt){
				  gb_btn_azul_rise_evnt = false;
					
					estado_btn_azul = BTN_IN;
				
				}
				
			break;
			
			case BTN_IN:
				if(tim_btn_azul.timer_virtual_flag){
				  tim_btn_azul.timer_virtual_flag = false;
					
					count_ms_btn_azul++;
					if(count_ms_btn_azul >= 20){
					
						if(HAL_GPIO_ReadPin(GPIO_BTN_PORT,GPIO_BOTON_AZUL) == GPIO_PIN_SET){
						
							estado_btn_azul = BTN_OUT;
						
						}
						else{
							
							estado_btn_azul = BTN_IDLE;
						
						}
					}
				}
			
			break;
		
			case BTN_OUT:
				if(tim_btn_azul.timer_virtual_flag){
				  tim_btn_azul.timer_virtual_flag = false;
					
					count_ms_btn_azul++;
					
					if(HAL_GPIO_ReadPin(GPIO_BTN_PORT,GPIO_BOTON_AZUL) == GPIO_PIN_RESET){
						
						if(count_ms_btn_azul > PULSO_LARGO_MS){
							
							gb_btn_azul_corto = false;
							gb_btn_azul_larga = true;						
						
						}
						else{
						
							gb_btn_azul_corto = true;
							gb_btn_azul_larga = false;						
						
						}
						estado_btn_azul = BTN_IDLE;
						
				  }
			  }
								
			break;				
		} 
	}
}
/*

Annadir en el stm32f4xx_it.c

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
    if(GPIO_Pin == GPIO_BOTON_AZUL){
        boton_azul_callback();
    }
}
*/
