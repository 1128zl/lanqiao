#include "myfile.h"
#include "main.h"
#include "display.h"
#include "key.h"
#include "lcd.h"

//�����ⲿ����
extern ADC_HandleTypeDef hadc2;
extern float v_max,v_min,v_37;
extern unsigned char led_upper,led_lower,led_sta;
extern uint8_t menu,change_type,upper_led,low_led;
extern TIM_HandleTypeDef htim2,htim3;
extern struct key keys[4];

/************************************ADCģ��*************************/
void Get_adc()
{
	int adc;
	HAL_ADC_Start(&hadc2);
	adc=HAL_ADC_GetValue(&hadc2);
	HAL_ADC_Stop(&hadc2);
	v_37= adc*3.3/4096;
}


/************************************��������ʵ��***********************/
void Key_Function()
{
	//�л���ʾ����
	if(keys[0].key_flg==1)
	{
		menu++;
		keys[0].key_flg=0;
	}
	//ѡ������Ĳ�����
	if(keys[1].key_flg==1)
	{
		change_type++;
		keys[1].key_flg=0;
	}
	
	//���ӵ�ǰѡ��ĵ�ѹ������LED����ָʾ�Ʋ���
	if(keys[2].key_flg==1)
	{
		//��ѹ����
		if(change_type%4==0)
		{
			v_max+=0.3;
		}
		
		//��ѹ����
		else if(change_type%4==1)
		{
			v_min+=0.3;
		}
		
		//����LED��
		else if(change_type%4==2)
		{
			if((upper_led+1)%8!=low_led)
			{
				//�ı�LED��λ��
				if(led_upper==0x80)
					led_upper=0x01;
				else
					led_upper<<=1;
				upper_led++;
				upper_led%=8;
			}
			else
				{
					//�ı�LED��λ��
				if(led_upper==0x80)
					led_upper=0x02;
				else
					led_upper<<=2;
				upper_led+=2;
				upper_led%=8;
				}
		}
		
		//����LED��
		else
		{
			if((low_led+1)%8!=upper_led)
			{
				if(led_lower==0x80)
					led_lower=0x01;
				else
					led_lower<<=1;
				low_led++;
				low_led%=8;
			}
			else
			{
				if(led_lower==0x80)
					led_lower=0x02;
				else
					led_lower<<=2;
				low_led+=2;
				low_led%=8;
			}
		}
		keys[2].key_flg=0;
	}
	
	//���ٵ�ǰѡ��ĵ�ѹ������LED����ָʾ�Ʋ���
	if(keys[3].key_flg==1)
	{
		//��ѹ����
		if(change_type%4==0)
		{
			v_max-=0.3;
		}
		
		//��ѹ����
		else if(change_type%4==1)
		{
			v_min-=0.3;
		}
		
		
		//����LED��
		else if(change_type%4==2)
		{
			
			if(upper_led==0 && low_led==7)
			{
				upper_led=6;
				led_upper=0x40;
			}
			else if(upper_led==0 && low_led!=7)
			{
				upper_led=7;
				led_upper=0x80;
			}
			else if(upper_led!=0 && upper_led-1==low_led)
			{
				upper_led-=2;
				led_upper>>=2;
			}
			else
			{
				upper_led--;
				led_upper>>=1;
			}
		}
		

		//����LED��
		else
		{
			
			if(low_led==0 && upper_led==7)
			{
				low_led=6;
				led_lower=0x40;
			}
			else if(low_led==0 && upper_led!=7)
			{
				low_led=7;
				led_lower=0x80;
			}
			else if(low_led!=0 && low_led-1==upper_led)
			{
				low_led-=2;
				led_lower>>=2;
			}
			else
			{
				low_led--;
				led_lower>>=1;
			}
		}
		keys[3].key_flg=0;
	}
	
	Display_led();
}



/********************************�Զ���ϵͳ����****************/
void sys_init()
{
	HAL_TIM_Base_Start_IT(&htim2);
	
	//lcd ��ʼ��
	LCD_Init();
	LCD_Clear(Black);
	LCD_SetTextColor(White);
	LCD_SetBackColor(Black);

	//LED��ʼ��
	Invoid_Complict();
	Led_Init();
	
	
}

void sys_work()
{
	Get_adc();
	Display_lcd();
	Invoid_Complict();
	Key_Function();
}