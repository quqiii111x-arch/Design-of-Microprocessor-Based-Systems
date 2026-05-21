/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "SCCB.h"
#include "ov7670.h"
#include "stm32f1xx.h"
#include "fifo.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define FRAME_WIDTH    160
#define FRAME_HEIGHT   120
#define FRAME_SIZE     (FRAME_WIDTH * FRAME_HEIGHT)
#define CP_W 20 // compressed width
#define CP_H 15 // compressed height
#define SCALE 8 // compress scale

uint8_t frame_buf[FRAME_SIZE];
volatile uint8_t capture_count = 0;
volatile uint32_t time_elapsed, last_capture_time = 0; 
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
     if (htim == &htim2)
     {
         uint32_t current_time = __HAL_TIM_GET_COUNTER(htim);
         if (current_time < last_capture_time)
				{
						time_elapsed = (htim->Init.Period + current_time - last_capture_time);
				}
				else
				{
						time_elapsed = (current_time - last_capture_time);
				}
         last_capture_time = current_time;
         capture_count++;
    }
}

void Send_Test_Image(void)
{
    //const uint8_t header[2] = {0xAA, 0x55}; 
    //HAL_UART_Transmit(&huart1, (uint8_t *)header, 2, HAL_MAX_DELAY);

    const uint16_t width = 160;
    const uint16_t height = 120;
    const uint32_t total_pixels = width * height;
    uint8_t image_packet[2] = {0x00, 0xF8}; 

    for (uint32_t i = 0; i < total_pixels; i++) {
        HAL_UART_Transmit(&huart1, image_packet, 2, HAL_MAX_DELAY);
    }
		
		//uint8_t tail[2] = {0x55, 0xAA};
    //HAL_UART_Transmit(&huart1, tail, 2, HAL_MAX_DELAY);
}

uint8_t rgb565_to_gray(uint16_t rgb565)
{
    uint8_t r = (rgb565 >> 11) & 0x1F;
    uint8_t g = (rgb565 >> 5) & 0x3F;
    uint8_t b = rgb565 & 0x1F;

    r = (r << 3);
    g = (g << 2);
    b = (b << 3);

    uint8_t gray = (uint8_t)((r * 77 + g * 150 + b * 29) >> 8);
    return gray;
}

#define DETECT_X_START 40
#define DETECT_X_END   120
#define DETECT_Y_START 30
#define DETECT_Y_END   90
#define BLACK_THRESHOLD 50

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_USB_DEVICE_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
	//__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 500); // 50% duty cycle
	//HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); // fifo clock
	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_2); // VSYNC capture
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2); // servo 1
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3); // servo 2
	
	OV7670_Init(&hi2c1);
	FIFO_Init();
	//char test[] = "Hello\r\n";
	//HAL_UART_Transmit(&huart1, (uint8_t*)test, strlen(test), HAL_MAX_DELAY);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
	#define OFFSET_PIXELS 2
	
  while (1)
  {
		//Send_Test_Image();
			
    while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == GPIO_PIN_RESET); // VSYNC = 1
    while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == GPIO_PIN_SET);   // VSYNC = 0

    FIFO_StartReadFrame();

    //uint8_t head[2] = {0xAA, 0x55};
    //HAL_UART_Transmit(&huart1, head, 2, HAL_MAX_DELAY);
		
		uint8_t compressed[300] = {0}; // 160*120 -> 20*15

		
    for (uint32_t i = 0; i < FRAME_SIZE; i++)
    {
      uint8_t byte = FIFO_ReadByte();
			//if (i < OFFSET_PIXELS) continue;
			//uint8_t buf[2] = {(uint8_t)(byte & 0xFF), (uint8_t)(byte >> 8)};
			//HAL_UART_Transmit(&huart1, buf, 2, HAL_MAX_DELAY);
      HAL_UART_Transmit(&huart1, &byte, 1, HAL_MAX_DELAY);
			// compressing
			compressed[(i / FRAME_WIDTH / SCALE ) * CP_W + (i % FRAME_WIDTH)/ SCALE] 
						+= (uint8_t)((byte >= 128) ? 0x03 : 0x00 );
    }
		
		
		//for (uint16_t i = 0; i < 300; i++)
		//{
		//	HAL_UART_Transmit(&huart1, &compressed[i], 1, HAL_MAX_DELAY);
		//}
    
		//uint8_t tail[2] = {0x55, 0xAA};
    //HAL_UART_Transmit(&huart1, tail, 2, HAL_MAX_DELAY);
		
		uint8_t black_detected = 0;

		for (uint32_t y = 0; y < FRAME_HEIGHT; y++)
		{
				for (uint32_t x = 0; x < FRAME_WIDTH; x++)
				{
						uint8_t high = FIFO_ReadByte();
						uint8_t low  = FIFO_ReadByte();
						uint16_t pixel = (high << 8) | low;

						uint8_t gray = rgb565_to_gray(pixel);

						if (x >= DETECT_X_START && x < DETECT_X_END &&
								y >= DETECT_Y_START && y < DETECT_Y_END)
						{
								if (gray <= BLACK_THRESHOLD)
								{
										black_detected = 1;
								}
						}

				}
		}

		if (black_detected)
		{
				const char *msg = "Black detected\r\n";
				HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
		}
		
		//vsync count output
		HAL_UART_Transmit(&huart1, (uint8_t*)&capture_count, 1, HAL_MAX_DELAY);
		capture_count = 0;
		
    HAL_Delay(10);
			
		
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
