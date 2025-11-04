#ifndef TIMER_INPUT_CAPTURE_H
#define TIMER_INPUT_CAPTURE_H

#include "stm32f4xx_hal.h"

void timer_input_capture_init(void);

void timer_input_capture_start(void);
void timer_input_capture_stop(void);

void timer_input_capture_irq_handler(void); //handler
void timer_input_capture_callback(void);

uint32_t timer_input_capture_get_frecuencia(void);

#endif //TIMER_INPUT_CAPTURE.H
