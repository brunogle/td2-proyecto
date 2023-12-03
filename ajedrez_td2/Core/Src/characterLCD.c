#include "util.h"
#include "characterLCD.h"
#include "main.h"

 
#define LCD_DB4_Set(x) HAL_GPIO_WritePin(LCD_D4_GPIO_Port,LCD_D4_Pin, x)
#define LCD_DB5_Set(x) HAL_GPIO_WritePin(LCD_D5_GPIO_Port,LCD_D5_Pin, x)
#define LCD_DB6_Set(x) HAL_GPIO_WritePin(LCD_D6_GPIO_Port,LCD_D6_Pin, x)
#define LCD_DB7_Set(x) HAL_GPIO_WritePin(LCD_D7_GPIO_Port,LCD_D7_Pin, x)
#define LCD_RS_Set(x) HAL_GPIO_WritePin(LCD_RS_GPIO_Port,LCD_RS_Pin, x)
#define LCD_E_Set(x) HAL_GPIO_WritePin(LCD_E_GPIO_Port,LCD_E_Pin, x)



void LCD_InternalDelay(uint32_t delay);

//Inicializa el display.
void LCD_Init() {
	LCD_InternalDelay(100000);
	LCD_Write(0x03);
	LCD_InternalDelay(5000);
	LCD_Write(0x03);
	LCD_InternalDelay(200);
	LCD_Write(0x03);
	// Modo de 4 bits
	LCD_Write(0x02);
	// Modo de 4 bits, 2 l�neas, caracter de 5x7
	LCD_Send(0x28, LCD_COMANDO);
	// Apagar display
	LCD_Send(0x08, LCD_COMANDO);
	// Borrar display
	LCD_Send(0x01, LCD_COMANDO);
	// Display encendido, sin cursor, sin titilar el cursor
	LCD_Send(0x0C, LCD_COMANDO);
	// Incremental, sin scrollear
	LCD_Send(0x06, LCD_COMANDO);
}

//Escribe 4 bits en el display.
void LCD_Write(uint8_t data) {
	LCD_DB4_Set(data & 0x01);
	LCD_DB5_Set(data & 0x02);
	LCD_DB6_Set(data & 0x04);
	LCD_DB7_Set(data & 0x08);
	LCD_E_Set(1);
	LCD_InternalDelay(50);
	LCD_E_Set(0);
	LCD_InternalDelay(50);
}

//Escribe un dato o un comando.
void LCD_Send(uint8_t data, uint8_t operation) {
	LCD_RS_Set(operation);
	LCD_Write((data & 0xF0) >> 4);
	LCD_Write(data & 0x0F);
	LCD_RS_Set(0);
	LCD_InternalDelay(10000);
}

//Imprime un string. Necesita previamente haber posicionado el cursor.
void LCD_PrintString(char* string) {
	uint8_t i = 0;
	while (string[i] != '\0') {
		LCD_Send(string[i], LCD_DATO);
		i++;
	}
}

void LCD_InternalDelay(uint32_t delay) {
	DWT_Delay_us(delay * 10);
}
