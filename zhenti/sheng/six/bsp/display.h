#ifndef _DISPLAY_H_
#define _DISPLAY_H_


#include "main.h"

void Control_All_Led();
void Control_Single_Led(unsigned char location,unsigned char state);
void Invoid_Complict();
void Display_sys_lcd();
void Display_time();
void Display_lcd();
void Display_led();
#endif