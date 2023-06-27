/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "adc.h"
#include "usart.h"
#include "rtc.h"
#include "ucpd.h"
#include "usb.h"
#include "gpio.h"
#include "dma.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "LOS_D_UART.hpp"
#include <cstring>
#include <stdio.h>
#include <stdarg.h> //for va_list var arg functions
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
UARTDevice* p_uart_dev1;
UARTDevice* p_uart_dev2;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void myprintf(const char *fmt, ...);
void ping_pong_test();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void myprintf(const char *fmt, ...)
{
	static char buffer[256];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, args);
	va_end(args);

	int len = strlen(buffer);
	HAL_UART_Transmit(&hlpuart1, (uint8_t*)buffer, len, -1);
}

void ping_pong_test()
{
	const uint16_t buf_size = UARTDevice::BUFFER_SIZE;
	uint8_t buf[buf_size];
	uint16_t data_size = 0;

	const uint8_t max_test_iterations = 5;

	const char* ping_msg = "Ping";
	const char* pong_msg = "Pong";

	for (uint8_t test_iterations = 0; test_iterations < max_test_iterations; test_iterations++) {
		myprintf("Test Iteration #%d\r\n", test_iterations + 1);

		// Send ping
		p_uart_dev1->transmit((uint8_t*)ping_msg, strlen(ping_msg));
		myprintf("UART Device 1: ping sent.\r\n");

		// Receive ping
		data_size = 0;
		while (data_size == 0) {
			data_size = p_uart_dev2->getAvailDataSize();
			HAL_Delay(10);
		}

		memset(buf, 0, buf_size);
		p_uart_dev2->read(buf, data_size);
		if (strcmp((char*)buf, ping_msg) == 0) {
			myprintf("UART Device 2: ping received.\r\n");
		} else {
			myprintf("UART Device 2: invalid message received.\r\n");
			break;
		}

		// Send pong
		p_uart_dev2->transmit((uint8_t*)pong_msg, strlen(pong_msg));
		myprintf("UART Device 2: pong sent.\r\n");

		// Receive pong
		data_size = 0;
		while (data_size == 0) {
			data_size = p_uart_dev1->getAvailDataSize();
			HAL_Delay(10);
		}

		memset(buf, 0, buf_size);
		p_uart_dev1->read(buf, data_size);
		if (strcmp((char*)buf, pong_msg) == 0) {
			myprintf("UART Device 1: pong received.\r\n");
		}
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	UARTDevice uart_dev1(&huart2);
	UARTDevice uart_dev2(&huart3);
	p_uart_dev1 = &uart_dev1;
	p_uart_dev2 = &uart_dev2;
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
  MX_DMA_Init();
  MX_LPUART1_UART_Init();
  MX_RTC_Init();
  MX_UCPD1_Init();
  MX_USB_PCD_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */

  // Run test
  myprintf("Test Start\r\n");
  p_uart_dev1->init();
  p_uart_dev2->init();
  HAL_Delay(1000);
  ping_pong_test();
  myprintf("Test End\r\n");

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE0) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_LSE
                              |RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 55;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size){
    if (p_uart_dev1->matchUART(huart)) {
    	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7,GPIO_PIN_SET); //turn on green light

    	p_uart_dev1->callback(size);

        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7,GPIO_PIN_RESET); //turn off green light
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,GPIO_PIN_RESET); //turn off red light
    }

    if (p_uart_dev2->matchUART(huart)) {
    	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7,GPIO_PIN_SET); //turn on green light

    	p_uart_dev2->callback(size);

        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7,GPIO_PIN_RESET); //turn off green light
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,GPIO_PIN_RESET); //turn off red light
    }
}

 void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
 {
    if(p_uart_dev1->matchUART(huart)){
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,GPIO_PIN_SET); //turn on blue light
        /*
            should never enter this callback
        */
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,GPIO_PIN_RESET); //turn off blue light
    }

    if(p_uart_dev2->matchUART(huart)){
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,GPIO_PIN_SET); //turn on blue light
        /*
            should never enter this callback
        */
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,GPIO_PIN_RESET); //turn off blue light
    }
 }

 void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
 {
    if(p_uart_dev1->matchUART(huart)){
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,GPIO_PIN_SET); //turn on red light
        HAL_UART_DMAStop(huart);
        p_uart_dev1->callback(UARTDevice::BUFFER_SIZE);
    }

    if(p_uart_dev2->matchUART(huart)){
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,GPIO_PIN_SET); //turn on red light
        HAL_UART_DMAStop(huart);
        p_uart_dev2->callback(UARTDevice::BUFFER_SIZE);
    }
 }
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
