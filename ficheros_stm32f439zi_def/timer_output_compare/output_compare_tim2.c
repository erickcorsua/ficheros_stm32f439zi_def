//intro 
//titulo: output_compare_tim2
//autor: erick cs
//descripcion: fichero que configura el timer 2 en modo output compare, para una señal cuadrada en 
//             el pin PB11(modo alternativo)  el tim 2 es apb1


//librerias

#include "output_compare_tim2.h"

#include "stm32f4xx_hal.h"
#include "RTE_Components.h"           
#include "stdbool.h"

//macros

#define PIN GPIO_PIN_11
#define PORT GPIOB


//variables
static TIM_HandleTypeDef htim2; 
static GPIO_InitTypeDef GPIO_InitStruct_pb11;
static TIM_OC_InitTypeDef sConfigOC;

volatile bool gb_output_compare_tim2 = false;

//prototype funtions

static void gpio_pb11_init(void);
static void tim2_channel_config(void);

void output_compare_tim2_init(uint32_t periodo_ms);
void output_compare_tim2_init_us(uint32_t periodo_us);

static uint32_t decode_AHB_div(uint32_t divider);
static uint32_t decode_APB_div(uint32_t divider);

void output_compare_tim2_start(void);
void output_compare_tim2_stop(void);
void output_compare_tim2_reconfig(uint32_t nuevo_periodo_ms);
void output_compare_tim2_reconfig_us(uint32_t nuevo_periodo_us);


//funtions



static uint32_t decode_APB_div(uint32_t divider)
{
    switch (divider) {
        case RCC_HCLK_DIV1:  return 1;
        case RCC_HCLK_DIV2:  return 2;
        case RCC_HCLK_DIV4:  return 4;
        case RCC_HCLK_DIV8:  return 8;
        case RCC_HCLK_DIV16: return 16;
        default: return 1;
    }
}

static uint32_t decode_AHB_div(uint32_t divider)
{
    switch (divider) {
        case RCC_SYSCLK_DIV1:   return 1;
        case RCC_SYSCLK_DIV2:   return 2;
        case RCC_SYSCLK_DIV4:   return 4;
        case RCC_SYSCLK_DIV8:   return 8;
        case RCC_SYSCLK_DIV16:  return 16;
        case RCC_SYSCLK_DIV64:  return 64;
        case RCC_SYSCLK_DIV128: return 128;
        case RCC_SYSCLK_DIV256: return 256;
        case RCC_SYSCLK_DIV512: return 512;
        default: return 1;
    }
}


static void gpio_pb11_init(void){

  __HAL_RCC_GPIOB_CLK_ENABLE();
 
  GPIO_InitStruct_pb11.Pin = PIN;
	GPIO_InitStruct_pb11.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct_pb11.Pull = GPIO_NOPULL;
	GPIO_InitStruct_pb11.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct_pb11.Alternate = GPIO_AF1_TIM2;

	HAL_GPIO_Init(PORT, &GPIO_InitStruct_pb11);

}

//-------------------------------

