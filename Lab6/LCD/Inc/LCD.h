/************************************************************************ 
* lcd1602.h * Header file for the LCD Driver * for LAB3 *DB0~DB7 *PA0~PA7= LCD_PORT 
************************************************************************/ 
#ifndef __LCD_H 
#define __LCD_H 
#include "stm32f1xx_hal.h" 
#define LCD_PORT GPIOA->ODR 
typedef unsigned char uchar; 
/* Function prototypes */ 
void LCD_init(void); 
void LCD_Clear(void); 
void LCD_Write_Command(uchar Com); 
void LCD_Write_Data(uchar dat); 
uchar LCD_Read_State(void); 
void LCD_Set_Position(uchar x,uchar y); 
void LCD_Display_Char(uchar Char,uchar x,uchar y); 
void LCD_Display_String(uchar x,uchar y,uchar *str); 
#endif 
/*****************end of LCD.h**********************/
