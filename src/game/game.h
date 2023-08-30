#ifndef GAME_H
#define GAME_H

#include "engine/engine.h"
#include "stdint.h"

typedef void (*sensor_reader_t)(char[8]);

typedef struct{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} led_color;


void set_sensor_reader(sensor_reader_t reader);

void get_led_colors(led_color board_led_colors[8][8]);

void game_reset();

void game_fsm();

#endif
