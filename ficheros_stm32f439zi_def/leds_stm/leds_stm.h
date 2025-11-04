#ifndef LEDS_STM_H
#define LEDS_STM_H

#include "leds_stm.h"
#include "RTE_Components.h"
#include "stm32f4xx_hal.h"
#include "stdbool.h"

//--------variables-----------



//----------------------------
//-----funtions prototype-----

void leds_stm_init(void);

void encender_all_leds(void);
void apagar_all_leds(void);

void encender_green(void);
void encender_blue(void);
void encender_red(void);

void apagar_green(void);
void apagar_blue(void);	
void apagar_red(void);

void alternar_green(void);
void alternar_blue(void);	
void alternar_red(void);

//----------------------------


#endif //LEDS_STM.H
