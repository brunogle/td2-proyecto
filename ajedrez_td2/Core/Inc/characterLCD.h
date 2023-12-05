#ifndef CHARACTERLCD_H_
#define CHARACTERLCD_H_

#include "main.h"

#define SLAVE_ADDRESS_LCD 0x27 << 1

#define LCD_POSICION_RENGLON_1	0x80
#define LCD_POSICION_RENGLON_2	0xC0
// Para displays de 4 renglones
#define LCD_POSICION_RENGLON_3	0x94
#define LCD_POSICION_RENGLON_4	0xD4

void lcd_init (void);
void lcd_send_cmd(char data);
void lcd_send_data (char data);

// Estructura para mensajes de la cola del LCD
// Tipo 0: CMD
// En data[0] se guarda el comando a mandar
// En delay se guarda el tiempo en milisegundos que necesita el comando para ejecutarse
// Tipo 1: Data (char)
// En data[0] se guarda el dato a mandar
// El delay no es necesario
// Tipo 2: Data (string)
// En data se guarda el string a mandar (maximo 16 caracteres por linea)
// El delay no es necesario
typedef struct LCD_QueueItem
{
	uint8_t type;
	char data[17]; // Max 16 caracteres + \0
	uint32_t delay;
} LCDQueueItem_t;

#endif /* CHARACTERLCD_H_ */