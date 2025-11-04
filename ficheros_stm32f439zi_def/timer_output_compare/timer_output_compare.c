// timer configurado para output capture (stm32 f4 39zi)

//timer TIM2 de 32 bit up/down general purpose con el pin PB11

#include "timer_output_compare.h"
#include "RTE_Components.h"

#include "stm32f4xx_hal.h"

//macros

#define PERIODO_1s 9999
#define PERIODO_2s 19999
#define PERIODO_5s 49999

//variable

static TIM_HandleTypeDef htim2;  
static TIM_OC_InitTypeDef TIM2_channel_InitStruct;

static GPIO_InitTypeDef GPIO_InitStruct;

//funtions prototype

void timer_output_compare_init(void);

void timer_output_compare_start(void);
void timer_output_compare_stop(void);

void timer_output_compare_incrementar(void);
void timer_output_compare_decrementar(void);

//funtions 

void timer_output_compare_init(void){

	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM2; 
	
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	
	htim2.Instance = TIM2;
	htim2.Init.Period = PERIODO_1s;
	htim2.Init.Prescaler = (8400 - 1);
	
	HAL_TIM_OC_Init(&htim2);
	
	TIM2_channel_InitStruct.OCMode = TIM_OCMODE_TOGGLE;
	TIM2_channel_InitStruct.OCNPolarity = TIM_OCPOLARITY_HIGH;
	TIM2_channel_InitStruct.OCFastMode = TIM_OCFAST_DISABLE;
	
	HAL_TIM_OC_ConfigChannel(&htim2, &TIM2_channel_InitStruct, TIM_CHANNEL_4);
	
	
	
}
//----------------------------
void timer_output_compare_start(void){

	HAL_TIM_OC_Start(&htim2, TIM_CHANNEL_4);

}
//----------------------------
void timer_output_compare_stop(void){

	HAL_TIM_OC_Stop(&htim2, TIM_CHANNEL_4);

}
//----------------------------
void timer_output_compare_incrementar(void){

 if(htim2.Init.Period != PERIODO_5s){
	 
	 HAL_TIM_OC_Stop(&htim2, TIM_CHANNEL_4);
	 
	 if(htim2.Init.Period == PERIODO_1s){
		 
		 htim2.Init.Period = PERIODO_2s;
		 HAL_TIM_OC_Init(&htim2);
		 
		 
	 }
	 else{
		 htim2.Init.Period = PERIODO_5s;
		 HAL_TIM_OC_Init(&htim2);
		 
		
	 }	 
 }
}
//---------------------------- 
void timer_output_compare_decrementar(void){

 if(htim2.Init.Period != PERIODO_1s){
	 
	 HAL_TIM_OC_Stop(&htim2, TIM_CHANNEL_4);
	 
	 if(htim2.Init.Period == PERIODO_5s){
		 
		 htim2.Init.Period = PERIODO_2s;
		 HAL_TIM_OC_Init(&htim2);
		 
		 
	 }
	 else{
		 htim2.Init.Period = PERIODO_1s;
		 HAL_TIM_OC_Init(&htim2);
		 
		
	 }	 
 }
}
