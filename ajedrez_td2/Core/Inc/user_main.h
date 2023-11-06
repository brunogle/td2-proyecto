/*
 * user_main.h
 *
 *  Created on: Nov 6, 2023
 *      Author: bruno
 */

#ifndef INC_USER_MAIN_H_
#define INC_USER_MAIN_H_


// Handle copies from main.c

extern TIM_HandleTypeDef * user_htim1;



void user_init();

void user_loop();


#endif /* INC_USER_MAIN_H_ */
