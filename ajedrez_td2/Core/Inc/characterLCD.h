#ifndef CHARACTERLCD_H_
#define CHARACTERLCD_H_

#define LCD_COMANDO					0
#define LCD_DATO					1

#define LCD_POSICION_RENGLON_1		0x80
#define LCD_POSICION_RENGLON_2	0xC0
// Para displays de 4 renglones
#define LCD_POSICION_RENGLON_3	0x94
#define LCD_POSICION_RENGLON_4	0xD4

//Inicializa el display.
extern void LCD_Init();

//Escribe 4 bits en el display.
extern void LCD_Write(uint8_t data);

//Escribe un dato o un comando.
extern void LCD_Send(uint8_t data, uint8_t operation);

//Imprime un string. Necesita previamente haber posicionado el cursor.
extern void LCD_PrintString(char* string);

#endif /* CHARACTERLCD_H_ */
