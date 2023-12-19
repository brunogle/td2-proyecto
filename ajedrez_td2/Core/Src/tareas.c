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

typedef enum ME_general_estados_td {
	ME_GENERAL_RESET,
	ME_GENERAL_SELECCION_MODO,
	ME_GENERAL_SELECCION_TIEMPO,
	ME_GENERAL_JVJ,
	ME_GENERAL_JVM,
	ME_GENERAL_FIN
} ME_general_estados;

static void ME_general(uint8_t button);
ME_general_estados estado = ME_GENERAL_RESET;
uint8_t modo_de_juego = 0, mostrar_tiempo = 0, contar = 0, actualizar_display =
		0;

uint32_t segundos_j1 = 0, segundos_j2 = 0, incremento = 0;

void t_Timer() {
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = 1000; //1seg
	while (1) {
		if (!get_error_position() && modo_de_juego == 1) {
			if (get_side_to_move() == 0) {
				if (segundos_j1 > 0)
					segundos_j1--;
			}

			else {
				if (segundos_j2 > 0)
					segundos_j2--;
			}

		}

		actualizar_display = 1;

		vTaskDelayUntil(&xLastWakeTime, xPeriod);
	}
}

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

		if (debounce_flank(&b1)) { // Boton arriba (no contra el tablero)
			msg = 1;
			if (uxQueueMessagesWaiting(buttons_queue) < 20) {
				xQueueSend(buttons_queue, (void* )&msg, portMAX_DELAY);
			}
		}

		if (debounce_flank(&b2)) { // Boton abajo (contra el tablero)
			msg = 2;
			if (uxQueueMessagesWaiting(buttons_queue) < 20) {
				xQueueSend(buttons_queue, (void* )&msg, portMAX_DELAY);
			}
		}

		if (debounce_flank(&b3)) { // Boton verde
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

	if (lcd_queue == NULL) {
		tareas_error_handler(1);
	}

	user_htim1 = &htim1;
	user_init();

	ws2812_init();

	uint8_t button = 0;

	TickType_t xLastWakeTime;
	const TickType_t xPeriod = 33; // 30fps aprox

	LCDQueueItem_t msg = { 0 };

	char msg_tiempo[] = "-> JX - XX:XX    ";

	uint8_t prev_side_to_move = 0;

	xQueueSend(buttons_queue, &button, 0);

	while (1) {
		if (xQueueReceive(buttons_queue, &button, 0) == pdTRUE) {
			ME_general(button);
		}

		switch (modo_de_juego) {
		case 0:
			break;
		case 1:
			set_cpu_player(0);
			user_loop(); // FSM CHESS

			// Detecto si hubo un cambio de turno para sumar incremento de tiempo
			if (prev_side_to_move != get_side_to_move()) {
				prev_side_to_move = get_side_to_move();
				if (prev_side_to_move)
					segundos_j1 += incremento;
				else
					segundos_j2 += incremento;
			}

			//Imprimo tiempos si tengo que hacerlo
			if (actualizar_display && mostrar_tiempo) {
				actualizar_display = 0;
				msg = lcd_msg_first_line();
				xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);

				if (get_side_to_move() == 0) {
					msg_tiempo[0] = '-';
					msg_tiempo[1] = '>';
				} else {
					msg_tiempo[0] = ' ';
					msg_tiempo[1] = ' ';
				}

				// char * msg_tiempo = "JX - XX:XX";
				msg_tiempo[4] = '1';
				msg_tiempo[8] = ((segundos_j1 / 60) / 10) + 48;
				msg_tiempo[9] = ((segundos_j1 / 60) % 10) + 48;
				msg_tiempo[11] = ((segundos_j1 % 60) / 10) + 48;
				msg_tiempo[12] = ((segundos_j1 % 60) % 10) + 48;

				msg = lcd_msg_from_string(msg_tiempo);
				xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
				msg = lcd_msg_second_line();
				xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);

				if (get_side_to_move() == 1) {
					msg_tiempo[0] = '-';
					msg_tiempo[1] = '>';
				} else {
					msg_tiempo[0] = ' ';
					msg_tiempo[1] = ' ';
				}

				msg_tiempo[4] = '2';
				msg_tiempo[8] = ((segundos_j2 / 60) / 10) + 48;
				msg_tiempo[9] = ((segundos_j2 / 60) % 10) + 48;
				msg_tiempo[11] = ((segundos_j2 % 60) / 10) + 48;
				msg_tiempo[12] = ((segundos_j2 % 60) % 10) + 48;

				msg = lcd_msg_from_string(msg_tiempo);
				xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
			}
			if (get_finished_state()) {
				modo_de_juego = 3;
				button = 4;
				xQueueSend(buttons_queue, &button, 0);
			}

			break;
		case 2:
			user_loop();
			if (get_finished_state()) {
				modo_de_juego = 3;
				button = 4;
				xQueueSend(buttons_queue, &button, 0);
			}

			break;
		case 3:
			break;
		}

		xSemaphoreGive(ws2812_sem);
		vTaskDelay(33);
	}
}

