#include "led.h"

//��������LED�Ƶ�����
void Control_All_Led()
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_8 
                          |GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_SET);
	 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
	 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
}

//���Ƶ���LED�Ƶ�����
void Control_Single_Led(unsigned char location,unsigned char state)
{
	 HAL_GPIO_WritePin(GPIOC, (location<<8), state);
	 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
	 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);

}

//Ϊ�˱���LED��LCD��ʾ��ͻ���еĴ���
void Invoid_Complict()
{
		HAL_GPIO_WritePin(GPIOC,0xff00,GPIO_PIN_SET);
	 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
	 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
}