#include "myfile.h"
#include "lcd.h"
#include "key.h"
#include "display.h"

//定义外部变量
extern TIM_HandleTypeDef htim2,htim3,htim4,htim17;
extern ADC_HandleTypeDef hadc2;
extern struct key keys[4];
extern uint8_t menu,mode,r,k,n,status,change_para,change_flag,is_change_mode,led2_sta;
extern float speed_high_max,speed_low_max,speed_l,speed_h,duty;
extern uint16_t freq_l,freq_h,count_tim,freq;

/*************************ADC*********************************/
float Get_ADC()
{
	HAL_ADC_Start(&hadc2);
	int value;
	value=HAL_ADC_GetValue(&hadc2);
	HAL_ADC_Stop(&hadc2);
	return value*3.3/4096;
}

//电位器调节pwm输出信号占空比
void Pwm_Duty_Change()
{
	if(status)	//可以解锁
	{
		float volt=Get_ADC();
		if(volt<=1.0)
		{
			duty=10;
			__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,100);
		}
		else if(volt>=3.0)
		{
			duty=85;
			__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,850);
		}
		else
		{
			duty=(0.375*volt-0.275)*100;
			__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,(0.375*volt-0.275)*1000);
		}
	}
}
/********************************END******************************************/


//PA1输出信号的频率切换期间频率的均匀变化
void Change_Freq()
{
	if(mode==1)	//从高频到低频
	{
		if(freq!=4000)
		{
			freq=freq-100;	//每次降低100HZ
			__HAL_TIM_SetAutoreload(&htim2,4000000/(freq)-1);//设置自动重载数值
			__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,4000000/(freq)*(duty/100));	//保持占空比不变
		}
	}
	else			//从低频到高频
	{
		if(freq!=8000)
		{
			freq=freq+100;	//每次增加100HZ
			__HAL_TIM_SetAutoreload(&htim2,4000000/(freq)-1);//设置自动重载数值
			__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,4000000/(freq)*(duty/100));	//保持占空比不变
		}
	}

}

//PA7频率测量
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	uint16_t value=0;
	if(htim->Instance==TIM17)
	{
		value=__HAL_TIM_GET_COUNTER(&htim17);
		__HAL_TIM_SetCounter(&htim17,0);
		if(mode)
			freq_h=1000000/value;
		else
			freq_l=1000000/value;
		HAL_TIM_IC_Start_IT(&htim17,TIM_CHANNEL_1);
	}
}

void Get_Speed_Max()
{
	speed_h=(freq_h*2*3.14*r)/100/k;
	speed_l=(freq_l*2*3.14*r)/100/k;
	if(speed_h>speed_high_max)
		speed_high_max=speed_h;
	if(speed_l>speed_low_max)
		speed_low_max=speed_l;
}
/*************************按键控制****************************/
void Key_Function()
{
	//切换显示界面
	if(keys[0].key_sflg==1)
	{
		menu++;
		keys[0].key_sflg=0;
		change_flag=1;
		Display_Led();
	}
	
	//"选择"按键
	if(keys[1].key_sflg==1)
	{
		//数据界面下
		if(menu%3==0)
		{
			if(is_change_mode)
			{
				Change_Freq();
				Display_Menu();
				if(freq==40000 || freq==8000)
					mode=mode^1;
				n++;
				Display_Led();
				is_change_mode=0;	//不允许切换模式
				HAL_TIM_Base_Start_IT(&htim4);
				count_tim=0;
				led2_sta=0;
				Control_Single_Led(0x02,led2_sta);
			}
			
			
			
		}
		
		//参数界面下
		if(menu%3==1)
		{
			if(change_flag==1)
			{
				change_para=0;
				change_flag=0;
			}
			else
				change_para=change_para^1;
		}
		keys[1].key_sflg=0;
	}
	
	//加 按键
	if(keys[2].key_sflg==1)
	{
		if(change_para==0)
		{
			if(r==10)
				r=1;
			else
				r++;
		}
		else
		{
			if(k==10)
				k=1;
			else
				k++;
		}
		keys[2].key_sflg=0;
	}
	
	if(keys[3].key_sflg==1)
	{
		if(menu%3==1)
		{
			if(change_para==0)
		{
			if(r==1)
				r=10;
			else
				r--;
		}
		else
		{
			if(k==1)
				k=10;
			else
				k--;
		}
		}
		keys[3].key_sflg=0;
	}
	
	if(keys[3].key_lflg==1)
	{
		if(menu%3==0)
		{
			status=status^1;
			Display_Led();
		}
		keys[3].key_lflg=0;
		keys[3].key_tim=0;
	}

}
/********************************END******************************************/



/****************自定义系统函数*******************************/
void sys_init()
{
	//定时器初始化
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	HAL_TIM_IC_Start_IT(&htim17,TIM_CHANNEL_1);
	//LCD初始化
	LCD_Init();
	LCD_Clear(Black);
	LCD_SetTextColor(White);
	LCD_SetBackColor(Black);
	
	Get_Speed_Max();
	Display_Menu();
	
	//LED初始化
	Invoid_Complict();
	Control_All_Led();
	
	Control_Single_Led(0x01,0);
}

void sys_work()
{
	Get_Speed_Max();
	//Pwm_Duty_Change();
	Display_Menu();
	Key_Function();
}