#ifndef IC_TIM4_H
#define IC_TIM4_H


#include "RTE_Components.h"
#include "stm32f4xx_hal.h"
#include "stdbool.h"


//----------------------------

extern uint32_t diff_capture;
extern bool volatile capture_ready;

extern double periodo;

void ic_tim4_init(void);

void input_capture_tim4_start(void);
void input_capture_tim4_stop(void);
void TIM4_IRQHandler(void);
void ic_tim4_callback(void);



#endif //IC_TIM4_H
