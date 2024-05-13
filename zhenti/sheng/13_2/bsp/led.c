#include "led.h"

extern uint16_t time_count;
extern uint8_t led2_sta,is_enputy,is_buy,is_enputy,led2_sta;
extern uint8_t stock_x,stock_y;					//��¼������Ʒ�Ŀ����
extern TIM_HandleTypeDef htim4,htim3,htim2,htim16;

//��������LED�������
void Contr_All_led()
{
	 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_8 
                          |GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_SET);
	 HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	 HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}

//���Ƶ���LED��
void Change_One_led(unsigned char location,unsigned char state)
{
	HAL_GPIO_WritePin(GPIOC,location<<8,state);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}

//����LCD��LED��ʾ��ͻ
void Close_Led()
{
	HAL_GPIO_WritePin(GPIOC,0xff00,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}

void Display_Led()
{
	if(is_buy==1)
	{
		time_count=0;
		HAL_GPIO_WritePin(GPIOC,0xff00,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
		Change_One_led(0x01,0);
		//pwm����仯
		__HAL_TIM_SetCompare(&htim16,TIM_CHANNEL_1,300);
		
		if(stock_x==0 && stock_y==0)
		{
			led2_sta=0;
			Change_One_led(0x02,led2_sta);
			is_enputy=1;
		}
		HAL_TIM_Base_Start_IT(&htim3);
	}
	
	
}