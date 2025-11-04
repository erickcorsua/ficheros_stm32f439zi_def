#ifndef BOTON_AZUL_H
#define BOTON_AZUL_H

#include "RTE_Components.h"
#include "stm32f4xx_hal.h"
#include "stdbool.h"

#define GPIO_BOTON_AZUL GPIO_PIN_13
#define GPIO_BTN_PORT GPIOC

extern volatile bool gb_btn_azul_corto;

void boton_azul_init(void);
void EXTI15_10_IRQHandler(void);
void boton_azul_callback(void);

//void boton_azul_irq_handler(void); --> si usas mas pines con numero
//                                       10 al 15, debes habilitar esto y tocar en el stm32f4xx_it.c

#endif //BOTON_AZUL.H
