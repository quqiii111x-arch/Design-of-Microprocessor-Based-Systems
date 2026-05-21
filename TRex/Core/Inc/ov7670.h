#ifndef _OV7670_H_
#define _OV7670_H_

#include "stm32f1xx_hal.h"

#define OV7670_ADDW 0x42 // write
#define OV7670_ADDR 0x43 // read

#define OV7670_VSYNC  	PAin(8)			//同步信号检测IO
#define OV7670_WRST		PBout(0)		//写指针复位 
#define OV7670_WREN		PBout(12)		//写入FIFO使能
#define OV7670_RCK_H	GPIOA->BSRR=1<<11//设置读数据时钟高电平
#define OV7670_RCK_L	GPIOA->BRR=1<<11	//设置读数据时钟低电平
#define OV7670_RRST		PBout(1)  		//读指针复位
#define OV7670_CS		PAout(15)  		//片选信号(OE)

void OV7670_Init(I2C_HandleTypeDef *hi2c);

#endif
