#ifndef __LCD1602_H 
#define __LCD1602_H 
#include "stm32f1xx_hal.h" 

/*
#define LCD_PORT GPIOA->ODR 
typedef unsigned char uchar; 
void LCD_Init(void); 
void LCD_EnableCapture(void);
void LCD_WriteByte(uchar data);
void LCD_Clear(void); 
void LCD_Write_Command(uchar cmd); 
void LCD_Write_Data(uchar dat); 
void LCD_SetCursor(uchar row, uchar col); 
void LCD_Print(uchar *str);
#endif 
*/


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

