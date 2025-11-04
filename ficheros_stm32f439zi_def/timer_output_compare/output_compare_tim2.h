#ifndef OUTPUT_COMPARE_TIM2_H
#define OUTPUT_COMPARE_TIM2_H

#include "stdbool.h"
#include "stm32f4xx_hal.h"
#include "RTE_Components.h"  


extern volatile bool gb_output_compare_tim2;

void output_compare_tim2_init(uint32_t periodo_ms);
void output_compare_tim2_init_us(uint32_t periodo_us);

void output_compare_tim2_start(void);
void output_compare_tim2_stop(void);

void output_compare_tim2_reconfig(uint32_t nuevo_periodo_ms);
void output_compare_tim2_reconfig_us(uint32_t nuevo_periodo_us);


#endif //OUTPUT_COMPARE_TIM2_H
