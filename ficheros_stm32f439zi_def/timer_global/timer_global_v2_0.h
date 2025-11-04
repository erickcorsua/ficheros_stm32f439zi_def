#ifndef TIMER_GLOBAL_V2_0_H
#define TIMER_GLOBAL_V2_0_H

#include "stdbool.h"
#include "stm32f4xx_hal.h"
#include "RTE_Components.h"  

//variables

extern volatile bool gb_timer_global_1ms;
extern volatile bool gb_timer_global_10ms;
extern volatile bool gb_timer_global_100ms;
extern volatile bool gb_timer_global_1s;

/*
declaradas asi para que el debugger lo pueda leer

*/
extern volatile uint32_t sysclk, pclk1,pclk2;
extern volatile uint32_t PSC,Period;
extern volatile  uint32_t ahb_div,apb1_div,apb2_div;

//prototype funtions
void timer_global_callback(void);


void Timer_Config_ms(TIM_TypeDef *instance);


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



#endif //TIMER_GLOBAL_V2_0_H
