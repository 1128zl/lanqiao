#include "key.h"
#include "myfile.h"
#include "lcd.h"
#include "stdio.h"


extern TIM_HandleTypeDef htim2,htim3,htim4,htim5,htim6,htim7,htim16,htim17;
extern uint8_t plat_pro,platindex;
extern uint8_t plat_now,plat_future,plat_pro;
extern int8_t isup_or_down;
struct key keys[4]={0,0,0,0};
uint8_t count=0;

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
					if(keys[i].key_sta==0)
						keys[i].key_pro=1;
					break;
				
				case 1:
					if(keys[i].key_sta==0)
						keys[i].key_pro=2;
					else
						keys[i].key_pro=0;
					break;
					
				case 2:
					if(keys[i].key_sta==1)
					{
						keys[i].key_pro=0;
						keys[i].key_flg=1;
					}
					break;
			}
		}
	}
	
	if(htim->Instance==TIM3)
	{
		
		if(plat_pro%4==0)
		{
			count++;
			
			if(platindex!=0 && count>=10 && keys[0].key_flg!=1 &&keys[1].key_flg!=1 &&keys[2].key_flg!=1 &&keys[3].key_flg!=1)
			{
				count=0;
				plat_pro++;
				HAL_TIM_Base_Stop(&htim3);
			}
			else if(!(keys[0].key_flg!=1 &&keys[1].key_flg!=1 &&keys[2].key_flg!=1 &&keys[3].key_flg!=1))
			{
				HAL_TIM_Base_Stop(&htim3);
			}

		}
	}
	
	//运行到目标站台6秒
	if(htim->Instance==TIM4)
	{
		__HAL_TIM_SetCompare(&htim16,TIM_CHANNEL_1,1000);
		plat_now=plat_future;
		plat_pro++;
		isup_or_down=0;
		HAL_TIM_Base_Stop(&htim4);
	}
	
	//升降门关门4秒
	if(htim->Instance==TIM6)
	{
		if(plat_pro%4==1)//关门
		{
			__HAL_TIM_SetCompare(&htim17,TIM_CHANNEL_1,1000);
			plat_pro++;
			HAL_TIM_Base_Stop(&htim6);
		}
		else if(plat_pro%4==3)	//开门
		{
			__HAL_TIM_SetCompare(&htim17,TIM_CHANNEL_1,1000);
			plat_pro++;
			HAL_TIM_Base_Stop(&htim6);
			HAL_TIM_Base_Start_IT(&htim7);	//延时2秒，判断是否还有目标平台
		}
	}
	
	if(htim->Instance==TIM7)
	{
		plat_pro++;
		HAL_TIM_Base_Stop(&htim7);
	}
}