static void tim2_channel_config(void){
  
  sConfigOC.OCMode = TIM_OCMODE_TOGGLE;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	
  HAL_TIM_OC_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_4);
	
}
//-------------------------------
void output_compare_tim2_init(uint32_t periodo_ms){

  if(!gb_output_compare_tim2){
		gb_output_compare_tim2=true;
		
    __HAL_RCC_TIM2_CLK_ENABLE();
    gpio_pb11_init();

  	RCC_ClkInitTypeDef clkconfig;
    uint32_t latency;
		
	  uint32_t sysclk, pclk1;
    uint32_t PSC,Period;
    uint32_t ahb_div,apb1_div,apb2_div;
	
	  SystemCoreClockUpdate();
    HAL_RCC_GetClockConfig(&clkconfig, &latency);

    sysclk = HAL_RCC_GetSysClockFreq();

   	ahb_div  = decode_AHB_div(clkconfig.AHBCLKDivider);
	  apb1_div = decode_APB_div(clkconfig.APB1CLKDivider);
    apb2_div = decode_APB_div(clkconfig.APB2CLKDivider);
	
	  if(apb1_div == 1){
	    pclk1 = (sysclk / ahb_div) / apb1_div;
    } 
    else{
      pclk1 = ((sysclk / ahb_div) / apb1_div) * 2;
    }
	  // 3. Calcular ticks, PSC y Period
		
	  if(periodo_ms <= 6500){
			
	    PSC = (pclk1/10000)-1;
	    Period = (periodo_ms * 10)-1;
		
    }
	  else if(periodo_ms > 6500 && pclk1 <= 65000000){
			
	    PSC = (pclk1/1000)-1;
		  Period = (periodo_ms)-1;
		
    }
	  else if(periodo_ms > 6500 && pclk1 > 65000000){
		
	    PSC = (pclk1 / 2000)-1;
		  Period = (periodo_ms * 2)-1;
			
  }
	
    // 3. Configurar timer base
    htim2.Instance = TIM2;
    htim2.Init.Prescaler = PSC;                        
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = Period;                  
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    HAL_TIM_OC_Init(&htim2);

    tim2_channel_config();

	  __HAL_TIM_SET_COUNTER(&htim2, 0);
    HAL_TIM_OC_Start(&htim2, TIM_CHANNEL_4);
  }
}

//-------------------------------

void output_compare_tim2_init_us(uint32_t periodo_us){

  if(!gb_output_compare_tim2){
		gb_output_compare_tim2=true;
		
    __HAL_RCC_TIM2_CLK_ENABLE();
    gpio_pb11_init();

  	RCC_ClkInitTypeDef clkconfig;
    uint32_t latency;
		
	  uint32_t sysclk, pclk1;
    uint32_t PSC,Period;
    uint32_t ahb_div,apb1_div,apb2_div;
	
	  SystemCoreClockUpdate();
    HAL_RCC_GetClockConfig(&clkconfig, &latency);

    sysclk = HAL_RCC_GetSysClockFreq();

   	ahb_div  = decode_AHB_div(clkconfig.AHBCLKDivider);
	  apb1_div = decode_APB_div(clkconfig.APB1CLKDivider);
    apb2_div = decode_APB_div(clkconfig.APB2CLKDivider);
	
	  if(apb1_div == 1){
	    pclk1 = (sysclk / ahb_div) / apb1_div;
    } 
    else{
      pclk1 = ((sysclk / ahb_div) / apb1_div) * 2;
    }
	  // 3. Calcular ticks, PSC y Period
		
	  PSC = pclk1 / 1000000 - 1;
    uint32_t psc_real = PSC + 1;

   // cálculo real de frecuencia del contador
    uint32_t f_cnt = pclk1 / psc_real;

    // y ajuste del periodo en base a esa frecuencia
    Period = (f_cnt * periodo_us / 1000000) - 1;
		
		
    // 3. Configurar timer base
    htim2.Instance = TIM2;
    htim2.Init.Prescaler = PSC;                        
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = Period;                  
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    HAL_TIM_OC_Init(&htim2);

    tim2_channel_config();

	  __HAL_TIM_SET_COUNTER(&htim2, 0);
    HAL_TIM_OC_Start(&htim2, TIM_CHANNEL_4);
  }
}

