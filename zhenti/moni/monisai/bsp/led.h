#ifndef _LED_H_
#define _LED_H_

#include "main.h"

void Close_All_Led();
void Change_One_Led(unsigned char led ,unsigned char value);

void Change_Menu_Led(uint8_t menu);
void Led_Check_Qulify();

#endif