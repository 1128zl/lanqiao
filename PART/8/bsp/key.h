#ifndef _KEY_H_
#define _KEY_H_


#include "main.h"
#include "stdbool.h"
//�����ṹ��
struct key{
	unsigned int state;		//��ǰ�����ĵ�ƽ���
	bool flag;		//�����Ƿ񱻰���	0--δ����		1--������
	unsigned int step;	//�жϵ�ǰ�����жϰ��������µ���һ��
};
//�����ص�����
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void Lcd_Disp(unsigned signal);
#endif