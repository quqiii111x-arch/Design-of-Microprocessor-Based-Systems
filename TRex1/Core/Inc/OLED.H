#ifndef __OLED_H
#define __OLED_H
//#include "sys.h"
  
#include "main.h"
//OLED模式设置
//0: 4线串行模式  （模块的BS1，BS2均接GND）
//1: 并行8080模式 （模块的BS1，BS2均接VCC）
#define OLED_MODE 0
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128  //列
#define Max_Row		64     //行
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

void delay_us(uint32_t nus);
void Write_IIC_Data(unsigned char IIC_Data);
void Write_IIC_Command(unsigned char I2C_Command);
void OLED_WR_Byte(uint8_t dat, uint8_t cmd);
void fill_picture(unsigned char fill_Data);
void OLED_On(void)  ;
void OLED_Clear(void)  ;
void OLED_Display_Off(void);
void OLED_Display_On(void);

void OLED_Set_Pos(unsigned char x, unsigned char y) ;
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no);
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t chr[],uint8_t Char_Size);// OLED_ShowString(0,0,a,sizeof(a));
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size2);	//OLED_ShowNum(103,6,t,3,16);//显示ASCII字符的码值

uint32_t oled_pow(uint8_t m,uint8_t n);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size);
void OLED_Init(void);
void OLED_Refresh_Gram(void);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
/*			OLED_ShowChar(0,3,data,16);
		  	OLED_ShowChar(8,3,data,16);*/
//			OLED_ShowChar((i-1)*8,3,str_data[i-1],16);
/*				             i=第几列    3=第三行								
		


*/
			
			
/*
x x x x x x x x x x x x x x x x     ==16个字符列   y=0行
x x x x x x x x x x x x x x x x     ==16个字符列   y=1行
x x x x x x x x x x x x x x x x     ==16个字符列   y=2行
x x x x x x x x x x x x x x x x     ==16个字符列   y=3行
x x x x x x x x x x x x x x x x     ==16个字符列   y=4行
x x x x x x x x x x x x x x x x     ==16个字符列   y=5行
x x x x x x x x x x x x x x x x     ==16个字符列   y=6行
x x x x x x x x x x x x x x x x     ==16个字符列   y=7行




*/




#endif



