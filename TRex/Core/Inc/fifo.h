#ifndef __FIFO_H
#define __FIFO_H

#include "main.h"

void FIFO_Init(void);
void FIFO_StartReadFrame(void);
uint8_t FIFO_ReadByte(void);
void FIFO_EndReadFrame(void);

uint8_t rgb565_to_y(uint16_t rgb);

#endif