#include "badc.h"


//ADC获取函数
double Get_ADC(ADC_HandleTypeDef *pin)
{
	
	uint adc;
	HAL_ADC_Start(pin);
	
	//读取
	adc=HAL_ADC_GetValue(pin);
	
	//4096---根据比特位12位
	return adc*3.3/4096;
}