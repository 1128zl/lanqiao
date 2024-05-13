#include "key.h"
#include "lcd.h"
#include "main.h"
#include "stdio.h"
#include "led.h"
#include "myfile.h"
#include "tim.h"
#include "hadc.h"
#include "adc.h"

struct key_infrom keys[4]={0,0,0,0};

extern uint signal;
extern uint view_chart;
extern char text[30];
extern uint menu_mode;
extern uint pa6_duty;
extern uint pa7_duty;
extern char mode[2][30];
//回调函数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM4)
	{
		keys[0].key_state=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0);
		keys[1].key_state=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1);
		keys[2].key_state=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2);
		keys[3].key_state=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
		
		int i;
		
		for(i=0;i<4;i++)
		{
			
			switch(keys[i].key_tip)
				{
					case 0:
						{
								if(keys[i].key_state==0)
									{
											keys[i].key_tip=1;
									}
						}
						break;
					
					case 1:
						{
								
								if(keys[i].key_state==0)
									{
											
											keys[i].key_tip=2;
									}
								else
									{
											keys[i].key_tip=0;
									}
						}
						break;
						
					case 2:
						{
									if(keys[i].key_state==1)
									{
										keys[i].key_tip=0;
										keys[i].key_flag=1;
										signal=i;
										//若是切换显示界面，则将界面的标志进行改变
										if(signal==0)
											view_chart=!view_chart;
										
										//若是切换模式，则将模式的标志进行改变
										if(signal==3)
											menu_mode=!menu_mode;
									}
						}
						break;
				}
		}
	}

}

void Key_Contr(uint code,uint view_chart,uint menu_mode)
{

	
	switch(code)
	{
		//显示界面切换
		case 0:
			break;
			//手动增加PA6占空比
		case 1:
			{
				if(view_chart==1 && menu_mode==1)
					{
						pa6_duty+=10;
						if(pa6_duty==100)
							pa6_duty=10;
					}
					signal=-1;
				
			}
			break;
		
		//手动增加PA7占空比
		case 2:
			{
					if(view_chart==1 && menu_mode==1)
					{
						pa7_duty+=10;
						if(pa7_duty==100)
							pa7_duty=10;
					}
					signal=-1;
			}
			break;
		//运行模式切换
		case 3:
			{
				signal=-1;
			}
			break;
	
	}

}