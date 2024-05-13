#include "hadc.h"
#include "adc.h"

uint16_t getADC(void)
{
  uint16_t adc1=0;
	HAL_ADC_Start(&hadc2);
	adc1= HAL_ADC_GetValue(&hadc2);
	HAL_ADC_Stop(&hadc2);
	return adc1;
}