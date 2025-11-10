
#ifndef LCD_SPI_H
#define LCD_SPI_H

#include "Driver_SPI.h"           
#include "RTE_Components.h"           
#include "RTE_Device.h"           
#include "stdbool.h"
#include "stm32f4xx_hal.h"
#include "LCD_SPI.h"    

extern ARM_DRIVER_SPI Driver_SPI1;

extern unsigned char buffer[512];
extern uint16_t positionL1;
extern uint16_t positionL2;

void LCD_Init(void);

void LCD_Reset(void);
void LCD_wr_data(unsigned char data);
void LCD_wr_cmd(unsigned char cmd);
void LCD_update(void);

void LCD_symbolToLocalBuffer_L1(const char *str);
void LCD_symbolToLocalBuffer_L2(const char *str);
void LCD_symbolToLocalBuffer(uint8_t line, const char *text);

void LCD_write_ERROR(uint8_t line);
void LCD_clean(uint8_t line);

void LCD_print_prueba_valores(int valor1, float valor2);



#endif //LCD_SPI_H
