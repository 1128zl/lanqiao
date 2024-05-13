#ifndef _KEY_H_
#define _KEY_H_

#include "main.h"

struct key{
	uint8_t key_state;
	uint8_t key_process;
	uint8_t key_flag;
};

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#endif
