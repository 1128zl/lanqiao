#include "key.h"
#include "display.h"

struct key keys[4]={0,0,0,0};
extern uint8_t led1_sta,is_open,h,m,s;
extern float k,volt;
extern TIM_HandleTypeDef htim2,htim3;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM2)
	{
		keys[0].key_sta=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0);
		keys[1].key_sta=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1);
		keys[2].key_sta=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2);
		keys[3].key_sta=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
		
		for(int i=0;i<4;i++)
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
		if(volt>(3.3*k))
		{
			led1_sta^=1;
			Invoid_Complict();
			Control_Single_Led(0x01,led1_sta);
		}
		else
		{
			Control_Single_Led(0x01,1);
			HAL_TIM_Base_Stop(&htim3);
		}
	}
}