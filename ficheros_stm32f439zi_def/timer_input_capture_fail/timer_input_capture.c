// timer configurado para input capture (stm32 f4 39zi)

//timer TIM5 de 32 bit up/down general purpose pin PA3

#include "timer_input_capture.h"
#include "RTE_Components.h"

#include "stm32f4xx_hal.h"

//macros

#define RESOLUCION_10us 839
#define RESOLUCION_1us 83

//variable
static TIM_HandleTypeDef htim5;  
static TIM_IC_InitTypeDef TIM5_channel_InitStruct;

static GPIO_InitTypeDef GPIO_InitStruct;

static uint32_t captura_anterior = 0;
static uint32_t frecuencia_hz = 0;

//funtions prototype

void timer_input_capture_init(void);

void timer_input_capture_start(void);
void timer_input_capture_stop(void);

void timer_input_capture_irq_handler(void); //handler
void timer_input_capture_callback(void);

uint32_t timer_input_capture_get_frecuencia(void);

//funtions 

void timer_input_capture_init(void){

	__HAL_RCC_TIM5_CLK_ENABLE();
	HAL_NVIC_EnableIRQ(TIM5_IRQn);
	
  __HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Pin = GPIO_PIN_3;  //pin que uso PA3 chanel 4
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM5;
	
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	htim5.Instance = TIM5;
	htim5.Init.Period = 0xFFFFFFFF; // como vamos a usarlo para medir flancos le ponemos un valor alto, para que no haya overflow entre flancos
	htim5.Init.Prescaler = RESOLUCION_10us; // seria 84 MHz, esto tiene una teoria por detras, que la explico en escrito porque es bastante
	htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
	
	HAL_TIM_IC_Init(&htim5);

  TIM5_channel_InitStruct.ICFilter = 0;  
  TIM5_channel_InitStruct.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  TIM5_channel_InitStruct.ICPrescaler = TIM_ICPSC_DIV1;
  TIM5_channel_InitStruct.ICSelection = TIM_ICSELECTION_DIRECTTI;
	
	HAL_TIM_IC_ConfigChannel(&htim5,&TIM5_channel_InitStruct,TIM_CHANNEL_4);
	
}
//---------------------------------
void timer_input_capture_start(void){

 HAL_TIM_IC_Start_IT(&htim5,TIM_CHANNEL_4);
	
}
//----------------------------------

void timer_input_capture_stop(void){

	HAL_TIM_IC_Stop_IT(&htim5,TIM_CHANNEL_4);

}


//----------------------------------

void timer_input_capture_irq_handler(void){

  HAL_TIM_IRQHandler(&htim5);

}  // función de tu módulo

void timer_input_capture_callback(void){
 
	uint32_t captura_actual = HAL_TIM_ReadCapturedValue(&htim5, TIM_CHANNEL_4);
	uint32_t diferencia;

	if(captura_actual >= captura_anterior){
	
		diferencia = captura_actual - captura_anterior;
	 
	}
	else{
	
		diferencia = (0xFFFFFFFF - captura_anterior) + captura_actual + 1;
	
	}
	
	if( diferencia > 0){
	
	  frecuencia_hz = 100000 / diferencia;
	
	}
	else{
	
		 frecuencia_hz = 0;
	
	}
	
	captura_anterior = captura_actual;
	
}

//----------------------------------

uint32_t timer_input_capture_get_frecuencia(void){
    
	return frecuencia_hz;

}

