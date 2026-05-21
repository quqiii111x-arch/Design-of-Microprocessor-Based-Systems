/*
OV7670-带FIFO——STM32F103C8T6接线图
OV7670—————C8T6
VCC             3.3V
GND             GND
DC0-7           PA0-PA7
VSYNC           PA8
RCK             PA11
CS(OE)          PA15
WRST            PB0
RRST            PB1
SIOC            PB10
SIOD            PB11
WREN(WR/WEN)    PB12



2.8寸TFT SPI屏幕——STM32F103C8T6接线图
VCC             3.3V      
GND             GND          
SDI(MOSI)       PB15        
SDO(MISO)       PB14                
SCK             PB13         
DC/RS           PB8         
RST             PB7         
CS              PB6  
LED             PB5   
*/


#include "stm32f10x.h"
#include "delay.h"
#include "lcd.h"
#include "ov7670.h"
#include "spi.h"
#include "test.h"
#include "usart.h"
#include "timer.h"
#include "exti.h"
#include "GUI.h"
#include "sccb.h"

extern u8 ov_sta;	//全局变量定义
extern u8 ov_frame;	//全局变量定义

//TFT LCD刷新显示内容
void camera_refresh(void)
{
	u32 j;
 	u16 color;

	if(ov_sta)
	{
		LCD_direction(1);
		LCD_SetWindows(0,0,319,239);
		
		OV7670_RRST=0;				//开始复位读指针 
		OV7670_RCK_L;
		OV7670_RCK_H;
		OV7670_RCK_L;
		OV7670_RRST=1;				//复位读指针结束 
		OV7670_RCK_H;

		for(j=0;j<76800;j++)        //清除TFT内部显示方向控制寄存器值
		{
			//读数据
			OV7670_RCK_L;
			color=GPIOA->IDR&0XFF;	
			OV7670_RCK_H; 
			color<<=8;  
			
			//读数据
			OV7670_RCK_L;
			color|=GPIOA->IDR&0XFF;	
			OV7670_RCK_H; 
			Lcd_WriteData_16Bit(color); 

		}  
		EXTI_ClearITPendingBit(EXTI_Line8); //清零帧中断标记
 		ov_sta=0;					
		ov_frame++; 

	} 
}


//串口调试
void camera_refresh_1(void)
{
	u32 j;
	u8 data1,data2;

	if(ov_sta)//有帧中断更新
	{
		
		OV7670_RRST=0;				//开始复位读指针 
		OV7670_RCK_L;
		OV7670_RCK_H;
		OV7670_RCK_L;
		OV7670_RRST=1;				//复位读指针结束 
		OV7670_RCK_H;
	
		printf("%c", 0x01);			
		printf("%c", 0xFE);
		for(j=0;j<76800;j++)        //清除TFT内部显示方向控制寄存器值
		{
			//读数据
			OV7670_RCK_L;
			data1=GPIOA->IDR&0XFF;	
			OV7670_RCK_H;  
				
			//读数据
			OV7670_RCK_L;
			data2 =GPIOA->IDR&0XFF;	
			OV7670_RCK_H; 
			
			USART_SendData(USART1, data1);
			USART_SendData(USART1, data2);

		}  	
 		ov_sta=0;					//下一次扫描
		
		printf("%c", 0xFE);
		printf("%c", 0x01);

	} 
}
int main()
{
	SystemInit();                   //初始化RCC 设置系统主频为72MHZ
	delay_init(72);	                //延时初始化
	
#if 0
	
	SPI2_Init();                    //硬件SPI2初始化
	LCD_Init();	                    //液晶屏初始化

	while(OV7670_Init())
	{
		LCD_ShowString(100,20,16,"ERROR",1);
		delay_ms(400);
	}
	LCD_ShowString(200,200,16,"ok",1);     
	delay_ms(1500);	 
	

	LCD_Clear(BLACK);
	EXTI8_Init();	
	OV7670_Effects_Set();
	OV7670_CS=0;
	
	LCD_Clear(BLACK);
	
	while(1){
		camera_refresh();//更新显示
	}
	
#endif	
	
	uart_init(115200);

#if 1
	while(OV7670_Init())
	{
		printf("OV7670 init failed");
	}
	
	delay_ms(1500); 
	
	EXTI8_Init();
	OV7670_Effects_Set();
	OV7670_CS=0;
	
	while(1)
	{
		camera_refresh_1();
	}
#endif	
}
