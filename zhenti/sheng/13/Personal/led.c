#include "led.h"
#include "main.h"

//熄灭所有LED灯
extern int8_t code_correct,error_counts;
extern TIM_HandleTypeDef htim2,htim3,htim4;

extern uint8_t counts;
unsigned char sta=1;

void Close_All_LED()
{
	HAL_GPIO_WritePin(GPIOC,0xff00,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}

//控制单个灯的亮灭
void Change_One_LED(unsigned char location,unsigned char state)
{
	HAL_GPIO_WritePin(GPIOC,location<<8,state);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);

}


//LED显示函数
void Led_Disp()
{
	//密码输入正确，LED1点亮5s后熄灭
	if(code_correct==1)
	{
		Close_All_LED();
		HAL_TIM_Base_Start_IT(&htim3);
		Change_One_LED(0x01,0);
	
	}
	else if(error_counts>=3)
	{
		counts=0;
		Close_All_LED();
		HAL_TIM_Base_Start_IT(&htim2);
		sta=sta^1;
		Change_One_LED(0x02,sta);
	}
}
