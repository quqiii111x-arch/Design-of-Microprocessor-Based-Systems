#include "ov7670.h"
#include "sccb.h"

void OV7670_Init(I2C_HandleTypeDef *hi2c) {
		SCCB_W_Reg(hi2c, 0x12, 0x80); // COM7: Reset
		HAL_Delay(10);

		SCCB_W_Reg(hi2c, 0x8C, 0x00); // RGB444: bit 0 disable
		SCCB_W_Reg(hi2c, 0x12, 0x10); // COM7: QVGA + RGB
    SCCB_W_Reg(hi2c, 0x0C, 0x08); // COM3: enable scaling
		SCCB_W_Reg(hi2c, 0x3E, 0x08); // COM14: scaling parameter adjust manually
    //SCCB_W_Reg(hi2c, 0x40, 0x10); // COM15: Y only (grey), bit[54]->01 rgb 565
		SCCB_W_Reg(hi2c, 0x40, 0xD0);
    SCCB_W_Reg(hi2c, 0x56, 0x80); // CONTRAST: Max
		//SCCB_W_Reg(hi2c, 0x3D, 0xC0); // COM13: bit7 gamma enable, bit 6 UV auto adjustment
	
		SCCB_W_Reg(hi2c, 0x11, 0x01); // CLKRC: clock /2
    SCCB_W_Reg(hi2c, 0x3B, 0x0A); // COM11: Optimize timing, bit 1 timing less than limit of banding filter when light too strong
	
		// window cut
		SCCB_W_Reg(hi2c, 0x17, 0x16); // HSTART
		SCCB_W_Reg(hi2c, 0x18, 0x04); // HSTOP
		SCCB_W_Reg(hi2c, 0x19, 0x02); // VSTART
		SCCB_W_Reg(hi2c, 0x1A, 0x7A); // VSTOP
		SCCB_W_Reg(hi2c, 0x32, 0xA4); // HREF
	
		// filter
		//SCCB_W_Reg(hi2c, 0x70, 0x3A);
		//SCCB_W_Reg(hi2c, 0x71, 0x35);
}


