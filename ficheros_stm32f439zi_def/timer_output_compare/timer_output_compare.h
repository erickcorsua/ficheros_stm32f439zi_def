#ifndef TIMER_OUTPUT_CAPTURE_H
#define TIMER_OUTPUT_CAPTURE_H

#include "stm32f4xx_hal.h"

void timer_output_compare_init(void);

void timer_output_compare_start(void);
void timer_output_compare_stop(void);

void timer_output_compare_incrementar(void);
void timer_output_compare_decrementar(void);

#endif //TIMER_OUTPUT_CAPTURE.H
