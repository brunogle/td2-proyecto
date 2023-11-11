/*
 * ws2812.h
 *
 *  Created on: Nov 6, 2023
 *      Author: bruno
 */

#ifndef INC_LED_WS2812_H_
#define INC_LED_WS2812_H_

#include "stm32f4xx_hal.h"


#define WS2812_ZERO 	(104 - 76) 		// This gives a HIGH signal of 350ns (and stays 900ns LOW)
#define WS2812_ONE 		76              // This gives a HIGH signal of 950ns (and stays 300ns LOW)
#define WS2812_BREAK 	0               // This stays 1250ns LOW


extern uint16_t ws2812_pwm_data[64 * 24 + 60]; //PWM data fed to TIM1_CH4 by DMA

extern uint32_t ws2812_color_data[8][8]; //Color data: 24bits: 0xBBRRGG

extern uint8_t ws2812_finished_dma; //DMA finished flag



void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim);

void ws2812_init();

void ws2812_update_pwm_data();

void ws2812_update_leds_from_data(TIM_HandleTypeDef *htim);

#endif /* INC_LED_WS2812_H_ */
