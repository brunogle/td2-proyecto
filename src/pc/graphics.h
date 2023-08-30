#ifndef DRAWINGS_H
#define DRAWINGS_H

#include <stdint.h>

#define SIZE 600

#include "../game/lighting/lighting.h"

void graphics_draw_all();


void graphics_start();

/*
Array de donde draw_all() lee para dibujar las casillas de los tableros
*/
extern led_color graphics_led_colors[8][8];

void graphics_get_sensors(uint8_t sensor_data[8]);



#endif