static void ME_general(uint8_t button) {
	static int8_t sub_estado = 0;
	LCDQueueItem_t msg = { 0 };

	uint8_t aux_button = 0;

	switch (estado) {
	case ME_GENERAL_RESET:
		modo_de_juego = 0;
		msg = lcd_msg_clear();
		xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
		msg = lcd_msg_first_line();
		xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
		msg = lcd_msg_from_string("Smart Chess");
		xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
		msg = lcd_msg_second_line();
		xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
		msg = lcd_msg_from_string("Btn Verde-Start");
		xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);

		if (button == 3) {
			estado = ME_GENERAL_SELECCION_MODO;
			xQueueSend(buttons_queue, &aux_button, 0); // Actualizo ME
		}

		break;
	case ME_GENERAL_SELECCION_MODO:
		msg = lcd_msg_clear();
		xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
		msg = lcd_msg_first_line();
		xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
		msg = lcd_msg_from_string("Selec. Modo:");
		xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
		msg = lcd_msg_second_line();
		xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
		if (sub_estado == 0) {
			msg = lcd_msg_from_string("-> Jug vs Jug");
			xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
		}
		if (sub_estado == 1) {
			msg = lcd_msg_from_string("-> Jug vs Maq");
			xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
		}

		if (button == 3) {
			estado =
					sub_estado == 0 ?
							ME_GENERAL_SELECCION_TIEMPO : ME_GENERAL_JVM;
			sub_estado = 0;
			xQueueSend(buttons_queue, &aux_button, 0); // Actualizo ME
		} else if (button != 0) {
			sub_estado = !sub_estado;
			xQueueSend(buttons_queue, &aux_button, 0); // Actualizo ME
		}
		break;

	case ME_GENERAL_SELECCION_TIEMPO:
		msg = lcd_msg_clear();
		xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
		msg = lcd_msg_first_line();
		xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
		msg = lcd_msg_from_string("Selec. Tiempo:");
		xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
		msg = lcd_msg_second_line();
		xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);

		switch (sub_estado) {
		case 0:
			msg = lcd_msg_from_string("5min S/INC");
			break;
		case 1:
			msg = lcd_msg_from_string("5min +30seg");
			break;
		case 2:
			msg = lcd_msg_from_string("10min S/INC");
			break;
		case 3:
			msg = lcd_msg_from_string("10min +1min");
			break;
		case 4:
			msg = lcd_msg_from_string("30min S/INC");
			break;
		case 5:
			msg = lcd_msg_from_string("30min +3min");
			break;
		}

		xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);

		if (button == 1) {
			sub_estado--;
			if (sub_estado < 0)
				sub_estado = 5;

			xQueueSend(buttons_queue, &aux_button, 0); // Actualizo ME
		} else if (button == 2) {
			sub_estado++;
			if (sub_estado > 5)
				sub_estado = 0;

			xQueueSend(buttons_queue, &aux_button, 0); // Actualizo ME
		} else if (button == 3) {
			estado = ME_GENERAL_JVJ;
			switch (sub_estado) {
			case 0:
				segundos_j1 = 300;
				segundos_j2 = 300;
				incremento = 0;
				break;
			case 1:
				segundos_j1 = 300;
				segundos_j2 = 300;
				incremento = 30;
				break;
			case 2:
				segundos_j1 = 600;
				segundos_j2 = 600;
				incremento = 0;
				break;
			case 3:
				segundos_j1 = 600;
				segundos_j2 = 600;
				incremento = 60;
				break;
			case 4:
				segundos_j1 = 1800;
				segundos_j2 = 1800;
				incremento = 0;
				break;
			case 5:
				segundos_j1 = 1800;
				segundos_j2 = 1800;
				incremento = 180;
				break;
			}
			sub_estado = 0;
			mostrar_tiempo = 1;
			xQueueSend(buttons_queue, &aux_button, 0); // Actualizo ME
		}
		break;

	case ME_GENERAL_JVJ:
		modo_de_juego = 1;
		set_cpu_player(0);

		if (sub_estado == 0) {
			msg = lcd_msg_clear();
			xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
			msg = lcd_msg_first_line();
			xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
			msg = lcd_msg_from_string("Ver Tiempos");
			xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
			msg = lcd_msg_second_line();
			xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
			msg = lcd_msg_from_string("Boton Verde");
			xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
		} else {
			mostrar_tiempo = 0;
			msg = lcd_msg_clear();
			xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
			msg = lcd_msg_first_line();
			xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
			msg = lcd_msg_from_string("Modo Reset");
			xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
			msg = lcd_msg_second_line();
			xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
			msg = lcd_msg_from_string("Boton Verde");
			xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
		}

		if (button == 3) {
			if (sub_estado == 1) {
				user_init();
				estado = ME_GENERAL_RESET;
			} else {
				mostrar_tiempo = 1;

			}

			xQueueSend(buttons_queue, &aux_button, 0); // Actualizo ME
		} else if (button != 0) {
			sub_estado = !sub_estado;

			xQueueSend(buttons_queue, &aux_button, 0); // Actualizo ME
		}

		if (button == 4) {
			estado = ME_GENERAL_FIN;
			xQueueSend(buttons_queue, &aux_button, 0); // Actualizo ME
		}
		break;
	case ME_GENERAL_JVM:
		set_cpu_player(1);
		modo_de_juego = 2;

		msg = lcd_msg_clear();
		xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
		msg = lcd_msg_first_line();
		xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
		msg = lcd_msg_from_string("Jug vs Maq");
		xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
		msg = lcd_msg_second_line();
		xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
		msg = lcd_msg_from_string("Btn Verde-Rst");
		xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);

		if (button == 3) {
			user_init();
			estado = ME_GENERAL_RESET;

			xQueueSend(buttons_queue, &aux_button, 0); // Actualizo ME
		}

		if (button == 4) {
			estado = ME_GENERAL_FIN;
			xQueueSend(buttons_queue, &aux_button, 0); // Actualizo ME
		}
		break;
	case ME_GENERAL_FIN:
		modo_de_juego = 3;

		msg = lcd_msg_clear();
		xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
		msg = lcd_msg_first_line();
		xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
		msg = lcd_msg_from_string("Juego Terminado");
		xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
		msg = lcd_msg_second_line();
		xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);
		msg = lcd_msg_from_string("Btn Verde-Start");
		xQueueSend(lcd_queue, (void* )&msg, portMAX_DELAY);

		if (button == 3) {
			user_init();
			estado = ME_GENERAL_RESET;

			xQueueSend(buttons_queue, &aux_button, 0); // Actualizo ME
		}
		break;
	}
}

void t_WS2812(void*) {

	if (ws2812_sem == NULL) {
		tareas_error_handler(2);
	}

	ws2812_init();

	while (1) {
		xSemaphoreTake(ws2812_sem, portMAX_DELAY);
		ws2812_update_leds_from_data(user_htim1);
		while (!ws2812_finished_dma) {
			vTaskDelay(5);
		}

	}
}

void tareas_error_handler(void *error) {
	__disable_irq();
	while (1) {
		UNUSED(error);
	}

}
