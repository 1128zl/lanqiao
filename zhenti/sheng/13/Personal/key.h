#ifndef _KEY_H_
#define _KEY_H_

#include "main.h"

//����ṹ���¼�ĸ�������״̬
struct Key {

	int key_state;//������Ӧ�����ŵĵ�ƽ���
	int key_flag;//���������µı�־	1--������ 0--δ������
	int key_process;//��¼��ʱ�����жϰ����Ƿ��µ��ĸ�����
};
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void Key_Function();
#endif