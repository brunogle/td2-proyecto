/*
 * tareas.c
 *
 *  Created on: Dec 5, 2023
 *      Author: Tomás
 */

#include "tareas.h"

#include "main.h"
#include "cmsis_os.h"

#include "characterLCD.h"
#include "led/ws2812.h"
#include "debounce.h"
#include "user_main.h"

extern xQueueHandle lcd_queue;
extern xQueueHandle buttons_queue;
extern xSemaphoreHandle ws2812_sem;

extern TIM_HandleTypeDef htim1;

void tareas_error_handler(uint8_t);

void t_LCD(void*) {

	if (lcd_queue == NULL) {
		tareas_error_handler(1);
	}

	lcd_init();

	LCDQueueItem_t msg;

	while (1) {
		xQueueReceive(lcd_queue, &msg, portMAX_DELAY);
		switch (msg.type) {
		case 0:
			lcd_send_cmd(msg.data[0]);
			vTaskDelay((TickType_t) msg.delay);
			break;
		case 1:
			lcd_send_data(msg.data[0]);
			break;
		case 2:
			lcd_send_string(msg.data);
			break;
		default:
		}
	}
}

void t_Botones(void*) {

	Debounce_t b1, b2, b3;

	debounce_init(&b1, 1, 2);
	debounce_init(&b2, 1, 2);
	debounce_init(&b3, 1, 2);

	if (buttons_queue == NULL) {
		tareas_error_handler(4);
	}

	uint8_t msg = 0;

	while (1) {
		debounce_run(&b1, HAL_GPIO_ReadPin(BOTON_1_GPIO_Port, BOTON_1_Pin));
		debounce_run(&b2, HAL_GPIO_ReadPin(BOTON_2_GPIO_Port, BOTON_2_Pin));
		debounce_run(&b3, HAL_GPIO_ReadPin(BOTON_3_GPIO_Port, BOTON_3_Pin));

		if (debounce_flank(&b1)) {
			msg = 1;
			if (uxQueueMessagesWaiting(buttons_queue) < 20) {
				xQueueSend(buttons_queue, (void* )&msg, portMAX_DELAY);
			}
		}

		if (debounce_flank(&b2)) {
			msg = 2;
			if (uxQueueMessagesWaiting(buttons_queue) < 20) {
				xQueueSend(buttons_queue, (void* )&msg, portMAX_DELAY);
			}
		}

		if (debounce_flank(&b3)) {
			msg = 3;
			if (uxQueueMessagesWaiting(buttons_queue) < 20) {
				xQueueSend(buttons_queue, (void* )&msg, portMAX_DELAY);
			}
		}

		vTaskDelay(10);
	}
}

void t_userLoop(void*) {

	if (ws2812_sem == NULL) {
		tareas_error_handler(2);
	}

	if (buttons_queue == NULL) {
		tareas_error_handler(4);
	}

	user_htim1 = &htim1;
	user_init();

	ws2812_init();

	while (1) {
		user_loop();
		xSemaphoreGive(ws2812_sem);
		//ws2812_update_leds_from_data(user_htim1);
		vTaskDelay(33); //30 fps aprox
	}
}

void t_WS2812(void*) {

	if (ws2812_sem == NULL) {
		tareas_error_handler(2);
	}

	ws2812_init();

	while (1) {

		if (xSemaphoreTake(ws2812_sem,portMAX_DELAY) != pdTRUE) {
			tareas_error_handler(3);
		}

		ws2812_update_leds_from_data(user_htim1);

		while (!ws2812_finished_dma) {
			vTaskDelay(5);
		}

	}
}

void tareas_error_handler(uint8_t error) {
	while (1) {
		UNUSED(error);
	}

}
