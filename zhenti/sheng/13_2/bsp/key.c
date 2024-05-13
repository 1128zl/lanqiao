#include "key.h"
#include "led.h"

struct key keys[4]={0,0,0,0};

//定义外部变量
extern uint8_t is_buy,is_enputy,led2_sta,stock_x,stock_y;
extern uint16_t time_count,time_count1;
extern TIM_HandleTypeDef htim4,htim3,htim2,htim16;

//回调函数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM4)
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
							keys[i].key_pro=0;
							keys[i].key_flg=1;
						}
					}
					break;
			}
		}
	}
	
	//定时器定时0.1s
	if(htim->Instance==TIM3)
	{
		if(is_buy==1 && is_enputy==1)
			{
				time_count++;
				Close_Led();
				led2_sta=led2_sta^1;
				Change_One_led(0x02,led2_sta);
				if(time_count==50)
				{
					Close_Led();
					Change_One_led(0x01,1);
					is_buy=0;
				}
			}
		else if(is_buy==1 && is_enputy==0)
		{
			time_count++;
			if(time_count==50)
			{
				Close_Led();
				Change_One_led(0x01,1);
				is_buy=0;
				HAL_TIM_Base_Stop(&htim3);
			}
		}
		else if(is_buy==0 && is_enputy==1)
		{
			Close_Led();
			led2_sta=led2_sta^1;
			Change_One_led(0x02,led2_sta);
		}
		else if(is_enputy==0 && is_buy==0)
		{
			Close_Led();
			HAL_TIM_Base_Stop(&htim3);
		}
	}
	

}