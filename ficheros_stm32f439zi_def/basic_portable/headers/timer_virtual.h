#ifndef TIMER_VIRTUAL_H
#define TIMER_VIRTUAL_H

#include "stdbool.h"
#include "stm32f4xx_hal.h"
#include "RTE_Components.h"  

// Simulación de un objeto tipo Timer

typedef struct {
    uint32_t contador_ms;                   // Conteo interno
    uint32_t periodo_ms;                    // Tiempo deseado en ms
    bool timer_virtual_flag;                // Se pone a true cuando se cumple el periodo
    bool timer_virtual_habilitado;          // Control de encendido/apagado del timer
} TimerVirtual;

// API pública
void TimerVirtual_Iniciar(TimerVirtual *timer, uint32_t periodo_ms);
void TimerVirtual_Actualizar(TimerVirtual *timer);
void TimerVirtual_Reiniciar(TimerVirtual *timer);
void TimerVirtual_Apagar(TimerVirtual *timer);
void TimerVirtual_Continuar(TimerVirtual *timer);


#endif //TIMER_VIRTUAL_H

