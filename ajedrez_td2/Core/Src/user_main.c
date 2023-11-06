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

#include <stdlib.h>
#include <math.h>

TIM_HandleTypeDef * user_htim1;



void user_init(){
	ws2812_init();
}



uint8_t sensor_data[64];

void user_loop(){


	reed_scan_sensors(sensor_data);


	for(int row = 0; row < 8; row++){
		for(int col = 0; col < 8; col++){
			if(sensor_data[row*8 + col]){
				ws2812_color_data[row*8 + col] = 0xFF0000;
			}
			else{
				ws2812_color_data[row*8 + col] = 0x000080;
			}
		}
	}

	ws2812_update_leds_from_data(user_htim1);
	while(ws2812_finished_dma == 0){}

	HAL_Delay(10);
}




