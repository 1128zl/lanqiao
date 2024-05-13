#include "myfile.h"
#include "i2c_hal.h"
#include "lcd.h"
#include "stdio.h"
#include "display.h"
#include "key.h"

//定义外部变量
extern ADC_HandleTypeDef hadc2;
extern TIM_HandleTypeDef htim2,htim3;
extern uint8_t threshold[3];//初始阈值
extern char str[30];
extern float height;//液位高度，R37输出电压
extern float v_37;
extern uint8_t level;//液位等级
extern char TXbuff[30],RXbuff[30];//串口接收发送缓冲区
extern uint8_t Rxdat,BufIndex,menu,level_flg,check_flg;
extern uint8_t led1_sta,led2_sta,led3_sta;//LED1，LED2，LED3的电平状态
extern int8_t change_thre;	//阈值改变处
extern struct key keys[4];
extern UART_HandleTypeDef huart1;

//定义变量
uint8_t thres_init=0;
uint8_t level_before;
//液位等级判定
void level_confirm()
{
	
	height=v_37*(100/3.3);
	level_before=level;
	if(height<=threshold[0])
		level=0;
	else if(height>threshold[0] && height<=threshold[1])
		level=1;
	else if(height>threshold[1] && height<=threshold[2])
		level=2;
	else 
		level=3;
	
	char str1[20];
	if(level>level_before)
		{
			
			sprintf(str1,"A:H%.1f+L%d+U\r\n",height,level);
			HAL_UART_Transmit(&huart1,(u8*)str1,sizeof(str1),50);
			
		}
	else if(level<level_before)
		{
			
			sprintf(str1,"A:H%.1f+L%d+D\r\n",height,level);
			HAL_UART_Transmit(&huart1,(u8*)str1,sizeof(str1),50);
			
		}
	
	
	if( level_flg==0 && level!=level_before)
	{
		level_flg=1;
		
		//避免指示灯多次闪烁
		level_before=level;
	}
}



/***********************************串口通信****************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)
	{
		RXbuff[BufIndex++]=Rxdat;
		HAL_UART_Receive_IT(&huart1,&Rxdat,1);
	}
}

void Deal_Infromation()
{
	if(BufIndex==0)
		return ;
	
	if(RXbuff[0]=='C' )
	{
		sprintf(str,"C:H%.1f+L%d\r\n",height,level);
		HAL_UART_Transmit(&huart1,(u8*)str,sizeof(str),50);
		check_flg=1;
	}
	else if(RXbuff[0]=='S')
	{
		sprintf(str,"S:TL%d+TM%d+TH%d\r\n",threshold[0],threshold[1],threshold[2]);
		HAL_UART_Transmit(&huart1,(u8*)str,sizeof(str),50);
		check_flg=1;
	}
	
	else
	{
		sprintf(str,"INPUT ERROR\r\n");
		HAL_UART_Transmit(&huart1,(u8*)str,sizeof(str),50);
	}
	
	BufIndex=0;
}


/**********************************EEPROM*************************************/

void eeprom_update_thres()
{
	//第一次上电
	if(thres_init==0)
	{
		uint8_t thre1,thre2,thre3;
		thre1=20; thre2=40;	thre3=60;
		Eeprom_Write_Bytes(0x10,&thre1,1);
		HAL_Delay(10);
		Eeprom_Write_Bytes(0x11,&thre2,1);
		HAL_Delay(10);
		Eeprom_Write_Bytes(0xa0,&thre3,1);
		HAL_Delay(10);
		thres_init=1;
	}
	else
	{
		Eeprom_Write_Bytes(0x10,&threshold[0],1);
		HAL_Delay(10);
		Eeprom_Write_Bytes(0x11,&threshold[1],1);
		HAL_Delay(10);
		Eeprom_Write_Bytes(0xa0,&threshold[2],1);
		HAL_Delay(10);
		
	}
}

void eeprom_get_thres()
{
	Eeprom_Read_Bytes(0x10,&threshold[0],1);
	HAL_Delay(10);
	Eeprom_Read_Bytes(0x11,&threshold[1],1);
	HAL_Delay(10);
	Eeprom_Read_Bytes(0xa0,&threshold[2],1);
	HAL_Delay(10);
}
/*********************************ADC*********************************/
void Get_adc()
{
	int adc;
	HAL_ADC_Start(&hadc2);
	adc=HAL_ADC_GetValue(&hadc2);
	HAL_ADC_Stop(&hadc2);
	v_37=adc*3.3/4096;

}

/***************************************按键功能实现***********************************/
void Key_Function()
{
	//显示界面切换
	if(keys[0].key_flg==1)
	{
		menu++;
		keys[0].key_flg=0;
	}

	//切换阈值修改处
	if(menu%2==1 && keys[1].key_flg==1)
	{
		change_thre++;
		Display_lcd();
		keys[1].key_flg=0;
	}
	
	//阈值增加
	if(menu%2==1 && keys[2].key_flg==1)
	{
		if(threshold[change_thre%3]!=95)
		{
			threshold[change_thre%3]+=5;
			eeprom_update_thres();
			
		}
		keys[2].key_flg=0;
	}
	
	//阈值减少
	if(menu%2==1 && keys[3].key_flg==1)
	{
		if(threshold[change_thre%3]!=5)
		{
			threshold[change_thre%3]-=5;
			eeprom_update_thres();
		}
		keys[3].key_flg=0;
	}
	Display_lcd();
	
	Display_led();
}
/**************************************自定义系统函数******************************************/
void sys_init()
{
	//I2C初始化
	I2CInit();
	
	//LCD初始化
	LCD_Init();
	LCD_Clear(Black);
	LCD_SetTextColor(White);
	LCD_SetBackColor(Black);
	
	//打开定时器
	HAL_TIM_Base_Start_IT(&htim2);
	
	Get_adc();
	HAL_TIM_Base_Start_IT(&htim3);
	
	//串口初始化
	HAL_UART_Receive_IT(&huart1,&Rxdat,1);
	//阈值上电初始化
	eeprom_update_thres();
	Display_lcd();
	Display_led();
}

void sys_work()
{
	
	Deal_Infromation();
	Key_Function();


}
