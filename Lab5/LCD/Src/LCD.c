/************************************************************************ 
 * lcd1602.c 
 * ************************************************************************/ 
#include "LCD.h" 
#include "gpio.h" 
#include "main.h" 
/* initialize the LCD module */ 
/* define macros for LCD instructions*/ 
#define LCD_IDLE    0x33 
#define LCD_2_LINE_4_BITS  0x28 
#define LCD_2_LINE_8_BITS  0x38 
#define LCD_DSP_CSR   0x0c  
#define LCD_CLR_DSP   0x01 
#define LCD_CSR_INC   0x06  
#define LCD_SFT_MOV   0x14 
 
void LCD_init(void)      
{ 
// Delay_ms(15); 
// HAL_GPIO_WritePin(GPIOB,LCD_RS_Pin, GPIO_PIN_RESET); // LCD_RS = 0,send Command 
// LCD_PORT = LCD_IDLE; //function set -8 bit interface 
// Delay_ms(5);  //wait for 5 ms 
// LCD_PORT = LCD_IDLE; //function set -8 bit interface 
// Delay_us(100);   //wait for 100us 
// LCD_PORT = LCD_IDLE;  // function set 
// Delay_ms(5); 
// LCD_PORT = LCD_IDLE; 
// Delay_us(100); 
  
 //LCD_Write_Command(0x38); 
 LCD_Write_Command(LCD_2_LINE_8_BITS);//8-bit display 
 HAL_Delay(5);
 LCD_Write_Command(LCD_2_LINE_8_BITS); 
 //LCD_Write_Command(0x08);  
 //LCD_Write_Command(LCD_2_LINE_8_BITS); 
  
 //LCD_Write_Command(0x01); 
 LCD_Write_Command(LCD_CLR_DSP); 
 HAL_Delay(1);
 //LCD_Write_Command(0x06); 
 LCD_Write_Command(LCD_CSR_INC); 
 HAL_Delay(1);
 //LCD_Write_Command(0x0c); 
 LCD_Write_Command(LCD_DSP_CSR); 
 
} 
 
void LCD_Clear(void)     
{ 
 LCD_Write_Command(0x01);//  
} 
 
 
void LCD_Write_Command(uchar Com)  
{ 
 //while(LCD_Read_State()); 
 HAL_Delay(10);
// unsigned int Read_Dat = 0; //BJ 
 
  
 HAL_GPIO_WritePin(GPIOB,LCD_RS_Pin, GPIO_PIN_RESET); // LCD_RS = 0; 
 HAL_GPIO_WritePin(GPIOB,LCD_RW_Pin, GPIO_PIN_RESET); // LCD_RW = 0; 
 HAL_GPIO_WritePin(GPIOB,LCD_E_Pin, GPIO_PIN_SET); //LCD_E_Pin = 1; 
 HAL_Delay(1);
  
 LCD_PORT = Com; 
    
 HAL_GPIO_WritePin(GPIOB,LCD_E_Pin, GPIO_PIN_RESET);  //LCD_E_Pin =0; 
 HAL_Delay(1);
 //HAL_GPIO_WritePin(GPIOB,LCD_E_Pin, GPIO_PIN_SET);  //LCD_E_Pin = 1; 
 //Delay_ms(1); 
 //HAL_GPIO_WritePin(GPIOB,LCD_E_Pin, GPIO_PIN_RESET);  //LCD_E_Pin = 0, DB0~DB7 Validate 
  
} 
 
void LCD_Write_Data(uchar dat)  
{ 
 //while(LCD_Read_State()); 
 HAL_Delay(1);
 
 HAL_GPIO_WritePin(GPIOB,LCD_RS_Pin, GPIO_PIN_SET); //LCD_RS = 1; 
 HAL_GPIO_WritePin(GPIOB,LCD_RW_Pin, GPIO_PIN_RESET); // LCD_RW = 0; 
 HAL_GPIO_WritePin(GPIOB,LCD_E_Pin, GPIO_PIN_SET); //LCD_E_Pin = 1; 
 //Delay_ms(1); 
   
 LCD_PORT = dat;  
  
 HAL_GPIO_WritePin(GPIOB,LCD_E_Pin, GPIO_PIN_RESET);  //Set LCD_E = 0; 
  
 HAL_Delay(1);
 
} 
 
 
uchar LCD_Read_State(void) { 
 
 uchar state =0; 
 
 return state; 
} 
 
void LCD_Set_Position(uchar x,uchar y) 
{ 
 if(y==0)  
 { 
  LCD_Write_Command(0x80+x); 
 } 
  
 else if(y==1)  
 { 
  LCD_Write_Command(0xc0+x); 
 } 
 else; 
 
} 
 
void LCD_Display_Char(uchar Char,uchar x,uchar y)  
{ 
LCD_Set_Position(x,y); 
LCD_Write_Data(Char); 
} 
void LCD_Display_String(uchar x,uchar y,uchar *str) 
{ 
//int i=0; 
LCD_Set_Position(x,y);  
//while(str[i] !='\0')    
while(*str !='\0')   
{ 
LCD_Write_Data(*str++); 
} 
} 
/*************end of LCD.c**************/
