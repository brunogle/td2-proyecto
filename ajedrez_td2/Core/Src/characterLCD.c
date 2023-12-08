#include "characterLCD.h"

#include "main.h"
#include "cmsis_os.h"

extern I2C_HandleTypeDef hi2c1;

//Inicializa el display.
void lcd_init(void) {
	// 4 bit initialization
	vTaskDelay(50);  // wait for >40ms
	lcd_send_cmd(0x30);
	vTaskDelay(5);  // wait for >4.1ms
	lcd_send_cmd(0x30);
	vTaskDelay(1);  // wait for >100us
	lcd_send_cmd(0x30);
	vTaskDelay(10);
	lcd_send_cmd(0x20);  // 4bit mode
	vTaskDelay(10);

	// display initialization
	lcd_send_cmd(0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	vTaskDelay(1);
	lcd_send_cmd(0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
	vTaskDelay(1);
	lcd_send_cmd(0x01);  // clear display
	vTaskDelay(1);
	vTaskDelay(1);
	lcd_send_cmd(0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	vTaskDelay(1);
	lcd_send_cmd(0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
}

void lcd_send_cmd(char cmd) {
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd & 0xf0);
	data_l = ((cmd << 4) & 0xf0);
	data_t[0] = data_u | 0x0C;  //en=1, rs=0
	data_t[1] = data_u | 0x08;  //en=0, rs=0
	data_t[2] = data_l | 0x0C;  //en=1, rs=0
	data_t[3] = data_l | 0x08;  //en=0, rs=0
	HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_LCD, (uint8_t*) data_t, 4,
			100);
}

void lcd_send_data(char data) {
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data & 0xf0);
	data_l = ((data << 4) & 0xf0);
	data_t[0] = data_u | 0x0D;  //en=1, rs=1
	data_t[1] = data_u | 0x09;  //en=0, rs=1
	data_t[2] = data_l | 0x0D;  //en=1, rs=1
	data_t[3] = data_l | 0x09;  //en=0, rs=1
	HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_LCD, (uint8_t*) data_t, 4,
			100);
}

void lcd_send_string(char *str) {
	while (*str)
		lcd_send_data(*(str++));
}

LCDQueueItem_t lcd_msg_from_string(char *str) {
	LCDQueueItem_t msg = { 0 };
	msg.type = STRING_TYPE;
	for (int i = 0; i < 16; i++) {
		msg.data[i] = *str++;
		if (!(*str))
			break;
	}

	return msg;
}

LCDQueueItem_t lcd_msg_clear(void) {
	LCDQueueItem_t msg = { 0 };
	msg.type = CMD_TYPE;
	msg.data[0] = 0x01;
	msg.delay = 3;
	return msg;
}
LCDQueueItem_t lcd_msg_first_line(void){
	LCDQueueItem_t msg = { 0 };
	msg.type = CMD_TYPE;
	msg.data[0] = LCD_POSICION_RENGLON_1;
	msg.delay = 1;
	return msg;
}
LCDQueueItem_t lcd_msg_second_line(void){
	LCDQueueItem_t msg = { 0 };
	msg.type = CMD_TYPE;
	msg.data[0] = LCD_POSICION_RENGLON_2;
	msg.delay = 1;
	return msg;
}
