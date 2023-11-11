/*
 * ws2812.c
 *
 *  Created on: Nov 6, 2023
 *      Author: bruno
 */

#include "led/ws2812.h"

#include "main.h"


uint16_t ws2812_pwm_data[64 * 24 + 60]; //PWM data fed to TIM1_CH4 by DMA

uint32_t ws2812_color_data[8][8]; //Color data: 24bits: 0xBBRRGG

uint8_t ws2812_finished_dma; //DMA finished flag


void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim){
	HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_4);
	ws2812_finished_dma = 1;
}

void ws2812_init(){
	for(int i = 0; i < (64 * 24 + 60); i++){
		if(i < 60)
			ws2812_pwm_data[i] = WS2812_BREAK;
		else
			ws2812_pwm_data[i] = WS2812_ZERO;
	}
}

uint8_t led_strip_pos_lut[] = {	63, 48, 47, 32, 31, 16, 15, 0,
								62, 49, 46, 33, 30, 17, 14, 1,
								61, 50, 45, 34, 29, 18, 13, 2,
								60, 51, 44, 35, 28, 19, 12, 3,
								59, 52, 43, 36, 27, 20, 11, 4,
								58, 53, 42, 37, 26, 21, 10, 5,
								57, 54, 41, 38, 25, 22, 9, 6,
								56, 55, 40, 39, 24, 23, 8, 7};



void ws2812_update_pwm_data(){
	for(int row = 0; row < 8; row++){
		for(int col = 0; col < 8; col++){

			int index = row*8 + col;

			uint32_t ws2812_corrected_color = ((ws2812_color_data[row][col] & (0x00FF00)) << 8) |
											  ((ws2812_color_data[row][col] & (0xFF0000)) >> 8) |
										      (ws2812_color_data[row][col] & 0x0000FF);

			for(int i = 0; i < 24; i++){



				if(ws2812_corrected_color & (0x800000 >> i))
					ws2812_pwm_data[60 + 24*led_strip_pos_lut[index] + i] = WS2812_ONE;
				else
					ws2812_pwm_data[60 + 24*led_strip_pos_lut[index] + i] = WS2812_ZERO;
			}
		}

	}
}

void ws2812_update_leds_from_data(TIM_HandleTypeDef *htim){
	  ws2812_update_pwm_data();
	  ws2812_finished_dma = 0;
	  HAL_TIM_PWM_Start_DMA(htim, TIM_CHANNEL_4, (uint32_t *)ws2812_pwm_data, sizeof(ws2812_pwm_data) / sizeof(uint16_t));
}
