//intro 
//titulo: leds_stm
//autor: erick cs
//descripcion: fichero para la gestion de los leds del stm, LD1(verde), LD2(azul) y LD3(rojo) (PB0, PB7, PB14)

//----------librerias---------

#include "leds_stm.h"
#include "RTE_Components.h"
#include "stm32f4xx_hal.h"
#include "stdbool.h"


//----------------------------

//-----------macros-----------

#define LED_GREEN_PIN GPIO_PIN_0
#define LED_BLUE_PIN GPIO_PIN_7
#define LED_RED_PIN GPIO_PIN_14

#define LEDS_PORT GPIOB

//----------------------------

//--------variables-----------

static GPIO_InitTypeDef GPIO_InitStruct;

static bool gb_leds_stm_init = false;

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
//----------funtions----------

void leds_stm_init(void){

	if(!gb_leds_stm_init){
		__HAL_RCC_GPIOB_CLK_ENABLE();
		
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

		GPIO_InitStruct.Pin = LED_GREEN_PIN | LED_BLUE_PIN | LED_RED_PIN;  // led 1 verde // led2 azul //led3 rojo

		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		
		
		gb_leds_stm_init = true;
	}
}
//----------------------------
void encender_all_leds(void){

	HAL_GPIO_WritePin(LEDS_PORT,LED_GREEN_PIN,GPIO_PIN_SET);
	HAL_GPIO_WritePin(LEDS_PORT,LED_BLUE_PIN,GPIO_PIN_SET);
	HAL_GPIO_WritePin(LEDS_PORT,LED_RED_PIN,GPIO_PIN_SET);


}
//----------------------------
void apagar_all_leds(void){
	
	HAL_GPIO_WritePin(LEDS_PORT,LED_GREEN_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LEDS_PORT,LED_BLUE_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LEDS_PORT,LED_RED_PIN,GPIO_PIN_RESET);

}
//----------------------------
void encender_green(void){

	HAL_GPIO_WritePin(LEDS_PORT,LED_GREEN_PIN,GPIO_PIN_SET);

}
//----------------------------
void encender_blue(void){

	HAL_GPIO_WritePin(LEDS_PORT,LED_BLUE_PIN,GPIO_PIN_SET);

}
//----------------------------
void encender_red(void){

	HAL_GPIO_WritePin(LEDS_PORT,LED_RED_PIN,GPIO_PIN_SET);

}
//----------------------------
void apagar_green(void){

	HAL_GPIO_WritePin(LEDS_PORT,LED_GREEN_PIN,GPIO_PIN_RESET);

}
//----------------------------
void apagar_blue(void){

	HAL_GPIO_WritePin(LEDS_PORT,LED_BLUE_PIN,GPIO_PIN_RESET);

}
//----------------------------
void apagar_red(void){

	HAL_GPIO_WritePin(LEDS_PORT,LED_RED_PIN,GPIO_PIN_RESET);

}
//----------------------------
void alternar_green(void){

	HAL_GPIO_TogglePin(LEDS_PORT,LED_GREEN_PIN);
	
}
//----------------------------
void alternar_blue(void){

	HAL_GPIO_TogglePin(LEDS_PORT,LED_BLUE_PIN);
	
}
//----------------------------
void alternar_red(void){

	HAL_GPIO_TogglePin(LEDS_PORT,LED_RED_PIN);
	
}


