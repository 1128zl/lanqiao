#include "myfile.h"
#include "key.h"
#include "led.h"
#include "main.h"
#include "lcd.h"
#include "stdio.h"
#include "string.h"

//外部定义变量
extern TIM_HandleTypeDef htim3,htim2;
extern UART_HandleTypeDef huart1;

extern uint8_t standard_37,standard_38;
extern int tim3_count;

extern uint8_t menu,qulify_37,qulify_38,
			check_total_37,standard_total_37,
			check_total_38,standard_total_38,update_standard;
extern float v_37,v_38,qulify_ratio_37,qulify_ratio_38;
extern float standard_37_up,standard_37_down,
						standard_38_up,standard_38_down;
extern struct key keys[4];

extern void Get_ADC(uint8_t type);

extern char RX_Buff[10],TX_Buff[50];

extern unsigned char BufIndex;
extern unsigned char Rxdat;


/********************按键控制部分***********************************/

void Key_Proc()
{
	//按键1 --- 改变显示界面
	if(keys[0].key_flag==1)
	{
		menu++;
		//Display_Lcd();
		
		//Led_Menu(menu);
		keys[0].key_flag=0;
	}
	
	
	//按键2 --- 产品参数界面下，进行R37检测
	//      --- 标准设置界面下，决定要调整的标准
		if(keys[1].key_flag==1)
		{
			//产品参数界面
			if(menu%3==0)
				{
					if(v_37>=standard_37_down && v_37 <=standard_37_up)
						{
							standard_total_37++;
							qulify_37=1;
						}
						Led_Check_Qulify();
						check_total_37++;
						qulify_ratio_37=standard_total_37/check_total_37*100;
				}
			
			//标准设置界面
			else if(menu%3==1)
				{
					update_standard++;
				}
			keys[1].key_flag=0;
		}


	//按键3 --- 产品参数界面下，进行R37检测
	//      --- 标准设置界面下，增加当前需要调整的标准参数		
		if(keys[2].key_flag==1)
		{
			//产品参数界面
			if(menu%3==0)
				{
					if(v_38>=standard_38_down && v_38 <=standard_38_up)
						{
							standard_total_38++;
							qulify_38=1;
						}
						Led_Check_Qulify();
						check_total_38++;
						qulify_ratio_38=standard_total_38/check_total_38*100;
				}
			
			//标准设置界面
			else if(menu%3==1)
				{
					//增加R37的标准上限
					if(update_standard%4==0)
					{
						if(standard_37_up==3.0)
							standard_37_up=2.2;
						else
							standard_37_up+=0.2;
					}
					
					//增加R37的标准下限
					else if(update_standard%4==0)
					{
						Add_Value_Down(standard_37_down);
					}
					
					//增加R38的标准上限
					if(update_standard%4==0)
					{
						Add_Value_Up(standard_38_up);
					}
					
					//增加R38的标准下限
					else if(update_standard%4==0)
					{
						Add_Value_Down(standard_38_down);
					}
				qulify_ratio_37=qulify_ratio_38=0;
				}
			keys[2].key_flag=0;
		}
		
	//按键4 --- 合格率界面下，进行合格率清零
	//      --- 标准设置界面下，减少当前需要调整的标准参数		
		if(keys[3].key_flag==1)
		{
			if(menu%3==2)
				{
					qulify_ratio_37=qulify_ratio_38=0;
				}
			else if(menu%3==1)
			{
				//减少R37的标准上限
					if(update_standard%4==0)
					{
						Sub_Value_Up(standard_37_up);
					}
					
					//减少R37的标准下限
					else if(update_standard%4==0)
					{
						Sub_Value_Down(standard_37_down);
					}
					
					//减少R38的标准上限
					if(update_standard%4==0)
					{
						Sub_Value_Up(standard_38_up);
					}
					
					//减少R38的标准下限
					else if(update_standard%4==0)
					{
						Sub_Value_Down(standard_38_down);
					}
				qulify_ratio_37=qulify_ratio_38=0;
			}
		keys[3].key_flag=0;
		}
}

void sys_work()
{		
		
		Usart_Function();
		Get_ADC(37);
		Get_ADC(38);
		Key_Proc();
		Display_Lcd();
		HAL_GPIO_WritePin(GPIOC,0xff00,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
		Change_Menu_Led(menu);
}
//程序的上电初始化
void sys_Init()
{
	LCD_Init();   //LCD初始化
	LCD_Clear(Black);	//清空LCD
	LCD_SetBackColor(Black);	//设置背景颜色为黑色
	LCD_SetTextColor(White);	//设置字体颜色为白色
	Get_ADC(37);
	Get_ADC(38);
	Display_Lcd();
	Close_All_Led();
	HAL_TIM_Base_Start_IT(&htim2);
	Change_Menu_Led(menu);

}
/*****************串口通信函数******************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)
	{
		RX_Buff[BufIndex++]=Rxdat;
		HAL_UART_Receive_IT(&huart1, &Rxdat, 1);
	}
}

void Usart_Function()
{
	
	//判断是否接受到了数据
	if(BufIndex==0 )
		return;
	
	
	
		if(BufIndex>=3 && RX_Buff[0]=='R' && RX_Buff[1]=='3' &&RX_Buff[2]=='7' )
		{
			
			sprintf(TX_Buff,"R37:%d,%d,%.2f%%  \r\n",check_total_37,qulify_37,qulify_ratio_37);
			HAL_UART_Transmit_IT(&huart1,(uint8_t*)TX_Buff,sizeof(TX_Buff));
		}
		
		else if(BufIndex>=3 && RX_Buff[0]=='R' && RX_Buff[1]=='3' &&RX_Buff[2]=='8')
		{
			
			sprintf(TX_Buff,"R38:%d,%d,%.2f%%  \r\n",check_total_38,qulify_38,qulify_ratio_38);
			HAL_UART_Transmit_IT(&huart1,(uint8_t*)TX_Buff,sizeof(TX_Buff));
		}
	
	else 
		{
			
			sprintf(TX_Buff,"input error      \r\n");
			HAL_UART_Transmit_IT(&huart1,(uint8_t*)TX_Buff,sizeof(TX_Buff));
		}
	
	
	BufIndex=0;
	return ;
	
}
//增加标准的上限
void Add_Value_Up(float value)
{
	if(value==3.0)
		value=2.2;
	else 
		value+=0.2;
}
//增加标准的下限
void Add_Value_Down(float value)
{
	if(value==2.0)
		value=1.2;
	else 
		value+=0.2;
}
//减少标准的上限
void Sub_Value_Up(float value)
{
	if(value==2.2)
		value=3.0;
	else 
		value-=0.2;
}
//减少标准的下限
void Sub_Value_Down(float value)
{
	if(value==1.2)
		value=2.0;
	else 
		value-=0.2;
}


