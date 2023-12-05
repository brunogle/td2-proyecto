/*
 * user_main.c
 *
 *  Created on: Nov 6, 2023
 *      Author: bruno
 */

#include "stm32f4xx_hal.h"
#include "main.h"

#include "user_main.h"
#include "led/ws2812.h"
#include "reed/reed.h"

#include "engine/engine.h"
#include "lighting/lighting.h"
#include "game.h"

#include <stdlib.h>
#include <math.h>

TIM_HandleTypeDef * user_htim1;



void user_init(){
	game_set_sensor_reader((sensor_reader_t)reed_scan_sensors);

	game_set_led_output_array(ws2812_color_data);

	game_reset();
}


piece_t get_promotion()
{
    printf("Ingrese numero de pieza para seleccionar la promocion:\n");
    printf("Caballo: %d\n", KNIGHT);
    printf("Alfil: %d\n", BISHOP);
    printf("Reina: %d\n", QUEEN);

    unsigned int prom;

    printf("Seleccion: ");
    scanf("%u", &prom);

    return (piece_t) prom;
}


uint8_t sensor_data[64];

void user_loop(){
	game_fsm();
}




