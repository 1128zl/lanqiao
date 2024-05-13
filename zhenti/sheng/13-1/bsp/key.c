#include "key.h"
#include "led.h"

struct key keys[4]={0,0,0,0};

extern uint16_t count,freq;
extern uint8_t is_correct,error_counts,duty;
extern TIM_HandleTypeDef htim2,htim3;
extern unsigned char led2_sta;

/**************************��ʱ���ص�����******************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM4)
	{
		keys[0].key_sta=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0);
		keys[1].key_sta=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1);
		keys[2].key_sta=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2);
		keys[3].key_sta=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
		
		for(unsigned char i=0;i<4;i++)
		{
			switch(keys[i].key_pro)
			{
				case 0:
					{
						if(keys[i].key_sta==0)
							keys[i].key_pro=1;
					}
					break;
				
				case 1:
					{
						if(keys[i].key_sta==0)
							keys[i].key_pro=2;
						else
							keys[i].key_pro=0;
					}
					break;
				
				case 2:
					{
						if(keys[i].key_sta==1)
							{
								keys[i].key_flg=1;
								keys[i].key_pro=0;
							}
						
					}
					break;	
			}
		}
	}
	
	if(htim->Instance==TIM3)
	{
		count++;
		//����������ȷ��LED1����5s��Ϩ��
		//�ı�PWM�����Ƶ�ʣ�ռ�ձ�
		if(is_correct==1 && count==50)
			{
				HAL_GPIO_WritePin(GPIOC,0xff00,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
				Contro_One_Led(0x01,1);
				htim2.Init.Prescaler = 80-1;
				htim2.Init.Period = 999;
				__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,500);
				duty=50;
				freq=1000;
				HAL_TIM_Base_Start_IT(&htim3);
				HAL_TIM_Base_Stop(&htim3);
			}
			
		//�����������������������ϣ�LED2��5s��ÿ��0.1s�л�״̬
		if(is_correct==0 && error_counts>=3)
		{
			if(count!=50)
			{
				//����Ϊ�˱���LED��LCD������ͻ��������LED����ǰ�����˴���
				//ǰ��ĸ��������������ᵽ����Ҫ�Ļ�����ȥ����
				HAL_GPIO_WritePin(GPIOC,0xff00,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
				led2_sta=led2_sta^1;
				Contro_One_Led(0x02,led2_sta);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOC,0xff00,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
				Contro_One_Led(0x02,1);
				
				//������5s֮��رն�ʱ�����Է������ʱ����
				HAL_TIM_Base_Stop(&htim3);
			}
		}
	}
}