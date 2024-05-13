#include "key.h"
#include "myfile.h"
#include "display.h"

extern TIM_HandleTypeDef htim2,htim3,htim4;

struct key keys[4]={0,0,0,0};
extern uint16_t count_tim;
extern uint8_t is_change_mode,led2_sta;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	//与按键一起使用
	if(htim->Instance==TIM3)
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
						{
							keys[i].key_pro=1;
							keys[i].key_tim=0;
					}
				break;
				
				case 1:
					{
						if(keys[i].key_sta==0)
						{
							keys[i].key_pro=2;
							keys[i].key_tim++;
						}
						else
							keys[i].key_pro=0;
					}
				break;
				
				case 2:
					{
						if(keys[i].key_sta==1)
						{
							keys[i].key_pro=0;
							if(keys[i].key_tim<=70)
								keys[i].key_sflg=1;
						}
						else
						{
							keys[i].key_tim++;
							if(keys[i].key_tim>70)
								keys[i].key_lflg=1;
						}
					}
				break;
				
				}
			}
		}
	}
	
	if(htim->Instance==TIM4)
	{
		if(is_change_mode==0)
		{
			count_tim++;
			Change_Freq();
			Display_Led();
			if(count_tim==50)
				is_change_mode=1;
		}
	}
}