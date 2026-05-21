#include "SCCB.h"

// //delay函数：us级别（haldelay是ms）
// void DWT_Init(void){
//     CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
//     DWT->CYCCNT = 0;
//     DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
// }

// void delay_us(uint32_t us){
//     uint32_t start = DWT->CYCCNT;
//     uint32_t ticks = us * (SystemCoreClock / 1000000);
//     while ((DWT->CYCCNT - start) < ticks);
// }




// static void SDA_OutputMode(void) {
//     GPIO_InitTypeDef GPIO_InitStruct = {0};
//     GPIO_InitStruct.Pin = OV_SDA_PIN;
//     GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
//     GPIO_InitStruct.Pull = GPIO_PULLUP;
//     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//     HAL_GPIO_Init(OV_GPIO_PORT, &GPIO_InitStruct);
// }

// static void SDA_InputMode(void) {
//     GPIO_InitTypeDef GPIO_InitStruct = {0};
//     GPIO_InitStruct.Pin = OV_SDA_PIN;
//     GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//     GPIO_InitStruct.Pull = GPIO_PULLUP;
//     HAL_GPIO_Init(OV_GPIO_PORT, &GPIO_InitStruct);
// }


// //没有写init函数，直接在cubemx配置：
// //sda和scl的GPIO mode：Open Drain
// //Pull-up
// #define SetSDA(val) HAL_GPIO_WritePin(OV_GPIO_PORT, OV_SDA_PIN, (val) ? GPIO_PIN_SET : GPIO_PIN_RESET)
// #define SetSCL(val) HAL_GPIO_WritePin(OV_GPIO_PORT, OV_SCL_PIN, (val) ? GPIO_PIN_SET : GPIO_PIN_RESET)
// #define ReadSDA()   HAL_GPIO_ReadPin(OV_GPIO_PORT, OV_SDA_PIN)


// void SCCB_Start(void){
//     SDA_OutputMode();
//     SetSDA(1);
//     SetSCL(1);
//     SetSDA(0);
//     SetSCL(0);
// }

// void SCCB_Stop(void){
//     SDA_OutputMode();
//     SetSDA(0);
//     SetSCL(1);
//     SetSDA(1);
// }

// void SCCB_SendNoAck(void){
//     SDA_OutputMode();
//     SetSDA(1);
//     SetSCL(1);
//     SetSCL(0);
//     SetSDA(0);
// }


// uint8_t SCCB_SendByte(uint8_t data){
//     for (uint8_t i = 0; i < 8; i++) {
//         SetSDA((data & 0x80) != 0);
//         data <<= 1;
//         SetSCL(1);
//         SetSCL(0);
//     }

//     SetSDA(1); 
//     SetSCL(1);
//     uint8_t ack = ReadSDA(); 
//     SetSCL(0);
//     return ack;
// }

// uint8_t SCCB_ReadByte(void){
//     uint8_t data = 0;
//     SetSDA(1); 

//     for (uint8_t i = 0; i < 8; i++) {
//         SetSCL(1);
//         data <<= 1;
//         if (ReadSDA()) data |= 0x01;
//         SetSCL(0);
//     }

//     SetSDA(1);
//     SetSCL(1);
//     SetSCL(0);

//     return data;
// }

// uint8_t SCCB_W_Reg(uint8_t reg, uint8_t data){
//     SCCB_Start();
//     if (SCCB_SendByte(OV7670_W_ADD)) { 
//         SCCB_Stop(); 
//         return 1; 
//     }
//     if (SCCB_SendByte(reg)) { 
//         SCCB_Stop(); 
//         return 1; 
//     }
//     if (SCCB_SendByte(data)) { 
//         SCCB_Stop(); 
//         return 1; 
//     }
//     SCCB_Stop();
//     return 0;
// }

// uint8_t SCCB_R_Reg(uint8_t reg){
//     uint8_t val;

//     SCCB_Start();
//     if (SCCB_SendByte(OV7670_W_ADD)) { 
//         SCCB_Stop(); 
//         return 0xFF; 
//     }

//     if (SCCB_SendByte(reg)) { 
//         SCCB_Stop(); 
//         return 0xFF; 
//     }
//     SCCB_Stop();

//     SCCB_Start();
//     if (SCCB_SendByte(OV7670_R_ADD)) { 
//         SCCB_Stop(); 
//         return 0xFF; 
//     }
//     val = SCCB_ReadByte();
//     SCCB_SendNoAck();
//     SCCB_Stop();

//     return val;
// }

void SCCB_W_Reg(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t val)
{
    HAL_I2C_Mem_Write(hi2c, OV7670_W_ADD, reg,
                      I2C_MEMADD_SIZE_8BIT, &val, 1, 100);
}

uint8_t SCCB_R_Reg(I2C_HandleTypeDef *hi2c, uint8_t reg)
{
    uint8_t val;
    HAL_I2C_Mem_Read(hi2c, OV7670_W_ADD, reg,
                     I2C_MEMADD_SIZE_8BIT, &val, 1, 100);
    return val;
}

