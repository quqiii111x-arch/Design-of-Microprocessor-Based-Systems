#include "fifo.h"
#include "gpio.h"

void FIFO_Init(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);  // RRST = 0
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);    // WRST = 1
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);     // OE = 1
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);     // WEN = 1
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);  // RCK = 0
}

void FIFO_StartReadFrame(void)
{
		// write reset
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
		// read reset
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);  // RRST = 0
    for (int i = 0; i < 2; i++)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET); 
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET); 
    }
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);  // RRST = 1
}

uint8_t FIFO_ReadByte(void) // output: Y
{
    uint16_t data = 0;
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET); // OE = 0
		// high 8
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
		data = (uint16_t)(GPIOA->IDR & 0x00FF) << 8;
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
		// low 8
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
		data |= (GPIOA->IDR & 0x00FF);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
		/*
    for (int i = 0; i < 8; i++)
    {
        if (HAL_GPIO_ReadPin(GPIOA, i))
            data |= (1 << i);
    }
		
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);   
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET); 
		*/
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);   // OE = 1
		
		uint8_t Y = rgb565_to_y(data);

    return Y;
		//return data;
}

void FIFO_EndReadFrame(void)
{

}


uint8_t rgb565_to_y(uint16_t rgb) {
    uint8_t r = (rgb >> 11) & 0x1F;  // R: 5-bit (0-31)
    uint8_t g = (rgb >> 5)  & 0x3F;  // G: 6-bit (0-63)
    uint8_t b = rgb & 0x1F;          // B: 5-bit (0-31)

    return (uint8_t)( (r * 629 + g * 608 + b * 240) >> 8 );
}
