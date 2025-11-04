#ifndef JOISTICK_H
#define JOISTICK_H

#include "RTE_Components.h"
#include "stm32f4xx_hal.h"
#include "stdbool.h"

void gpio_joistick_init(void);

void EXTI9_5_IRQHandler(void);
void EXTI3_IRQHandler(void);

extern volatile bool gb_flag_up; 
extern volatile bool gb_flag_down;


#endif //JOISTICK_H
