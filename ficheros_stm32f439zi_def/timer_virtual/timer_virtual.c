//intro 
//titulo: timer_virtual
//autor: erick cs
//descripcion: timer virtual, que es el complemento de timer_global, para poder instanciar timers
//             virtuales que no dependan del hardware, esta si que la puedes añadir como una libreria
//             diseñado para trabajar en ms principalmente

//librerias

#include "timer_virtual.h"

#include "stdbool.h"
#include "stm32f4xx_hal.h"
#include "RTE_Components.h"  

//macros

//variables

//prototype funtions

void TimerVirtual_Iniciar(TimerVirtual *timer, uint32_t periodo_ms);
void TimerVirtual_Actualizar(TimerVirtual *timer);
void TimerVirtual_Reiniciar(TimerVirtual *timer);
void TimerVirtual_Apagar(TimerVirtual *timer);
void TimerVirtual_Continuar(TimerVirtual *timer);
//funtions

void TimerVirtual_Iniciar(TimerVirtual *timer, uint32_t periodo_ms){

  timer->contador_ms = 0;
	timer->periodo_ms = periodo_ms;
	timer->timer_virtual_flag = false;
	timer->timer_virtual_habilitado = true;

}
void TimerVirtual_Actualizar(TimerVirtual *timer){

	if(timer->timer_virtual_habilitado){
		
  	timer->contador_ms++;

	  if(timer->contador_ms >= timer->periodo_ms){
	
			timer->contador_ms = 0;
		  timer->timer_virtual_flag = true;		
	
	  }
  }
}
void TimerVirtual_Reiniciar(TimerVirtual *timer){

	if(timer->timer_virtual_habilitado){
	
		timer->contador_ms = 0;
		timer->timer_virtual_flag = false;
	
	}
}

void TimerVirtual_Apagar(TimerVirtual *timer){

	if(timer->timer_virtual_habilitado){
	
		TimerVirtual_Reiniciar(timer);
		timer->timer_virtual_habilitado = false;
		
	}
}
void TimerVirtual_Continuar(TimerVirtual *timer){

	if(!timer->timer_virtual_habilitado){
		
		timer->timer_virtual_habilitado = true;
	
	}
}

