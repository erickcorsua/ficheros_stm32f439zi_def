//intro 
//titulo: timer_global_3_0v
//autor: erick cs
//descripcion: fichero capaz de configurar un timer cualquiera, e independiente del sysclk que genera
//             un pulso de 1ms, que se utiliza para levantar flags de 1ms, 10ms, 100ms y de 1s, ademas puede
//             configurar cualquier timer con una frecuencia deseada siempre y cuadno no sea excesiva como
//             mas de 34 segundos de periodo

//librerias

#include "timer_global_v3_0.h"

#include "stm32f4xx_hal.h"
#include "RTE_Components.h"           
#include "stdbool.h"

//macros

//variables

TimerGlobal_t timer1, timer2, timer3, timer4, timer5;
TimerGlobal_t timer6, timer7, timer8, timer9, timer10;
TimerGlobal_t timer11, timer12, timer13, timer14;

volatile uint8_t counter_10ms = 0;
volatile uint8_t counter_100ms = 0;
volatile uint16_t counter_1s = 0;

volatile bool gb_timer_global_1ms = false;
volatile bool gb_timer_global_10ms = false;
volatile bool gb_timer_global_100ms = false;
volatile bool gb_timer_global_1s = false;

static bool b_timer_global_init = false;

//prototype funtions

static void Timer_EnableClock(TIM_TypeDef *instance);
static void Timer_EnableIRQ(TIM_TypeDef *instance);

static uint32_t decode_AHB_div(uint32_t divider);
static uint32_t decode_APB_div(uint32_t divider);

void timer_global_init(TimerGlobal_t *t, TIM_TypeDef *instance);

void timer_global_callback(void);

void Timer_Config_ms_nuevo(TimerGlobal_t *t, TIM_TypeDef *instance, uint32_t periodo_ms);

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

//-------------------TIM_IRQ_Handlers---------------------------

void TIM1_CC_IRQHandler(void);
//----------------tim 1 tim 9------------------------
void TIM1_BRK_TIM9_IRQHandler(void);
//----------------tim 1 tim 10------------------------
void TIM1_UP_TIM10_IRQHandler(void);
//----------------tim 1 tim 11------------------------
void TIM1_TRG_COM_TIM11_IRQHandler(void);
//----------------tim 2------------------------
void TIM2_IRQHandler(void);
//----------------tim 3------------------------
void TIM3_IRQHandler(void);
//----------------tim 4------------------------
void TIM4_IRQHandler(void);
//----------------tim 5------------------------
void TIM5_IRQHandler(void);
//----------------tim 6------------------------
void TIM6_DAC_IRQHandler(void);
//----------------tim 7------------------------
void TIM7_IRQHandler(void);
//----------------tim 8------------------------
void  TIM8_CC_IRQHandler(void);
//----------------tim 8 tim 12------------------------
void TIM8_BRK_TIM12_IRQHandler(void);
//----------------tim 8 tim 13------------------------
void  TIM8_UP_TIM13_IRQHandler(void);
//----------------tim 8 tim 14------------------------
void TIM8_TRG_COM_TIM14_IRQHandler(void);





//funtions

//---------------------------------bien--------------------------------------------
// Función auxiliar para habilitar RCC
static void Timer_EnableClock(TIM_TypeDef *instance){
    if      (instance == TIM1)  __HAL_RCC_TIM1_CLK_ENABLE();
    else if (instance == TIM2)  __HAL_RCC_TIM2_CLK_ENABLE();
    else if (instance == TIM3)  __HAL_RCC_TIM3_CLK_ENABLE();
    else if (instance == TIM4)  __HAL_RCC_TIM4_CLK_ENABLE();
    else if (instance == TIM5)  __HAL_RCC_TIM5_CLK_ENABLE();
    else if (instance == TIM6)  __HAL_RCC_TIM6_CLK_ENABLE();
    else if (instance == TIM7)  __HAL_RCC_TIM7_CLK_ENABLE();
    else if (instance == TIM8)  __HAL_RCC_TIM8_CLK_ENABLE();
    else if (instance == TIM9)  __HAL_RCC_TIM9_CLK_ENABLE();
    else if (instance == TIM10) __HAL_RCC_TIM10_CLK_ENABLE();
    else if (instance == TIM11) __HAL_RCC_TIM11_CLK_ENABLE();
    else if (instance == TIM12) __HAL_RCC_TIM12_CLK_ENABLE();
    else if (instance == TIM13) __HAL_RCC_TIM13_CLK_ENABLE();
    else if (instance == TIM14) __HAL_RCC_TIM14_CLK_ENABLE();
}