//-------------------------------
void output_compare_tim2_start(void){

	
	__HAL_TIM_SET_COUNTER(&htim2, 0);
  HAL_TIM_OC_Start(&htim2, TIM_CHANNEL_4);


}
void output_compare_tim2_stop(void){

	HAL_TIM_OC_Stop(&htim2, TIM_CHANNEL_4);

}
void output_compare_tim2_reconfig(uint32_t nuevo_periodo_ms){

	HAL_TIM_OC_Stop(&htim2, TIM_CHANNEL_4);
	__HAL_TIM_DISABLE(&htim2);

	
	RCC_ClkInitTypeDef clkconfig;
  uint32_t latency;
		
	uint32_t sysclk, pclk1;
  uint32_t PSC,Period;
  uint32_t ahb_div,apb1_div,apb2_div;
	
	SystemCoreClockUpdate();
  HAL_RCC_GetClockConfig(&clkconfig, &latency);

  sysclk = HAL_RCC_GetSysClockFreq();

	ahb_div  = decode_AHB_div(clkconfig.AHBCLKDivider);
	apb1_div = decode_APB_div(clkconfig.APB1CLKDivider);
  apb2_div = decode_APB_div(clkconfig.APB2CLKDivider);
	
	if(apb1_div == 1){
	  pclk1 = (sysclk / ahb_div) / apb1_div;
  } 
  else{
    pclk1 = ((sysclk / ahb_div) / apb1_div) * 2;
  }
	// 3. Calcular ticks, PSC y Period
		
	if(nuevo_periodo_ms <= 6500){
			
	  PSC = (pclk1/10000)-1;
	  Period = (nuevo_periodo_ms * 10)-1;
		
  }
	if(nuevo_periodo_ms > 6500 && pclk1 <= 65000000){
			
	  PSC = (pclk1/1000)-1;
		Period = (nuevo_periodo_ms)-1;
		
  }
	if(nuevo_periodo_ms > 6500 && pclk1 > 65000000){
		
	  PSC = (pclk1 / 2000)-1;
		Period = (nuevo_periodo_ms* 2)-1;
			
  }
	
  // 3. Configurar timer base
  
  htim2.Init.Prescaler = PSC;                        
  htim2.Init.Period = Period;                  
  
  HAL_TIM_OC_Init(&htim2);

  tim2_channel_config();

	__HAL_TIM_SET_COUNTER(&htim2, 0);
  HAL_TIM_OC_Start(&htim2, TIM_CHANNEL_4);
	
	__HAL_TIM_ENABLE(&htim2);

}
//-------------------------------
void output_compare_tim2_reconfig_us(uint32_t nuevo_periodo_us){
		
	HAL_TIM_OC_Stop(&htim2, TIM_CHANNEL_4);
	__HAL_TIM_DISABLE(&htim2);

	
	RCC_ClkInitTypeDef clkconfig;
  uint32_t latency;
		
	uint32_t sysclk, pclk1;
  uint32_t PSC,Period;
  uint32_t ahb_div,apb1_div,apb2_div;
	
	SystemCoreClockUpdate();
  HAL_RCC_GetClockConfig(&clkconfig, &latency);

  sysclk = HAL_RCC_GetSysClockFreq();

	ahb_div  = decode_AHB_div(clkconfig.AHBCLKDivider);
	apb1_div = decode_APB_div(clkconfig.APB1CLKDivider);
  apb2_div = decode_APB_div(clkconfig.APB2CLKDivider);
	
	if(apb1_div == 1){
	  pclk1 = (sysclk / ahb_div) / apb1_div;
  } 
  else{
    pclk1 = ((sysclk / ahb_div) / apb1_div) * 2;
  }
	// 3. Calcular ticks, PSC y Period
	
	// 3. Calcular ticks, PSC y Period
		
	  PSC = pclk1 / 1000000 - 1;
    uint32_t psc_real = PSC + 1;

   // cálculo real de frecuencia del contador
    uint32_t f_cnt = pclk1 / psc_real;

    // y ajuste del periodo en base a esa frecuencia
    Period = (f_cnt * nuevo_periodo_us / 1000000) - 1;
		
  // 3. Configurar timer base
  
  htim2.Init.Prescaler = PSC;                        
  htim2.Init.Period = Period;                  
  
  HAL_TIM_OC_Init(&htim2);

  tim2_channel_config();

	__HAL_TIM_SET_COUNTER(&htim2, 0);
  HAL_TIM_OC_Start(&htim2, TIM_CHANNEL_4);
	
	__HAL_TIM_ENABLE(&htim2);
}

