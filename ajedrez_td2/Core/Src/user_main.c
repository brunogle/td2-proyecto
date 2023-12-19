/*
 * user_main.c
 *
 *  Created on: Nov 6, 2023
 *      Author: bruno
 */

#include "stm32f4xx_hal.h"
#include "main.h"
#include "cmsis_os.h"

#include "user_main.h"
#include "led/ws2812.h"
#include "reed/reed.h"

#include "engine/engine.h"
#include "lighting/lighting.h"
#include "game.h"
#include "movement.h"

#include <stdlib.h>
#include <math.h>

#include "characterLCD.h"

TIM_HandleTypeDef *user_htim1;
extern xQueueHandle buttons_queue;
extern xQueueHandle lcd_queue;

extern game_state_t engine_game_state;
extern char lighting_state;

extern color_t cpu_player;

void user_init() {
	//game_set_sensor_reader((sensor_reader_t) reed_scan_sensors);

	game_set_led_output_array(ws2812_color_data);

	game_reset();

	movement_state = WAIT_STATE;
}

uint8_t get_side_to_move()
{
	return (engine_game_state.side_to_move == WHITE ? 0 : 1);
}

extern char movement_state;

uint8_t get_finished_state()
{
	return (movement_state == GAME_FINISHED_STATE ? 1 : 0);
}

void set_cpu_player(uint8_t on)
{
	cpu_player = on ? BLACK : COLOR_EMPTY;
}

uint8_t get_error_position()
{
	return (lighting_state == LIGHTING_ERROR_STATE ? 1 : 0);
}

piece_t get_promotion() {

	LCDQueueItem_t msg = { 0 };
	uint8_t button = 0;
	piece_t piece = QUEEN;

	/*    KING,
	 QUEEN,
	 ROOK,
	 BISHOP,
	 KNIGHT,
	 PAWN,
	 */

	//xQueueReset(buttons_queue); // Borro mensajes si habia alguna pulsada de boton pendiente

	while (1) {
		msg = lcd_msg_clear();
		xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
		msg = lcd_msg_first_line();
		xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
		msg = lcd_msg_from_string("Elija pieza:");
		xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
		msg = lcd_msg_second_line();
		xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
		switch (piece) {
		case QUEEN:
			msg = lcd_msg_from_string("-> Reina");
			xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
			break;
		case ROOK:
			msg = lcd_msg_from_string("-> Torre");
			xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
			break;
		case KNIGHT:
			msg = lcd_msg_from_string("-> Caballo");
			xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
			break;
		case BISHOP:
			msg = lcd_msg_from_string("-> Alfil");
			xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
			break;
		default:
		}
		xQueueReceive(buttons_queue, &button, portMAX_DELAY);

		switch (button) {
		case 3: // Boton verde, salgo del while con la ultima pieza seleccionada
			break;
		case 2: // Cambio de pieza "ascendentemente"
			piece++;
			if (piece > 4) // Salteo el peon
				piece = 1; // Salteo el rey

			button = 0;
			break;
		case 1: // Cambio de pieza "descendentemente"
			piece--;
			if (piece < 1) // Salteo el rey
				piece = 4; // Salteon el peon

			button = 0;
			break;
		}

		if (button)
			break;
	}

	return piece;
}

uint8_t sensor_data[64];

void user_loop() {
	game_fsm();
}

