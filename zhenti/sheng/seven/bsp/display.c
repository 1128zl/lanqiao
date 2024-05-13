#include "display.h"
#include "stdio.h"
#include "lcd.h"
#include "myfile.h"

//定义外部变量
extern uint8_t threshold[3];//初始阈值
extern char str[30];
extern float height;//液位高度，R37输出电压
extern float v_37;
extern uint8_t level;//液位等级
extern int8_t change_thre;	//阈值改变处
extern uint8_t menu,level_flg,check_flg,level_before;
extern uint8_t led1_sta,led2_sta,led3_sta;//LED1，LED2，LED3的电平状态
extern uint8_t shrink_count_led2,shrink_count_led3;
extern TIM_HandleTypeDef htim4,htim3;

/****************************LED显示****************************/
void led_init()
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_8 
                          |GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}

void Control_Single_Led(unsigned char location,unsigned char value)
{
	HAL_GPIO_WritePin(GPIOC,location<<8,value);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}

void Invoid_Complict()
{
	HAL_GPIO_WritePin(GPIOC,0xff00,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);

}

void Display_led()
{
	Invoid_Complict();
	
	//运行状态指示灯--LED1
	if(menu%2==0)
	{
		led1_sta^=1;
		Control_Single_Led(0x01,led1_sta);
	}
	
	//液位等级变化指示灯--LED2
	if(level_flg==1)
	{
		led2_sta^=1;
		Control_Single_Led(0x02,led2_sta);
		
	}
	//通讯状态指示灯--LED3
	if(check_flg==1)
	{
		led3_sta^=1;
		Control_Single_Led(0x04,led3_sta);
		
		
	}
}
/****************************LCD显示****************************/

void LCD_Check()
{
	level_confirm();
	sprintf(str,"    Liquid Level            ");
	LCD_DisplayStringLine(Line1,(u8*)str);
	sprintf(str,"   Height:%.1f            ",height);
	LCD_DisplayStringLine(Line2,(u8*)str);
	sprintf(str,"   ADC:%.2f            ",v_37);
	LCD_DisplayStringLine(Line3,(u8*)str);
	sprintf(str,"   Level:%d            ",level);
	LCD_DisplayStringLine(Line4,(u8*)str);
}

void LCD_Setting()
{
	sprintf(str,"   Parameter Setup     ");
	LCD_DisplayStringLine(Line1,(u8*)str);
	
	if(change_thre%3==0)
	{
		sprintf(str,"  Threshold1:%d          ",threshold[0]);
		LCD_SetBackColor(Blue);
		LCD_DisplayStringLine(Line2,(u8*)str);
		LCD_SetBackColor(Black);
		sprintf(str,"  Threshold2:%d          ",threshold[1]);
		LCD_DisplayStringLine(Line3,(u8*)str);
		sprintf(str,"  Threshold3:%d          ",threshold[2]);
		LCD_DisplayStringLine(Line4,(u8*)str);
	}
	
	else if(change_thre%3==1)
	{
		sprintf(str,"  Threshold1:%d          ",threshold[0]);
		LCD_DisplayStringLine(Line2,(u8*)str);
		sprintf(str,"  Threshold2:%d          ",threshold[1]);
		LCD_SetBackColor(Blue);
		LCD_DisplayStringLine(Line3,(u8*)str);
		LCD_SetBackColor(Black);
		sprintf(str,"  Threshold3:%d          ",threshold[2]);
		LCD_DisplayStringLine(Line4,(u8*)str);
	}

	else 
	{
		sprintf(str,"  Threshold1:%d          ",threshold[0]);
		LCD_DisplayStringLine(Line2,(u8*)str);
		sprintf(str,"  Threshold2:%d          ",threshold[1]);
		LCD_DisplayStringLine(Line3,(u8*)str);
		sprintf(str,"  Threshold3:%d          ",threshold[2]);
		LCD_SetBackColor(Blue);
		LCD_DisplayStringLine(Line4,(u8*)str);
		LCD_SetBackColor(Black);
	}
}

void Display_lcd()
{
	eeprom_get_thres();
	if(menu%2==0)
		LCD_Check();
	else
		LCD_Setting();
}