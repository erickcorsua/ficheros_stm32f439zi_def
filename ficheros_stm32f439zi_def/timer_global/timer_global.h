#ifndef TIMER_GLOBAL_H
#define TIMER_GLOBAL_H

#include "stdbool.h"
#include "stm32f4xx_hal.h"
#include "RTE_Components.h"  

#define PRESCALER_PARA_1MS 8399
#define PERIODO_PARA_1MS 9
#define TIMER TIM6


//variables

extern volatile bool gb_timer_global_1ms;
extern volatile bool gb_timer_global_10ms;
extern volatile bool gb_timer_global_100ms;
extern volatile bool gb_timer_global_1s;



//prototype funtions
void timer_global_init(void);

void TIM6_DAC_IRQHandler(void);
void timer_global_callback(void);

#endif //TIMER_GLOBAL_H
