#ifndef _MYFILE_H_
#define _MYFILE_H_

#include "main.h"

void level_confirm();
void eeprom_update_thres();
void eeprom_get_thres();
void Key_Function();
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void Deal_Infromation();
void Get_adc();
void sys_init();
void sys_work();

#endif