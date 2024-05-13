#include "myfile.h"
#include "stdio.h"
#include "lcd.h"
#include "led.h"
#include "key.h"

//定义外部变量
extern uint8_t origin_code,error_counts,menu;
extern char Rxbuff[8];
extern uint8_t Rxdat,BufIndex;
extern TIM_HandleTypeDef htim2,htim3,htim4;
extern int8_t code_1,code_2,code_3,code_correct;
extern uint8_t freq,duty;
//定义外部函数
extern void Lcd_Display_Code();	

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)
		{
			
			
		}

}


void sys_Init()
{
	LCD_Init();
	LCD_Clear(Black);
  LCD_SetTextColor(White);
  LCD_SetBackColor(Black);
  
  
	HAL_TIM_Base_Start_IT(&htim4);
	HAL_TIM_Base_Start_IT(&htim2);

	Close_All_LED();
	Lcd_Display_Code();	
}

void sys_work()
{
	
	if(menu==1)
		Lcd_Display_State();
	else
		Lcd_Display_Code();
	Close_All_LED();
	//Led_Disp();
	Key_Function();
}

//密码输入界面的显示
void Lcd_Display_Code()
{
	char str1[30],str2[30],str3[30];
	sprintf(str1,"       PSD       ");
	LCD_DisplayStringLine(Line1, (u8 *)str1);
	
	//判断是否是刚上电或者重新进入密码输入界面
	if(code_1==-1 )
	{
		sprintf(str1,"    B1:@            ");
		
	}
	else
		{
			sprintf(str1,"    B1:%d            ",code_1);
			
		}
	
	if(code_2==-1 )
	{
		sprintf(str2,"    B2:@            ");
		
	}
	else
		{
			sprintf(str2,"    B2:%d            ",code_2);
		
		}
		
	if(code_3==-1 )
	{
		sprintf(str3,"    B3:@            ");
	
	}
	else
		{
			sprintf(str3,"    B3:%d            ",code_3);
		
		}	
	LCD_DisplayStringLine(Line3, (u8 *)str1);
	LCD_DisplayStringLine(Line4, (u8 *)str2);
	LCD_DisplayStringLine(Line5, (u8 *)str3);
}

//输出状态界面显示
void Lcd_Display_State()
{
	char str[30];
	
	
	sprintf(str,"       STA          ");
	LCD_DisplayStringLine(Line1, (u8 *)str);
	
	sprintf(str,"    F:%dHz     ",freq);
	LCD_DisplayStringLine(Line3, (u8 *)str);
	sprintf(str,"    D:%d%%             ",duty);
	LCD_DisplayStringLine(Line4, (u8 *)str);
	LCD_DisplayStringLine(Line5, (u8 *)"                   ");	

}