#ifndef _KEY_H_
#define _KEY_H_

#include "main.h"

//������ؼ�¼�Ľṹ��
struct key{
	uint8_t key_sta;
	uint8_t key_pro;
	uint8_t key_flg;
};

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#endif