#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include "main.h"
#include "stdbool.h"
struct keys
{
	uchar judge_sta;//�жϰ������µĶ������е�����һ��
	bool key_sta;	//������������£�Ϊ0
	bool single_flag;//�������--���ȷ�ϱ����£�Ϊ1
	uint key_time;	//�����������µ�ʱ��
	bool double_key_flag;
	bool long_flag;	//������Ӧ��־
	uint pwm_duty;
};

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
#endif