//intro 
//titulo: boton_generico
//autor: erick cs
//descripcion: fichero para la gestion de botones, es practicamente una plantilla, para gestionar pulsaciones
//             largas y cortas. Desde luego que se mitiga los efectos del debouncing, para su buen funcionamiento
//             se usa el timer_global junto al timer_virtual

//----------librerias---------

#include "boton_generico.h"

#include "RTE_Components.h"
#include "stm32f4xx_hal.h"
#include "stdbool.h"
#include "timer_virtual.h"

//----------------------------

//-----------macros-----------

#define GPIO_BOTON GPIO_PIN_numero  //-->cambiar
#define GPIO_BTN_PORT GPIOpuerto    //-->cambiar
#define PULSO_LARGO_MS 1000
#define PULSO_ANTI_DEBOUNCING_MS 20
//----------------------------

//--------variables-----------

typedef enum{ BTN_IDLE, BTN_IN, BTN_OUT} btn_state_t;
static btn_state_t estado_btn;

static GPIO_InitTypeDef GPIO_InitStruct;

volatile bool gb_btn_fall_evnt = false;
volatile bool gb_btn_init = false;

volatile bool gb_btn_corto = false; //-->cambiar el nombre del boolena para distinguirlo en caso de mas botones
volatile bool gb_btn_larga = false; //-->cambiar el nombre de  boolena para distinguirlo en caso de mas botones

static uint16_t count_ms_btn;
TimerVirtual tim_btn_generico;      //-->cambiar el nombre de generico

//----------------------------
//-----funtions prototype-----
void boton_generico_init(void);     //-->cambiar el nombre de generico
void boton_generico_callback(void); //-->cambiar el nombre de generico
void EXTIgenerico_IRQHandler(void);    //-->cambiar el nombre de generico, el handler el que toque

void boton_generico_fsm(void);     //-->cambiar el nombre de generico


//void boton_generico_irq_handler(void); --> si usas mas pines con numero
//                                       compartido, debes habilitar esto y tocar en el stm32f4xx_it.c


//----------------------------
//----------funtions----------

//*****************************************************
//*****************************************************
	
				//incia el hardware (botones), debes incluir en el main

//*****************************************************
//*****************************************************
void tres_botones_con_automata_init(void){

	

	__HAL_RCC_GPIOpuerto_CLK_ENABLE();      //-->cambiar el nombre puerto
	
    HAL_NVIC_SetPriority(EXTIque_toque_IRQn, 0, 0);
  
    HAL_NVIC_EnableIRQ(EXTIque_toque_IRQn);
  
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	GPIO_InitStruct.Pin = GPIO_BOTON;

	HAL_GPIO_Init(GPIO_BTN_PORT,&GPIO_InitStruct);

    TimerVirtual_Iniciar(&tim_btn_generico,1);//--> 1ms	

    gb_tres_botones_init = true; 
    gb_fall_event_izq = false;
 
    estado_btn = BTN_IDLE;  
  
    count_ms_btn = 0;

}
//*****************************************************
//*****************************************************

				//Callbacks

//*****************************************************
//*****************************************************
/*
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
    if(GPIO_Pin == GPIO_BOTON_AZUL){
        boton_azul_callback();
    }
  }
	
*/
void tres_botones_callback_btn_generico(void){

  gb_btn_fall_evnt = true;                   //-->cambiar el nombre de generico
	
}
//*****************************************************
//*****************************************************

				//IRQ_Handler para los botones

//*****************************************************
//*****************************************************
void EXTIgenerico_IRQHandler(void){

	HAL_GPIO_EXTI_IRQHandler(BTN_IZQ_PIN);
	
}
/*
en caso de usar mas pines con numeros, usa este y comenta el otro, tendras que ponerlo en el stm32f4xx_it
void boton_generico_irq_handler(void){

  HAL_GPIO_EXTI_IRQHandler(GPIO_BOTON_AZUL);

}
*/
//*****************************************************
//*****************************************************

//fsm(maquina de estados finitos :-) )

//*****************************************************
//*****************************************************
void boton_azul_fsm(void){

	if(gb_btn_init){
		
		switch(estado_btn){
		
			case BTN_IDLE:
				
			  count_ms_btn = 0;
			  gb_btn_corto = false;
			  gb_btn_larga = false;
			
			  if(gb_btn_fall_evnt){
				  gb_btn_fall_evnt = false;
					
					estado_btn = BTN_IN;
				
				}
				
			break;
			
			case BTN_IN:
				if(tim_btn_generico.timer_virtual_flag){              //-->cambiar generico
				  tim_btn_generico.timer_virtual_flag = false;        //-->cambiar generico
					
					count_ms_btn++;
					if(count_ms_btn >= PULSO_ANTI_DEBOUNCING_MS){
					
						if(HAL_GPIO_ReadPin(GPIO_BTN_PORT,GPIO_BOTON) == GPIO_PIN_RESET){
						
							estado_btn = BTN_OUT;
						
						}
						else{
							
							estado_btn = BTN_IDLE;
						
						}
					}
				}
			
			break;
		
			case BTN_OUT:
				if(tim_btn.timer_virtual_flag){
				  tim_btn.timer_virtual_flag = false;
					
					count_ms_btn++;
					
					if(HAL_GPIO_ReadPin(GPIO_BTN_PORT,GPIO_BOTON) == GPIO_PIN_SET){
						
						if(count_ms_btn > PULSO_LARGO_MS){
							
							gb_btn_corto = false;
							gb_btn_larga = true;						
						
						}
						else{
						
							gb_btn_corto = true;
							gb_btn_larga = false;						
						
						}
						estado_btn = BTN_IDLE;
						
				  }
			  }
								
			break;				
		} 
	}
}





