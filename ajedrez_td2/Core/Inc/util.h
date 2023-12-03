/*
 * util.h
 *
 *  Created on: Nov 11, 2023
 *      Author: Bruno
 */

#ifndef INC_UTIL_H_
#define INC_UTIL_H_

#include "stm32f4xx_hal.h"


/**
 * @brief  This function provides a delay (in microseconds)
 * @param  microseconds: delay in microseconds
 */
__STATIC_INLINE void DWT_Delay_us(volatile uint32_t microseconds)
{
  DWT->CTRL|= DWT_CTRL_CYCCNTENA_Msk;

  DWT->CYCCNT = 0;

  /* Go to number of cycles for system */
  microseconds *= (HAL_RCC_GetHCLKFreq() / 1000000);

  /* Delay till end */
  while (DWT->CYCCNT < microseconds);
}


#endif /* INC_UTIL_H_ */
