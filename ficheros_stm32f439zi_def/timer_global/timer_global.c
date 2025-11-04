//intro 
//titulo: timer_global
//autor: erick cs
//descripcion: es aqui donde se configura un timer global (TIM6), para despues ser usado
//             en otro fichero de timers_virtual, el principal proposito es ahorrar
//             timers, y poder facilitar el uso de estos.

//nota: funciona con sysclk = 168Mhz M=4 P=2 N= 168

//librerias

#include "timer_global.h"

#include "stm32f4xx_hal.h"
#include "RTE_Components.h"           
#include "stdbool.h"

//macros

#define PRESCALER_PARA_1MS 8399 
#define PERIODO_PARA_1MS 9
#define TIMER TIM6

//variables

static TIM_HandleTypeDef htim;

volatile bool gb_timer_global_1ms = false;
volatile bool gb_timer_global_10ms = false;
volatile bool gb_timer_global_100ms = false;
volatile bool gb_timer_global_1s = false;

static bool b_timer_global_init = false;

static uint8_t counter_10ms = 0;
static uint8_t counter_100ms = 0;
static uint16_t counter_1s = 0;

//prototype funtions
void timer_global_init(void);

void TIM6_DAC_IRQHandler(void);
void timer_global_callback(void);

//funtions
//=============init===================
void timer_global_init(void){
	if(!b_timer_global_init){
		
		//habilitar reloj para el TIM6
		__HAL_RCC_TIM6_CLK_ENABLE();
		
		// Configurar interrupción TIM6
		HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
		
		counter_10ms = 0;
		counter_100ms = 0;
		counter_1s = 0;
		
		
		//configuracion
		
		htim.Instance = TIMER;
		
		htim.Init.Prescaler = PRESCALER_PARA_1MS;
		htim.Init.Period = PERIODO_PARA_1MS;
		
		htim.Init.CounterMode = TIM_COUNTERMODE_UP;
		htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

		HAL_TIM_Base_Init(&htim);
		
		HAL_TIM_Base_Start_IT(&htim);
		
		b_timer_global_init = true;
	}
		
}
//=========================================
void TIM6_DAC_IRQHandler(void){
	
  HAL_TIM_IRQHandler(&htim);  // Esto llama al callback del HAL
	
}
//=========================================

//=============callback===================
void timer_global_callback(void){
	if(b_timer_global_init){
		
		counter_10ms++;
		counter_100ms++;
		counter_1s++;
	
		gb_timer_global_1ms = true;
	
		if(counter_10ms == 10){
			counter_10ms = 0;
			gb_timer_global_10ms = true;
	
		}
		if(counter_100ms == 100){
			counter_100ms = 0;
			gb_timer_global_100ms = true;
	
		}
		if(counter_1s == 1000){
			counter_1s = 0;
			gb_timer_global_1s = true;
	
		}
	}
}


/*
============================LIBRERIA DE HANDLERS======================
 TIM1_BRK_TIM9_IRQHandler 
 TIM1_UP_TIM10_IRQHandler          
 TIM1_TRG_COM_TIM11_IRQHandler     
 TIM1_CC_IRQHandler                

 TIM2_IRQHandler                   

 TIM3_IRQHandler                   

 TIM4_IRQHandler                   

 TIM5_IRQHandler                                                                

 TIM6_DAC_IRQHandler        

 TIM7_IRQHandler                  

 TIM8_BRK_TIM12_IRQHandler
 TIM8_UP_TIM13_IRQHandler          
 TIM8_TRG_COM_TIM14_IRQHandler     
 TIM8_CC_IRQHandler                

 
 
=================================================================
-------------------tim 1--------------------------- 
void TIM1_CC_IRQHandler(void){
	
  HAL_TIM_IRQHandler(&htim);  // Esto llama al callback del HAL
	
}
----------------tim 1 tim 9------------------------
void TIM1_BRK_TIM9_IRQHandler(void){
	
  HAL_TIM_IRQHandler(&htim);  // Esto llama al callback del HAL
	
}
----------------tim 1 tim 10------------------------
void TIM1_UP_TIM10_IRQHandler(void){
	
  HAL_TIM_IRQHandler(&htim);  // Esto llama al callback del HAL
	
}
----------------tim 1 tim 11------------------------
void TIM1_TRG_COM_TIM11_IRQHandler(void){
	
  HAL_TIM_IRQHandler(&htim);  // Esto llama al callback del HAL
	
}
----------------tim 2------------------------
void TIM2_IRQHandler(void){
	
  HAL_TIM_IRQHandler(&htim);  // Esto llama al callback del HAL
	
}
----------------tim 3------------------------
void TIM3_IRQHandler(void){
	
  HAL_TIM_IRQHandler(&htim);  // Esto llama al callback del HAL
	
}
----------------tim 4------------------------
void TIM4_IRQHandler(void){
	
  HAL_TIM_IRQHandler(&htim);  // Esto llama al callback del HAL
	
}
----------------tim 5------------------------
void TIM5_IRQHandler(void){
	
  HAL_TIM_IRQHandler(&htim);  // Esto llama al callback del HAL
	
}
----------------tim 6------------------------
void TIM6_DAC_IRQHandler(void){
	
  HAL_TIM_IRQHandler(&htim);  // Esto llama al callback del HAL
	
}
----------------tim 7------------------------
void TIM7_IRQHandler(void){
	
  HAL_TIM_IRQHandler(&htim);  // Esto llama al callback del HAL
	
}
----------------tim 8------------------------
void  TIM8_CC_IRQHandler(void){
	
  HAL_TIM_IRQHandler(&htim);  // Esto llama al callback del HAL
	
}
----------------tim 8 tim 12------------------------
void TIM8_BRK_TIM12_IRQHandler(void){
	
  HAL_TIM_IRQHandler(&htim)(&htim);  // Esto llama al callback del HAL
	
}
----------------tim 8 tim 13------------------------
void  TIM8_UP_TIM13_IRQHandler(void){
	
  HAL_TIM_IRQHandler(&htim);  // Esto llama al callback del HAL
	
}
----------------tim 8 tim 14------------------------
void TIM8_TRG_COM_TIM14_IRQHandler(void){
	
  HAL_TIM_IRQHandler(&htim);  // Esto llama al callback del HAL
	
}

*/
/*
//----------------------Callback para timers por flanco --------------------

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	
    if (htim->Instance == TIM6) {
			
        tres_botones_callback_tim();
				
    }
}

*/

