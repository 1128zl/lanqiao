#ifndef _KEY_H_
#define _KEY_H_

#include "main.h"
#include "stdbool.h"
#define uchar unsigned char
#define uint unsigned int

struct key_infrom{
		uchar key_state;	//�����˿��Ƿ��Ǳ�����
		bool key_flag;		//ȷ�������Ǳ����µı�־��������������
		uint key_tip;			//���ھ��������жϹ��̵���һ��
};
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void Key_Contr(uint code,uint view_chart,uint mode);
#endif