// Función auxiliar para habilitar NVIC
static void Timer_EnableIRQ(TIM_TypeDef *instance){
    if      (instance == TIM1)  { HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 5, 0); HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn); }
    else if (instance == TIM2)  { HAL_NVIC_SetPriority(TIM2_IRQn, 5, 0); HAL_NVIC_EnableIRQ(TIM2_IRQn); }
    else if (instance == TIM3)  { HAL_NVIC_SetPriority(TIM3_IRQn, 5, 0); HAL_NVIC_EnableIRQ(TIM3_IRQn); }
    else if (instance == TIM4)  { HAL_NVIC_SetPriority(TIM4_IRQn, 5, 0); HAL_NVIC_EnableIRQ(TIM4_IRQn); }
    else if (instance == TIM5)  { HAL_NVIC_SetPriority(TIM5_IRQn, 5, 0); HAL_NVIC_EnableIRQ(TIM5_IRQn); }
    else if (instance == TIM6)  { HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 5, 0); HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn); }
    else if (instance == TIM7)  { HAL_NVIC_SetPriority(TIM7_IRQn, 5, 0); HAL_NVIC_EnableIRQ(TIM7_IRQn); }
    else if (instance == TIM8)  { HAL_NVIC_SetPriority(TIM8_UP_TIM13_IRQn, 5, 0); HAL_NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn); }
    else if (instance == TIM9)  { HAL_NVIC_SetPriority(TIM1_BRK_TIM9_IRQn, 5, 0); HAL_NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn); }
    else if (instance == TIM10) { HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 5, 0); HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn); }
    else if (instance == TIM11) { HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn, 5, 0); HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn); }
    else if (instance == TIM12) { HAL_NVIC_SetPriority(TIM8_BRK_TIM12_IRQn, 5, 0); HAL_NVIC_EnableIRQ(TIM8_BRK_TIM12_IRQn); }
    else if (instance == TIM13) { HAL_NVIC_SetPriority(TIM8_UP_TIM13_IRQn, 5, 0); HAL_NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn); }
    else if (instance == TIM14) { HAL_NVIC_SetPriority(TIM8_TRG_COM_TIM14_IRQn, 5, 0); HAL_NVIC_EnableIRQ(TIM8_TRG_COM_TIM14_IRQn); }
}

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

//---------------------------------bien--------------------------------------------


// Función principal
void timer_global_init(TimerGlobal_t *t, TIM_TypeDef *instance){

  if(!b_timer_global_init){
		
    t->instance   = instance;
    t->periodo_ms = 1;
    t->enabled    = true;
    t->flag       = false;

   RCC_ClkInitTypeDef clkconfig;
   uint32_t latency;
		
	  uint32_t sysclk, pclk1,pclk2;
    uint32_t PSC,Period;
    uint32_t ahb_div,apb1_div,apb2_div;

      // 1. Habilitar reloj
      Timer_EnableClock(instance);
		  Timer_EnableIRQ(instance);

      // 2. Calcular frecuencia base del timer
      SystemCoreClockUpdate();
      HAL_RCC_GetClockConfig(&clkconfig, &latency);

      sysclk = HAL_RCC_GetSysClockFreq();

      ahb_div  = decode_AHB_div(clkconfig.AHBCLKDivider);
      apb1_div = decode_APB_div(clkconfig.APB1CLKDivider);
      apb2_div = decode_APB_div(clkconfig.APB2CLKDivider);

      //==================APB2======================
      if(instance == TIM1 || instance == TIM8 || instance == TIM9 ||
        instance == TIM10 || instance == TIM11){

        if(apb2_div == 1){
          pclk2 = (sysclk / ahb_div) / apb2_div;
        }
        else{
          pclk2 = ((sysclk / ahb_div) / apb2_div) * 2;
        }
        if(pclk2 < 1000000){
          // Caso extremo: usar directamente pclk como frecuencia base
          PSC = 0;
          Period = (pclk2 / 1000) - 1;   // Divide directamente para 1ms
        }
        else{
          PSC = (pclk2 / 1000000) - 1; // 1 tick = 1 us
          Period = 1000 - 1;
        }
      }
        
      //==================APB1======================
      else{
        
        if(apb1_div == 1){
          pclk1 = (sysclk / ahb_div) / apb1_div;
        } 
        else{
          pclk1 = ((sysclk / ahb_div) / apb1_div) * 2;
        }
        // 3. Calcular ticks, PSC y Period
        
        if(pclk1 < 1000000){
        // Caso extremo: usar directamente pclk como frecuencia base
          PSC = 0;
          Period = (pclk1 / 1000) - 1;   // Divide directamente para 1ms
        }
        else{
          PSC = (pclk1 / 1000000) - 1; // 1 tick = 1 us
          Period = 1000 - 1;
        }
    }

    // 4. Inicializar TIM
     t->htim.Instance = instance;
    t->htim.Init.Prescaler = PSC;
    t->htim.Init.Period = Period;
    t->htim.Init.CounterMode = TIM_COUNTERMODE_UP;
    t->htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    t->htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
		
    HAL_TIM_Base_Init(&t->htim);
    HAL_TIM_Base_Start_IT(&t->htim);
    
    b_timer_global_init = true;

  }
}



