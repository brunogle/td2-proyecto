#include "reed/reed.h"

#include "main.h"
#include "util.h"


void reed_scan_sensors(uint8_t data[8]){
  for (int row = 0; row < 8; row++) {
	HAL_GPIO_WritePin(REED_1_GPIO_Port, REED_1_Pin, row == 0);
	HAL_GPIO_WritePin(REED_2_GPIO_Port, REED_2_Pin, row == 1);
	HAL_GPIO_WritePin(REED_3_GPIO_Port, REED_3_Pin, row == 2);
	HAL_GPIO_WritePin(REED_4_GPIO_Port, REED_4_Pin, row == 3);
	HAL_GPIO_WritePin(REED_5_GPIO_Port, REED_5_Pin, row == 4);
	HAL_GPIO_WritePin(REED_6_GPIO_Port, REED_6_Pin, row == 5);
	HAL_GPIO_WritePin(REED_7_GPIO_Port, REED_7_Pin, row == 6);
	HAL_GPIO_WritePin(REED_8_GPIO_Port, REED_8_Pin, row == 7);

	for(int i = 0; i < 10; i++){ } //Delay chico para dejar que la señal se propage


    data[row] = 0;
	data[row] |= (HAL_GPIO_ReadPin(REED_A_GPIO_Port, REED_A_Pin) != 0);
	data[row] |= (HAL_GPIO_ReadPin(REED_B_GPIO_Port, REED_B_Pin) != 0) << 1;
	data[row] |= (HAL_GPIO_ReadPin(REED_C_GPIO_Port, REED_C_Pin) != 0) << 2;
	data[row] |= (HAL_GPIO_ReadPin(REED_D_GPIO_Port, REED_D_Pin) != 0) << 3;
	data[row] |= (HAL_GPIO_ReadPin(REED_E_GPIO_Port, REED_E_Pin) != 0) << 4;
	data[row] |= (HAL_GPIO_ReadPin(REED_F_GPIO_Port, REED_F_Pin) != 0) << 5;
	data[row] |= (HAL_GPIO_ReadPin(REED_G_GPIO_Port, REED_G_Pin) != 0) << 6;
	data[row] |= (HAL_GPIO_ReadPin(REED_H_GPIO_Port, REED_H_Pin) != 0) << 7;

  }
}

/*
void reed_scan_sensors(uint8_t * data){
	for(int row = 0; row < 8; row++){
		HAL_GPIO_WritePin(REED_1_GPIO_Port, REED_1_Pin, row == 0);
		HAL_GPIO_WritePin(REED_2_GPIO_Port, REED_2_Pin, row == 1);
		HAL_GPIO_WritePin(REED_3_GPIO_Port, REED_3_Pin, row == 2);
		HAL_GPIO_WritePin(REED_4_GPIO_Port, REED_4_Pin, row == 3);
		HAL_GPIO_WritePin(REED_5_GPIO_Port, REED_5_Pin, row == 4);
		HAL_GPIO_WritePin(REED_6_GPIO_Port, REED_6_Pin, row == 5);
		HAL_GPIO_WritePin(REED_7_GPIO_Port, REED_7_Pin, row == 6);
		HAL_GPIO_WritePin(REED_8_GPIO_Port, REED_8_Pin, row == 7);

		for(int i = 0; i < 10; i++){ } //Delay chico para dejar que la señal se propage

		data[8*row + 0] = HAL_GPIO_ReadPin(REED_A_GPIO_Port, REED_A_Pin) != 0;
		data[8*row + 1] = HAL_GPIO_ReadPin(REED_B_GPIO_Port, REED_B_Pin) != 0;
		data[8*row + 2] = HAL_GPIO_ReadPin(REED_C_GPIO_Port, REED_C_Pin) != 0;
		data[8*row + 3] = HAL_GPIO_ReadPin(REED_D_GPIO_Port, REED_D_Pin) != 0;
		data[8*row + 4] = HAL_GPIO_ReadPin(REED_E_GPIO_Port, REED_E_Pin) != 0;
		data[8*row + 5] = HAL_GPIO_ReadPin(REED_F_GPIO_Port, REED_F_Pin) != 0;
		data[8*row + 6] = HAL_GPIO_ReadPin(REED_G_GPIO_Port, REED_G_Pin) != 0;
		data[8*row + 7] = HAL_GPIO_ReadPin(REED_H_GPIO_Port, REED_H_Pin) != 0;
	}
	HAL_GPIO_WritePin(REED_8_GPIO_Port, REED_8_Pin, 0);
}
*/
