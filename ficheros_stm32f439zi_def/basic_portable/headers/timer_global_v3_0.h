#ifndef TIMER_GLOBAL_V3_0_H
#define TIMER_GLOBAL_V3_0_H

#include "stdbool.h"
#include "stm32f4xx_hal.h"
#include "RTE_Components.h"  

//variables


extern volatile uint8_t counter_10ms;
extern volatile uint8_t counter_100ms;
extern volatile uint16_t counter_1s;

extern volatile bool gb_timer_global_1ms;
extern volatile bool gb_timer_global_10ms;
extern volatile bool gb_timer_global_100ms;
extern volatile bool gb_timer_global_1s;

// Definición tipo "clase"
typedef struct {
	
    TIM_HandleTypeDef htim;        // Handle HAL
    TIM_TypeDef *instance;         // TIMx
    bool enabled;                  // Estado
    volatile bool flag;            // Flag de interrupción
    uint32_t periodo_ms;           // Periodo configurado

} TimerGlobal_t;

// Timers globales
extern TimerGlobal_t timer1, timer2, timer3, timer4, timer5;
extern TimerGlobal_t timer6, timer7, timer8, timer9, timer10;
extern TimerGlobal_t timer11, timer12, timer13, timer14;

//prototype funtions
void timer_global_init(TimerGlobal_t *t, TIM_TypeDef *instance);

void timer_global_callback(void);

void Timer_Config_ms_nuevo(TimerGlobal_t *t, TIM_TypeDef *instance, uint32_t periodo_ms);

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);


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



#endif //TIMER_GLOBAL_V3_0_H
