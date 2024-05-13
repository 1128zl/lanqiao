#include "led.h"

/*******************************************************************************
* ������  : Led_Display
* ����    : ��������ģʽ�ֱ��Ӧ��LED�������
* ����    : mode---��ǰ����ģʽ 0---�Զ�ģʽ	1---�ֶ�ģʽ	
				  :view_chart---��ǰ��ʾ���� 0---���ݽ���	1---��������
* ����ֵ  : ��
*******************************************************************************/



void Led_Display (unsigned int mode,unsigned int view_chart)
{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_All,GPIO_PIN_SET);
		if(mode==0)
		{
			HAL_GPIO_WritePin(GPIOC,0x01<<8,GPIO_PIN_RESET);	
		}
		else
			HAL_GPIO_WritePin(GPIOC,0x01<<8,GPIO_PIN_SET);
		if(view_chart==0)
			HAL_GPIO_WritePin(GPIOC,0x02<<8,GPIO_PIN_RESET);
		else
			HAL_GPIO_WritePin(GPIOC,0x02<<8,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}

	
	