/*
 * user_main.h
 *
 *  Created on: Nov 6, 2023
 *      Author: bruno
 */

#ifndef INC_USER_MAIN_H_
#define INC_USER_MAIN_H_

#include "main.h"
#include "engine/moves.h"

// Handle copies from main.c

extern TIM_HandleTypeDef * user_htim1;



void user_init();
piece_t get_promotion();
void user_loop();
uint8_t get_side_to_move();
uint8_t get_error_position();
void set_cpu_player(uint8_t on);
uint8_t get_finished_state();

#endif /* INC_USER_MAIN_H_ */
