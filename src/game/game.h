#ifndef GAME_H
#define GAME_H

#include "stdint.h"
#include "lighting/lighting.h"

typedef void (*sensor_reader_t)(char[8]);




void game_set_sensor_reader(sensor_reader_t reader);

void game_set_led_output_array(led_color output_array[8][8]);

void game_get_led_colors(led_color board_led_colors[8][8]);

void game_reset();

void game_fsm();

#endif
