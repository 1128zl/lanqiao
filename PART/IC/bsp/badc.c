#include "badc.h"


//ADC��ȡ����
double Get_ADC(ADC_HandleTypeDef *pin)
{
	
	uint adc;
	HAL_ADC_Start(pin);
	
	//��ȡ
	adc=HAL_ADC_GetValue(pin);
	
	//4096---���ݱ���λ12λ
	return adc*3.3/4096;
}