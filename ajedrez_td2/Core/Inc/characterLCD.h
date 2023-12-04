#ifndef CHARACTERLCD_H_
#define CHARACTERLCD_H_

#define SLAVE_ADDRESS_LCD 0x27 << 1

#define LCD_POSICION_RENGLON_1		0x80
#define LCD_POSICION_RENGLON_2	0xC0
// Para displays de 4 renglones
#define LCD_POSICION_RENGLON_3	0x94
#define LCD_POSICION_RENGLON_4	0xD4

void lcd_init (void);
void lcd_send_cmd(char data);
void lcd_send_data (char data);

#endif /* CHARACTERLCD_H_ */