void Timer_Config_ms_nuevo(TimerGlobal_t *t, TIM_TypeDef *instance, uint32_t periodo_ms){
	
    t->instance   = instance;
    t->periodo_ms = periodo_ms;
    t->enabled    = true;
    t->flag       = false;


	RCC_ClkInitTypeDef clkconfig;
  uint32_t latency;
	
  uint32_t sysclk, pclk1,pclk2;
  uint32_t PSC,Period;
  uint32_t ahb_div,apb1_div,apb2_div;	

  // 1. Habilitar reloj
  Timer_EnableClock(instance);
  Timer_EnableIRQ(instance);
	
  // 2. Calcular frecuencia base del timer
	
	SystemCoreClockUpdate();
	HAL_RCC_GetClockConfig(&clkconfig, &latency);

  sysclk = HAL_RCC_GetSysClockFreq();

  ahb_div  = decode_AHB_div(clkconfig.AHBCLKDivider);
  apb1_div = decode_APB_div(clkconfig.APB1CLKDivider);
  apb2_div = decode_APB_div(clkconfig.APB2CLKDivider);

	//==================APB2======================
  if(instance == TIM1 || instance == TIM8 || instance == TIM9 ||
     instance == TIM10 || instance == TIM11){

    if(apb2_div == 1){
      pclk2 = (sysclk / ahb_div) / apb2_div;
    }
		else{
      pclk2 = ((sysclk / ahb_div) / apb2_div) * 2;
    }
		 // 3. Calcular ticks, PSC y Period
		
		if(periodo_ms <= 6500){
			
			PSC = (pclk2/10000)-1;
			Period = (periodo_ms * 10)-1;
		
		}
		if(periodo_ms > 6500 && pclk2 <= 65000000){
			
			PSC = (pclk2/1000)-1;
			Period = (periodo_ms)-1;
		
		}
		if(periodo_ms > 6500 && pclk2 > 65000000){
		
			PSC = (pclk2 / 2000)-1;
			Period = (periodo_ms * 2)-1;
			
		}
  }
		 
	//==================APB1======================
	else{
		
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
		if(periodo_ms > 6500 && pclk1 <= 65000000){
			
			PSC = (pclk1/1000)-1;
			Period = (periodo_ms)-1;
		
		}
		if(periodo_ms > 6500 && pclk1 > 65000000){
		
			PSC = (pclk1 / 2000)-1;
			Period = (periodo_ms * 2)-1;
			
		}
    
 }

    t->htim.Instance = instance;
    t->htim.Init.Prescaler = PSC;
    t->htim.Init.Period = Period;
    t->htim.Init.CounterMode = TIM_COUNTERMODE_UP;
    t->htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    t->htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    HAL_TIM_Base_Init(&t->htim);
    HAL_TIM_Base_Start_IT(&t->htim);

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	
	  timer_global_callback();
	
    if (htim->Instance == TIM1)   timer1.flag = true;
    else if (htim->Instance == TIM2)  timer2.flag = true;
    else if (htim->Instance == TIM3)  timer3.flag = true;
    else if (htim->Instance == TIM4)  timer4.flag = true;
    else if (htim->Instance == TIM5)  timer5.flag = true;
    else if (htim->Instance == TIM6)  timer6.flag = true;
    else if (htim->Instance == TIM7)  timer7.flag = true;
    else if (htim->Instance == TIM8)  timer8.flag = true;
    else if (htim->Instance == TIM9)  timer9.flag = true;
    else if (htim->Instance == TIM10) timer10.flag = true;
    else if (htim->Instance == TIM11) timer11.flag = true;
    else if (htim->Instance == TIM12) timer12.flag = true;
    else if (htim->Instance == TIM13) timer13.flag = true;
    else if (htim->Instance == TIM14) timer14.flag = true;
}

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

