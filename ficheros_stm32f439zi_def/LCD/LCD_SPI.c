/**********************************************************

autor       :  erick cs
titulo      :  LCD_SPI.c
descripcion :  se usa el CMSIS para usar un Driver_SPI. Para este caso se usa el SPI1, con los pines
               pa7 -> mosi
               pa5 -> clk
               pa6 -> rst
               pf13 -> ao (comando / dato)
               pd14 -> chip select
fecha:

*/


#include "Driver_SPI.h"           
#include "RTE_Components.h"           
#include "RTE_Device.h"           
#include "stdbool.h"
#include "stm32f4xx_hal.h"
#include "LCD_SPI.h"    
#include "Arial12x12.h"    
#include "string.h"
#include "stdio.h"

// -------- macros --------

//---------------pines y puertos---------------
#define LCD_RESET_PIN GPIO_PIN_6
#define LCD_A0_PIN    GPIO_PIN_13
#define LCD_CS_N_PIN  GPIO_PIN_14

#define LCD_RESET_PORT GPIOA
#define LCD_A0_PORT    GPIOF
#define LCD_CS_N_PORT  GPIOD

// -------- Comandos LCD --------
#define LCD_DISPLAY_OFF        0xAE
#define LCD_DISPLAY_ON         0xAF
#define LCD_SET_BIAS_1_9       0xA2
#define LCD_RAM_NORMAL         0xA0
#define LCD_COM_SCAN_NORMAL    0xC8
#define LCD_INTERNAL_RES_2     0x22
#define LCD_POWER_ON           0x2F
#define LCD_START_LINE_0       0x40
#define LCD_SET_CONTRAST       0x81
#define LCD_CONTRAST_VALUE     0x18  // normal
#define LCD_ALL_POINTS_NORMAL  0xA4
#define LCD_DISPLAY_NORMAL     0xA6


//variables
extern ARM_DRIVER_SPI Driver_SPI1;

static ARM_DRIVER_SPI* SPIdrv = &Driver_SPI1;

static GPIO_InitTypeDef GPIO_InitStruct;

static bool volatile spi_terminado = false;
static bool volatile b_lcd_init = false;

unsigned char buffer[512] = {0};

uint16_t positionL1 = 0;
uint16_t positionL2 = 0;

//prototype funtions

void LCD_Init(void);

void LCD_Reset(void);

void LCD_wr_data(unsigned char data);
void LCD_wr_cmd(unsigned char cmd);

void LCD_update(void);

void symbolToLocalBuffer_L1(uint8_t symbol);

void LCD_symbolToLocalBuffer_L1(const char *str);
void LCD_symbolToLocalBuffer_L2(const char *str);
void LCD_symbolToLocalBuffer(uint8_t line, const char *text);
void LCD_write_ERROR(uint8_t line);
void LCD_clean(uint8_t line);
void LCD_print_prueba_valores(int valor1, float valor2);

static void mySPI_callback(uint32_t event);
static void gpio_init(void);
static void SPI_Init(void);

//funtions

