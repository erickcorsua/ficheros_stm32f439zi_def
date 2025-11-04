// intro
// titulo: ic_tim4
// autor: erick cs
// descripcion: configura el timer 4 junto con el pd15 para que funcione en modo input capture

#include "ic_tim4.h"
#include "stm32f4xx_hal.h"
#include "stdbool.h"

// -------- macros --------

#define PIN GPIO_PIN_15
#define PORT GPIOD
#define CHANNEL TIM_CHANNEL_4

//--------variables-----------

static GPIO_InitTypeDef GPIO_InitStruct;
static TIM_IC_InitTypeDef sConfigIC;
static TIM_HandleTypeDef htim4;

static uint32_t capture1 = 0;
static uint32_t capture2 = 0;

uint32_t diff_capture = 0;
double periodo = 0;


static bool first_capture = false;
bool volatile capture_ready = false;

//-----funtions prototype-----

static void gpiod_15_init(void);
static void channel_config_init(void);

void ic_tim4_init(void);

void input_capture_tim4_start(void);
void input_capture_tim4_stop(void);
void TIM4_IRQHandler(void);
void ic_tim4_callback(void);

// ------------------------
// ---------------- funciones ----------------

void ic_tim4_init(){

  gpiod_15_init();
  
  __HAL_RCC_TIM4_CLK_ENABLE();
	HAL_NVIC_EnableIRQ(TIM4_IRQn);
  
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 0;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 0xFFFF; // para tener mas resolucion 
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  
	HAL_TIM_IC_Init(&htim4);
	
  channel_config_init();
  HAL_TIM_IC_Start_IT(&htim4, CHANNEL);
}

// ------------------------
static void gpiod_15_init(void){

  __HAL_RCC_GPIOD_CLK_ENABLE();

  GPIO_InitStruct.Pin = PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
	
  HAL_GPIO_Init(PORT, &GPIO_InitStruct);

}
// ------------------------
static void channel_config_init(void){

	sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  HAL_TIM_IC_ConfigChannel(&htim4, &sConfigIC, CHANNEL);

}
// ------------------------
void input_capture_tim4_start(void) {
  HAL_TIM_IC_Start_IT(&htim4, CHANNEL);
}
// ------------------------
void input_capture_tim4_stop(void) {
  HAL_TIM_IC_Stop_IT(&htim4, CHANNEL);
}
// ------------------------
void TIM4_IRQHandler(void){
	
  HAL_TIM_IRQHandler(&htim4);
	
}
// ------------------------
void ic_tim4_callback(void){
    
    if(!first_capture){
      capture1 = HAL_TIM_ReadCapturedValue(&htim4, CHANNEL);
      first_capture = true;
    } 
		else{
      capture2 = HAL_TIM_ReadCapturedValue(&htim4, CHANNEL);

			first_capture = false;
			
      if(capture2 >= capture1){
        diff_capture = capture2 - capture1;
			}
      else{
        diff_capture = (0xFFFF - capture1) + capture2;
			}
			
			periodo = (diff_capture*2*3.570552147)/49664;
			
      capture_ready = true;
    }
    
}

/*
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
    
    ic_tim4_callback();
  
  }
  */

