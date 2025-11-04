#ifndef BOTON_AZUL_V2_0_H
#define BOTON_AZUL_V2_0_H

#include "RTE_Components.h"
#include "stm32f4xx_hal.h"
#include "stdbool.h"
#include "timer_virtual.h"

#define GPIO_BOTON_AZUL GPIO_PIN_13
#define GPIO_BTN_PORT GPIOC

extern volatile bool gb_btn_azul_rise_evnt;

extern volatile bool gb_btn_azul_corto;
extern volatile bool gb_btn_azul_larga;

extern TimerVirtual tim_btn_azul;

void boton_azul_init(void);
void boton_azul_callback(void);
void EXTI15_10_IRQHandler(void);

void boton_azul_fsm(void);


//void boton_azul_callback(void);

#endif //BOTON_AZULV2_0_.H