/*        __          __  .__               
  _______/  |______ _/  |_|__| ____   ______
 /  ___/\   __\__  \\   __\  |/ ___\ /  ___/
 \___ \  |  |  / __ \|  | |  \  \___ \___ \ 
/____  > |__| (____  /__| |__|\___  >____  >
     \/            \/             \/     \/ 
*/
static void gpio_init(void){

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  
  GPIO_InitStruct.Pin   = LCD_RESET_PIN;
  HAL_GPIO_Init(LCD_RESET_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin   = LCD_A0_PIN;
  HAL_GPIO_Init(LCD_A0_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin   = LCD_CS_N_PIN;
  HAL_GPIO_Init(LCD_CS_N_PORT, &GPIO_InitStruct);
 
  //por defecto, para comprobar si estan bien
 
  HAL_GPIO_WritePin(LCD_RESET_PORT, LCD_RESET_PIN, GPIO_PIN_SET);  // Estado alto por defecto (sin reset)
  HAL_GPIO_WritePin(LCD_A0_PORT, LCD_A0_PIN, GPIO_PIN_SET);        // A0 en alto por defecto
  HAL_GPIO_WritePin(LCD_CS_N_PORT, LCD_CS_N_PIN, GPIO_PIN_SET);    // CS inactivo 
  
}

static void mySPI_callback(uint32_t event){
  
  if (event & ARM_SPI_EVENT_TRANSFER_COMPLETE) {
        spi_terminado = true;
    }

}

static void SPI_Init(void){

  //SPIdrv->Initialize(mySPI_callback);
	SPIdrv->Initialize(NULL);
  SPIdrv->PowerControl(ARM_POWER_FULL);
  SPIdrv->Control(ARM_SPI_MODE_MASTER | ARM_SPI_CPOL1_CPHA1 | ARM_SPI_MSB_LSB |
	ARM_SPI_DATA_BITS(8), 1000000);//poner a 1MHz para ver mejor todo en el analizador logico 1000000 el correcto es de: 20000000
  SPIdrv->Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_INACTIVE);

  gpio_init();
  
}


//--------------------------------------------------------------
/*               __                              
  ____ ___  ____/  |_  ___________  ____   ______
_/ __ \\  \/  /\   __\/ __ \_  __ \/    \ /  ___/
\  ___/ >    <  |  | \  ___/|  | \/   |  \\___ \ 
 \___  >__/\_ \ |__|  \___  >__|  |___|  /____  >
     \/      \/           \/           \/     \/ 
*/


void LCD_Init(void){
  if(!b_lcd_init){
		
		b_lcd_init=true;
		
	  SPI_Init();      // Inicializa SPI y GPIOs
    LCD_Reset();     // Pulso de reset

	
	  LCD_wr_cmd(LCD_DISPLAY_OFF);
    LCD_wr_cmd(LCD_SET_BIAS_1_9);
	
    LCD_wr_cmd(LCD_RAM_NORMAL);
    LCD_wr_cmd(LCD_COM_SCAN_NORMAL);
 	
    LCD_wr_cmd(LCD_INTERNAL_RES_2);
    LCD_wr_cmd(LCD_POWER_ON);
	
    LCD_wr_cmd(LCD_START_LINE_0);
    LCD_wr_cmd(LCD_DISPLAY_ON);
	
    LCD_wr_cmd(LCD_SET_CONTRAST);
    LCD_wr_cmd(LCD_CONTRAST_VALUE);
	
    LCD_wr_cmd(LCD_ALL_POINTS_NORMAL);
    LCD_wr_cmd(LCD_DISPLAY_NORMAL);
  }
}

//--------------------------------------------------------------
void LCD_Reset(void){ //puede ser static?
                       
  HAL_GPIO_WritePin(LCD_RESET_PORT, LCD_RESET_PIN, GPIO_PIN_RESET); // reset activo (bajo)
  HAL_Delay(10);                                                    // espera 10ms
	HAL_GPIO_WritePin(LCD_RESET_PORT, LCD_RESET_PIN, GPIO_PIN_SET);   // reset inactivo (alto)
  HAL_Delay(100);                                          // espera 10ms para estabilización
}

//--------------------------------------------------------------

void LCD_wr_data(unsigned char data){

	spi_terminado = false;
	
	ARM_SPI_STATUS stat_status;
	int32_t stat_send;

  // Seleccionar CS = 0;
  HAL_GPIO_WritePin(LCD_CS_N_PORT, LCD_CS_N_PIN, GPIO_PIN_RESET); 
  // Seleccionar A0 = 1; 
  HAL_GPIO_WritePin(LCD_A0_PORT, LCD_A0_PIN, GPIO_PIN_SET); 
	// Escribir un dato (data) usando la función SPIDrv->Send(…);
	stat_send = SPIdrv->Send(&data, sizeof(data)); 
	// Esperar a que se libere el bus SPI;
	//while(!spi_terminado){}
	
	do
  {
    stat_status = SPIdrv->GetStatus();  // Devuelve una variable ARM_SPI_STATUS
  }while(stat_status.busy);
  
	// Seleccionar CS = 1;
	HAL_GPIO_WritePin(LCD_CS_N_PORT, LCD_CS_N_PIN, GPIO_PIN_SET); 

}
//--------------------------------------------------------------
void LCD_wr_cmd(unsigned char cmd){
	
	spi_terminado = false;
	
	ARM_SPI_STATUS stat_status;
	int32_t stat_send;
	
 // Seleccionar CS = 0;
 HAL_GPIO_WritePin(LCD_CS_N_PORT, LCD_CS_N_PIN, GPIO_PIN_RESET); 
 // Seleccionar A0 = 0;
 HAL_GPIO_WritePin(LCD_A0_PORT, LCD_A0_PIN, GPIO_PIN_RESET); 
 // Escribir un comando (cmd) usando la función SPIDrv->Send(…);
 
	stat_send = SPIdrv->Send(&cmd, sizeof(cmd)); 
 // Esperar a que se libere el bus SPI;
 //while(!spi_terminado){}
	
	 do
  {
    stat_status = SPIdrv->GetStatus();  // Devuelve una variable ARM_SPI_STATUS
  }while(stat_status.busy);
  
	
 // Seleccionar CS = 1;
 HAL_GPIO_WritePin(LCD_CS_N_PORT, LCD_CS_N_PIN, GPIO_PIN_SET); 
}

//--------------------------------------------------------------
void LCD_update(void)
{
	// con esto dibujas el cuadro
  /*
  for (int i = 0; i < 8; i++) {
    buffer[i] = 0xFF;  // 11111111 ? 8 píxeles encendidos en vertical
  } 
	
	//----------------------
	*/
  /*
  
	 unsigned char letraA[8] = {0x00, 0x30, 0x46, 0x40, 0x40, 0x46, 0x30, 0x00};
	
	for (int i = 0; i < 8; i++) {

   	buffer[i] = letraA[i];  // 

  }*/
	
 int i;
 LCD_wr_cmd(0x00); // 4 bits de la parte baja de la dirección a 0
 LCD_wr_cmd(0x10); // 4 bits de la parte alta de la dirección a 0
 LCD_wr_cmd(0xB0); // Página 0

 for(i=0;i<128;i++){
 LCD_wr_data(buffer[i]);
 }

 LCD_wr_cmd(0x00); // 4 bits de la parte baja de la dirección a 0
 LCD_wr_cmd(0x10); // 4 bits de la parte alta de la dirección a 0
 LCD_wr_cmd(0xB1); // Página 1

 for(i=128;i<256;i++){
 LCD_wr_data(buffer[i]);
 }

 LCD_wr_cmd(0x00);
 LCD_wr_cmd(0x10);
 LCD_wr_cmd(0xB2); //Página 2
 for(i=256;i<384;i++){
 LCD_wr_data(buffer[i]);
 }

 LCD_wr_cmd(0x00);
 LCD_wr_cmd(0x10);
 LCD_wr_cmd(0xB3); // Pagina 3


 for(i=384;i<512;i++){
 LCD_wr_data(buffer[i]);
 }
}

//--------------------------------------------------------------

void LCD_symbolToLocalBuffer_L1(const char *str){
  //como una pagina mide 8 pixeles de alto y queremos letras mas grandes, tenemos que dibujar en dos paginas.
  // El orden de pintar es: pantalla 0 columna 0, pantalla 1 columna 0, pantalla 0 columna 1, pantalla 1 columna 1...
  uint8_t j,i, value1, value2,width;
  uint16_t offset = 0;
  positionL1 = 0;

    
	for(j=0;j < strlen(str);j++){
      
    offset = 25*(str[j] - ' ');
		   
	  width = Arial12x12[offset];
    
	  if (positionL1 + width > 128){
      LCD_write_ERROR(1);
      return;
    }
		 
    for(i = 0; i < 12; i++){
    
      value1 = Arial12x12[offset + i*2 + 1]; //cogemos datos alternos con value1 y value2. 
      value2 = Arial12x12[offset + i*2 + 2];

      buffer[i + positionL1] = value1;
      buffer[i + 128 + positionL1] = value2;
    } 
    positionL1 = positionL1 + Arial12x12[offset];//si ves que se pegan o se solapan poner un +1
      
  }
   
	LCD_update();  
			 
}
  
 void LCD_symbolToLocalBuffer_L2(const char *str){
  //como una pagina mide 8 pixeles de alto y queremos letras mas grandes, tenemos que dibujar en dos paginas.
  // El orden de pintar es: pantalla 0 columna 0, pantalla 1 columna 0, pantalla 0 columna 1, pantalla 1 columna 1...
    
	uint8_t j,i, value1, value2,width;
  uint16_t offset = 0;
  positionL2 = 0;

    
	for(j=0;j < strlen(str);j++){
      
    offset = 25*(str[j] - ' ');
		   
	  width = Arial12x12[offset];
    
	  if (positionL2 + width > 128){
      LCD_write_ERROR(2);
      return;
    }
		 
    for(i = 0; i < 12; i++){
    
      value1 = Arial12x12[offset + i*2 + 1]; //cogemos datos alternos con value1 y value2. 
      value2 = Arial12x12[offset + i*2 + 2];

      buffer[i + 256 + positionL2] = value1;
      buffer[i + 384 + positionL2] = value2;
    } 
    positionL2 = positionL2 + Arial12x12[offset]; //si ves que se pegan o se solapan poner un +1
      
  }
   
	LCD_update();  
			 
}
//------------------------------------------------- 

void LCD_symbolToLocalBuffer(uint8_t line, const char *text){
  
  if(line == 1)
    LCD_symbolToLocalBuffer_L1(text);

  if(line == 2)
    LCD_symbolToLocalBuffer_L2(text);

}

//-------------------------------------------------
void LCD_write_ERROR(uint8_t line){

  LCD_clean(line);
  
  LCD_symbolToLocalBuffer(line,"ERROR");   

  LCD_update(); // función que envía el buffer completo al LCD
}
//-------------------------------------------------
void LCD_clean(uint8_t line){
  
    // Limpiar el buffer antes de escribir (opcional)
  if(line == 1){
    for (int i = 0; i < 256; i++){ 
        buffer[i] = 0x00;
		}
		positionL1 = 0;
		
  }
	else{
    for (int i = 256; i < 512; i++) 
      buffer[i] = 0x00;
  }
  positionL2 = 0;	
	
}
//-------------------------------------------------  
void LCD_print_prueba_valores(int valor1, float valor2){
    char line1[32];
    char line2[32];

    // Formatea la primera línea: entero
    sprintf(line1, "Prueba valor1: %d", valor1);
    // Formatea la segunda línea: flotante con 5 decimales
    sprintf(line2, "Prueba valor2: %.5f", (double)valor2);

    // Imprime en la línea superior (1) y en la inferior (2)
    LCD_symbolToLocalBuffer(1, line1);
    LCD_symbolToLocalBuffer(2, line2);
}