/****************************************************

Irq handlers de todos los timers del stm32f429/39Zi

***************************************************/

//----------------tim 1 ------------------------
void TIM1_CC_IRQHandler(void){
	
  HAL_TIM_IRQHandler(&timer1.htim);  // Esto llama al callback del HAL
	
}
//----------------tim 1 tim 9------------------------
void TIM1_BRK_TIM9_IRQHandler(void){
	
  HAL_TIM_IRQHandler(&timer1.htim);  // Esto llama al callback del HAL
	HAL_TIM_IRQHandler(&timer9.htim);
	
}
//----------------tim 1 tim 10------------------------
void TIM1_UP_TIM10_IRQHandler(void){
	
  HAL_TIM_IRQHandler(&timer1.htim);  // Esto llama al callback del HAL
	HAL_TIM_IRQHandler(&timer10.htim);
	
}
//----------------tim 1 tim 11------------------------
void TIM1_TRG_COM_TIM11_IRQHandler(void){
	
	HAL_TIM_IRQHandler(&timer1.htim);
  HAL_TIM_IRQHandler(&timer11.htim);  // Esto llama al callback del HAL
	
}
//----------------tim 2------------------------
void TIM2_IRQHandler(void){
	
  HAL_TIM_IRQHandler(&timer2.htim);  // Esto llama al callback del HAL
	
}
//----------------tim 3------------------------
void TIM3_IRQHandler(void){
	
  HAL_TIM_IRQHandler(&timer3.htim);  // Esto llama al callback del HAL
	
}
//----------------tim 4------------------------
void TIM4_IRQHandler(void){
	
  HAL_TIM_IRQHandler(&timer4.htim);  // Esto llama al callback del HAL
	
}
//----------------tim 5------------------------
void TIM5_IRQHandler(void){
	
  HAL_TIM_IRQHandler(&timer5.htim);  // Esto llama al callback del HAL
	
}
//----------------tim 6------------------------
void TIM6_DAC_IRQHandler(void){
	
  HAL_TIM_IRQHandler(&timer6.htim);  // Esto llama al callback del HAL
	
}
//----------------tim 7------------------------
void TIM7_IRQHandler(void){
	
  HAL_TIM_IRQHandler(&timer7.htim);  // Esto llama al callback del HAL
	
}
//----------------tim 8------------------------
void  TIM8_CC_IRQHandler(void){
	
  HAL_TIM_IRQHandler(&timer8.htim);  // Esto llama al callback del HAL
	
}
//----------------tim 8 tim 12------------------------
void TIM8_BRK_TIM12_IRQHandler(void){
	
  HAL_TIM_IRQHandler(&timer8.htim);  // Esto llama al callback del HAL
	HAL_TIM_IRQHandler(&timer12.htim);
}
//----------------tim 8 tim 13------------------------
void  TIM8_UP_TIM13_IRQHandler(void){
	
	HAL_TIM_IRQHandler(&timer8.htim);
  HAL_TIM_IRQHandler(&timer13.htim);  // Esto llama al callback del HAL
	
}
//----------------tim 8 tim 14------------------------
void TIM8_TRG_COM_TIM14_IRQHandler(void){
	
	HAL_TIM_IRQHandler(&timer8.htim);
  HAL_TIM_IRQHandler(&timer14.htim);  // Esto llama al callback del HAL
	
}
