#include "key.h"
#include "display.h"

//定义外部变量
extern float v_max,v_min,v_37;
extern unsigned char led_upper,led_lower,led_sta;

struct key keys[4]={0,0,0,0};

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){

	if(htim->Instance==TIM2)
	{
		keys[0].key_sta=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0);
		keys[1].key_sta=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1);
		keys[2].key_sta=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2);
		keys[3].key_sta=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
	
	
		for(uint8_t i=0;i<4;i++)
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
	
	//LED灯间隔闪烁
	if(htim->Instance==TIM3)
	{
		if(v_37>v_max)
		{
			led_sta^=1;
			Invoid_Complict();
			Control_Single_Led(led_upper,led_sta);
		}
		else if(v_37<v_min)
		{
			led_sta^=1;
			Invoid_Complict();
			Control_Single_Led(led_lower,led_sta);
		}
	}
}