#include "lcd.h"
#include "led.h"
#include "stdio.h"
#include "key.h"

//定义外部变量
extern TIM_HandleTypeDef htim2,htim3,htim4;
extern int8_t code_1,code_2,code_3;
extern uint8_t code_one,code_two,code_thr;
extern uint8_t duty;
extern uint16_t freq,count;
extern struct key keys[4];
extern uint8_t menu,error_counts,is_correct;
extern unsigned char led2_sta;
extern UART_HandleTypeDef huart1;

//定义变量
char st[30];
char RXbuff[30],TXbuff[30];	//串口接收发送缓冲区
unsigned char Rxdat,BufIndex=0;	

// 密码输入界面显示
void Disp_Code_Enter()
{
	sprintf(st,"       PSD          ");
	LCD_DisplayStringLine(Line1,(u8*)st);
	if(code_1==-1)
	{
		sprintf(st,"    B1:@            ");
		LCD_DisplayStringLine(Line3,(u8*)st);
	}
	else
	{
		sprintf(st,"    B1:%d             ",code_1);
		LCD_DisplayStringLine(Line3,(u8*)st);
	}
	
	if(code_2==-1)
	{
		sprintf(st,"    B2:@            ");
		LCD_DisplayStringLine(Line4,(u8*)st);
	}
	else
	{
		sprintf(st,"    B2:%d             ",code_2);
		LCD_DisplayStringLine(Line4,(u8*)st);
	}
	
	if(code_3==-1)
	{
		sprintf(st,"    B3:@            ");
		LCD_DisplayStringLine(Line5,(u8*)st);
	}
	else
	{
		sprintf(st,"    B3:%d             ",code_3);
		LCD_DisplayStringLine(Line5,(u8*)st);
	}
}

// 参数界面显示
void Disp_Para()
{
	
	sprintf(st,"       STA          ");
	LCD_DisplayStringLine(Line1,(u8*)st);
	sprintf(st,"    F:%dHz             ",freq);
	LCD_DisplayStringLine(Line3,(u8*)st);
	sprintf(st,"    D:%d%%           ",duty);
	LCD_DisplayStringLine(Line4,(u8*)st);
	sprintf(st,"                    ");
	LCD_DisplayStringLine(Line5,(u8*)st);
}
//系统初始化函数
void sys_init()
{
	//lcd初始化
	LCD_Init();
	LCD_Clear(Black);
  LCD_SetTextColor(White);
  LCD_SetBackColor(Black);
	
	//使能PWM输出
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	
	//打开定时器TIM2
	HAL_TIM_Base_Start_IT(&htim4);
	
	//打开串口接收
	HAL_UART_Receive_IT(&huart1, &Rxdat, 1);
	//初始状态的界面
	Disp_Code_Enter();
	//LED初始化
	Contro_Led(1);
}

//LCD综合使用函数
void Display_lcd(uint8_t menu)
{
		if(menu==0)
		Disp_Code_Enter();
	else
		Disp_Para();
}

void Display_led()
{
	if(is_correct==1)
	{
		
		//LED1亮5秒后熄灭
		count=0;
		HAL_GPIO_WritePin(GPIOC,0xff00,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
		Contro_One_Led(0x01,0);
		HAL_TIM_Base_Start_IT(&htim3);
	
	}
	
	else if(is_correct==0 && error_counts>=3)
	{
		//	LED2每隔0.1秒切换亮灭，保持5秒钟
		count=0;
		led2_sta=1;
		
		//这里同样是解决LCD，LED显示冲突的一个小处理
		HAL_GPIO_WritePin(GPIOC,0xff00,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
		Contro_One_Led(0x02,led2_sta);
		
		//从这里打开定时器3，避免时间上有误差
		HAL_TIM_Base_Start_IT(&htim3);
	}

}
/************************串口模块****************************/
//串口回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)
	{
		RXbuff[BufIndex++]=Rxdat;
		HAL_UART_Receive_IT(&huart1, &Rxdat, 1);
	}

}
//串口接收数据处理函数
void GetData_uart()
{		
		//判断是否收到数据
		if(BufIndex<=6)
			return ;
		
		//判断接收到数据是否符合要求
		if(RXbuff[0]-48==code_one && RXbuff[1]-48==code_two && RXbuff[2]-48==code_thr && RXbuff[3]=='-')
		{
			if(RXbuff[4]-48==code_one && RXbuff[5]-48==code_two && RXbuff[6]-48==code_thr)
				return ;
			else
				{
					code_one=RXbuff[4]-48;
					code_two=RXbuff[5]-48;
					code_thr=RXbuff[6]-48;
					BufIndex=0;
					sprintf(TXbuff,"success\r\n");
					HAL_UART_Transmit_IT(&huart1, (u8*)TXbuff, sizeof(TXbuff));
				}
		}
		else
			return ;
}
void Key_Function()
{
	if(keys[0].key_flg==1)
	{
		//对密码第一位进行更新数值
		code_1++;
		if(code_1==10)
			code_1=0;
		
		keys[0].key_flg=0;
	}
	if(keys[1].key_flg==1)
	{
		//对密码第二位进行更新数值
		code_2++;
		if(code_2==10)
			code_2=0;
		
		keys[1].key_flg=0;
	}

	if(keys[2].key_flg==1)
	{
		//对密码第三位进行更新数值
		code_3++;
		if(code_3==10)
			code_3=0;
		
		keys[2].key_flg=0;
	}
	
	if(keys[3].key_flg==1)
	{
		//密码输入正确
		if(code_1==code_one && code_2==code_two && code_3==code_thr)
			{
				is_correct=1;
				error_counts=0;
				menu=1;
				
				//修改pwm输出频率，占空比
				htim2.Init.Prescaler = 40-1;
				htim2.Init.Period = 999;
				__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,100);
				duty=10;
				freq=2000;
				HAL_TIM_Base_Start_IT(&htim3);
				Display_led();
			}
		
		//密码输入错误
		else
		{
			is_correct=0;
			menu=0;
			error_counts++;
			code_1=code_2=code_3=-1;
			Display_led();
		}
		
		keys[3].key_flg=0;
	}
}

//程序运行总函数
void sys_work()
{
	//串口接收处理
	GetData_uart();

	Display_lcd(menu);
	Key_Function();
}