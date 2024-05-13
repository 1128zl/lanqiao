#include "key.h"
#include "led.h"
#include "main.h"
#include "lcd.h"
#include "stdio.h"
//定义按键对应的变量
struct Key keys[4]={0,0,0,0};

extern int8_t code_1,code_2,code_3,origin_code,code_correct,error_counts;
extern uint8_t menu;
extern void Lcd_Display_State();
extern void Lcd_Display_Code();
extern TIM_HandleTypeDef htim2,htim3,htim4;
extern uint8_t counts;
extern unsigned char sta;

int correct_flag=0;
//定时器回调函数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	//和按键相关的定时器
	if(htim->Instance==TIM4)
		{
			//获取当前判断按键是否被按下所处的过程
			keys[0].key_state=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0);
			keys[1].key_state=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1);
			keys[2].key_state=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2);
			keys[3].key_state=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
			
			//进一步判断
			int i;
			for(i=0;i<4;i++)
			{
//				//判断前处理标志量，以免错误判断
//				keys[i].key_flag=0;
				switch(keys[i].key_process)
				{
					case 0:
						{
							if(keys[i].key_state==0)
								{
									keys[i].key_process=1;
								}
						}
						break;
						
					case 1:
						{
								if(keys[i].key_state==0)
									keys[i].key_process=2;
								else
									keys[i].key_process=0;
						}
						break;
						
					case 2:
						{
								if(keys[i].key_state==1)
								{
									keys[i].key_flag=1;
									keys[i].key_process=0;
								}
						}
						break;
				
				}
			
			}
		}

	//定时器定时五秒钟
	if(htim->Instance==TIM3)
	{
			if(code_correct==1)
			{
				Close_All_LED();
				
				code_correct=0;
			}
			
	}
	
//	//定时器定时0.1秒切换LED2亮灭情况
	if(htim->Instance==TIM2)
	{
			if(error_counts>=3)
			{
				counts++;
				sta=sta^1;
				Change_One_LED(0x02,sta);
				if(counts==25)
				{
				  Close_All_LED();
					HAL_TIM_Base_Stop(&htim2);
				}
			}
	}
	
	
}

/*******************************************************************************
* 函数名  : Key_Function
* 功能    : 针对于按下的键来实现对应的功能
* 输入    : 无
* 返回值  : 无
*******************************************************************************/

void Key_Function()
{
	//按下B1键（第一个按键）第一位密码+1，每位密码的范围是0-9
	if(keys[0].key_flag)
		{
			keys[0].key_flag=0;
			if(menu==0)
			{
			//修改密码第一位
			code_1++;
			if(code_1==10)
				code_1=0;
			}
			
		}
	//按下B2键（第二个按键）第二位密码+1，每位密码的范围是0-9
	if(keys[1].key_flag)
		{
			keys[1].key_flag=0;
			
			if(menu==0)
			{
			//修改密码第二位
			code_2++;
			if(code_2==10)
				code_2=0;
			}
		
		}
	//按下B3键（第三个按键）第三位密码+1，每位密码的范围是0-9
	if(keys[2].key_flag)
		{
			keys[2].key_flag=0;
			if(menu==0)
			{
			//修改密码第三位
			code_3++;
			if(code_3==10)
				code_3=0;
			}
		
		}
	//按下B4键（第四个按键）则确认输入的密码，进行下一步的判断
	if(keys[3].key_flag)
		{
			keys[3].key_flag=0;
			
			//判断密码是否输入正确

			if((code_1*100+code_2*10+code_3) == origin_code)
			{
				//显示输出状态界面
				menu=1;
				code_correct=1;
				error_counts=0;
				
			}
			else
			{
				//记录密码连续输入错误次数
				error_counts++;
				code_correct=-1;

					//显示密码输入界面
				code_1=code_2=code_3=-1;
			
			}
			Led_Disp();
		}
}