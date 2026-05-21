#ifndef __SCCB_H
#define __SCCB_H

#include "stm32f1xx_hal.h"  


#define OV_SDA_PIN       GPIO_PIN_9
#define OV_SCL_PIN       GPIO_PIN_8
#define OV_GPIO_PORT     GPIOB //A

#define OV7670_W_ADD     0x42  
#define OV7670_R_ADD     0x43  


// void SCCB_Init(void);
// void DWT_Init(void);
// void delay_us(uint32_t us);

// uint8_t SCCB_SendByte(uint8_t data);
// uint8_t SCCB_ReadByte(void);
// void SCCB_SendNoAck(void);
// void SCCB_Start(void);
// void SCCB_Stop(void);

void SCCB_W_Reg(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t data);
uint8_t SCCB_R_Reg(I2C_HandleTypeDef *hi2c, uint8_t reg);

#endif
