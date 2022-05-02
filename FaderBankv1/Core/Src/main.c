/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "touchsensing.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "fader_bank.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SBUF_SIZE (128)
#define DETAILED_TSC_DATA (0)
	
#define SERIAL_HANDLE (&huart3)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define LINEAR_DETECT(chan) ((MyLinRots[chan].p_Data->StateId == TSL_STATEID_DETECT) || \
                       (MyLinRots[chan].p_Data->StateId == TSL_STATEID_DEB_RELEASE_DETECT))
#define LINEAR_POSITION(chan) (MyLinRots[chan].p_Data->Position)

#define KEY_DETECT(chan) ((MyTKeys[chan].p_Data->StateId == TSL_STATEID_DETECT) || \
                       (MyTKeys[chan].p_Data->StateId == TSL_STATEID_DEB_RELEASE_DETECT))

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 I2C_HandleTypeDef hi2c1;

TSC_HandleTypeDef htsc;

UART_HandleTypeDef huart3;

PCD_HandleTypeDef hpcd_USB_FS;

/* USER CODE BEGIN PV */
char sbuf[SBUF_SIZE];
uint8_t pos;
int size;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_TSC_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USB_PCD_Init(void);
/* USER CODE BEGIN PFP */

static void PrintLinRotDetails(int chan);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void PrintLinRotDetails(int chan) {
		size = sprintf(sbuf, "slider %d: %d\r\n", chan, MyLinRots[chan].p_Data->Position);
		HAL_UART_Transmit(SERIAL_HANDLE, (uint8_t *)sbuf, size, 5000);
		size = sprintf(sbuf, "\tdelta 1: %d\r\n", MyLinRots[chan].p_ChD[0].Delta);
		HAL_UART_Transmit(SERIAL_HANDLE, (uint8_t *)sbuf, size, 5000);
		size = sprintf(sbuf, "\tdelta 2: %d\r\n", MyLinRots[chan].p_ChD[1].Delta);
		HAL_UART_Transmit(SERIAL_HANDLE, (uint8_t *)sbuf, size, 5000);
		size = sprintf(sbuf, "\tdelta 3: %d\r\n", MyLinRots[chan].p_ChD[2].Delta);
		HAL_UART_Transmit(SERIAL_HANDLE, (uint8_t *)sbuf, size, 5000);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  tsl_user_status_t tsl_status;

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
	InitFaderBank();

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_TSC_Init();
  MX_USART3_UART_Init();
  MX_USB_PCD_Init();
  MX_TOUCHSENSING_Init();
	
  /* USER CODE BEGIN 2 */
	
	// Hello, World ...
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0);
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_1);
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
	
	size = sprintf(sbuf, "Hello World!\r\n");
	HAL_UART_Transmit(&huart3, (uint8_t *)sbuf, size, 5000);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
	uint8_t tkey_status = 0;
	HAL_GPIO_WritePin(GPIOC, TKEY_LED_Pin, GPIO_PIN_RESET);
	
  while (1)
  {	
		tsl_status = tsl_user_Exec();
		if (tsl_status != TSL_USER_STATUS_BUSY)
		{
			for (int chan = 0; chan < TSLPRM_TOTAL_LINROTS; chan++) {
				if (LINEAR_DETECT(chan)) {
					HAL_GPIO_WritePin(GPIOC, LED2_Pin, GPIO_PIN_SET);
					pos = LINEAR_POSITION(chan);
					
					
					SET_FADER_POSITION(chan, pos);
					// TODO: UpdateSliderPosition(pos);

					if (DETAILED_TSC_DATA) {
						PrintLinRotDetails(chan);
					} else {
						size = sprintf(sbuf, "slider %d: %d\r\n", chan, FADER_POSITION(chan));
						HAL_UART_Transmit(SERIAL_HANDLE, (uint8_t *)sbuf, size, 5000);						
					}
				} else {
					HAL_GPIO_WritePin(GPIOC, LED2_Pin, GPIO_PIN_RESET);
				}
			}
			
			if (KEY_DETECT(0)) {
				if (tkey_status) {
					// do nothing	
				} else {
					tkey_status = 1;
					HAL_GPIO_WritePin(GPIOC, TKEY_LED_Pin, GPIO_PIN_SET);
				}
			} else {
				if (tkey_status) {
					tkey_status = 0;
					HAL_GPIO_WritePin(GPIOC, TKEY_LED_Pin, GPIO_PIN_RESET);
				}
			}
			
			
//			if (LINEAR_DETECT(0))
//			{
//				HAL_GPIO_WritePin(GPIOC, LED1_Pin, GPIO_PIN_SET);

//				pos = LINEAR_POSITION(0);
//				
//				size = sprintf(sbuf, "slider 0: %d\r\n", pos);
//				HAL_UART_Transmit(&huart3, (uint8_t *)sbuf, size, 5000);
//				
//			} else {
//				HAL_GPIO_WritePin(GPIOC, LED1_Pin, GPIO_PIN_RESET);
//			}
//			
//			if (LINEAR_DETECT(1)) {
//				HAL_GPIO_WritePin(GPIOC, LED2_Pin, GPIO_PIN_SET);

//				pos = LINEAR_POSITION(1);
//				
//				size = sprintf(sbuf, "slider 1: %d\r\n", pos);
//				HAL_UART_Transmit(&huart3, (uint8_t *)sbuf, size, 5000);
//				size = sprintf(sbuf, "\tdelta 1: %d\r\n", MyLinRots[1].p_ChD[0].Delta);
//				HAL_UART_Transmit(&huart3, (uint8_t *)sbuf, size, 5000);
//				size = sprintf(sbuf, "\tdelta 2: %d\r\n", MyLinRots[1].p_ChD[1].Delta);
//				HAL_UART_Transmit(&huart3, (uint8_t *)sbuf, size, 5000);
//				size = sprintf(sbuf, "\tdelta 3: %d\r\n", MyLinRots[1].p_ChD[2].Delta);
//				HAL_UART_Transmit(&huart3, (uint8_t *)sbuf, size, 5000);
//				
//				
//			} else {
//				HAL_GPIO_WritePin(GPIOC, LED2_Pin, GPIO_PIN_RESET);
//			}			
		}			
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x2000090E;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TSC Initialization Function
  * @param None
  * @retval None
  */
static void MX_TSC_Init(void)
{

  /* USER CODE BEGIN TSC_Init 0 */

  /* USER CODE END TSC_Init 0 */

  /* USER CODE BEGIN TSC_Init 1 */

  /* USER CODE END TSC_Init 1 */

  /** Configure the TSC peripheral
  */
  htsc.Instance = TSC;
  htsc.Init.CTPulseHighLength = TSC_CTPH_2CYCLES;
  htsc.Init.CTPulseLowLength = TSC_CTPL_2CYCLES;
  htsc.Init.SpreadSpectrum = DISABLE;
  htsc.Init.SpreadSpectrumDeviation = 1;
  htsc.Init.SpreadSpectrumPrescaler = TSC_SS_PRESC_DIV1;
  htsc.Init.PulseGeneratorPrescaler = TSC_PG_PRESC_DIV4;
  htsc.Init.MaxCountValue = TSC_MCV_8191;
  htsc.Init.IODefaultMode = TSC_IODEF_OUT_PP_LOW;
  htsc.Init.SynchroPinPolarity = TSC_SYNC_POLARITY_FALLING;
  htsc.Init.AcquisitionMode = TSC_ACQ_MODE_NORMAL;
  htsc.Init.MaxCountInterrupt = DISABLE;
  htsc.Init.ChannelIOs = TSC_GROUP1_IO1|TSC_GROUP1_IO2|TSC_GROUP2_IO1|TSC_GROUP2_IO2
                    |TSC_GROUP3_IO1|TSC_GROUP3_IO2|TSC_GROUP3_IO3|TSC_GROUP4_IO2
                    |TSC_GROUP5_IO2|TSC_GROUP5_IO3|TSC_GROUP5_IO4|TSC_GROUP6_IO2
                    |TSC_GROUP6_IO3;
  htsc.Init.ShieldIOs = 0;
  htsc.Init.SamplingIOs = TSC_GROUP1_IO4|TSC_GROUP2_IO4|TSC_GROUP3_IO4|TSC_GROUP4_IO1
                    |TSC_GROUP5_IO1|TSC_GROUP6_IO1;
  if (HAL_TSC_Init(&htsc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TSC_Init 2 */

  /* USER CODE END TSC_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 38400;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USB Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_PCD_Init(void)
{

  /* USER CODE BEGIN USB_Init 0 */

  /* USER CODE END USB_Init 0 */

  /* USER CODE BEGIN USB_Init 1 */

  /* USER CODE END USB_Init 1 */
  hpcd_USB_FS.Instance = USB;
  hpcd_USB_FS.Init.dev_endpoints = 8;
  hpcd_USB_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_FS.Init.battery_charging_enable = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_Init 2 */

  /* USER CODE END USB_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LED_SDB_B_Pin|LED_SDB_C_Pin|LED_SDB_D_Pin|LED_SDB_A_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LED1_Pin|LED2_Pin|TKEY_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LED_SDB_B_Pin LED_SDB_C_Pin LED_SDB_D_Pin LED1_Pin
                           LED2_Pin TKEY_LED_Pin LED_SDB_A_Pin */
  GPIO_InitStruct.Pin = LED_SDB_B_Pin|LED_SDB_C_Pin|LED_SDB_D_Pin|LED1_Pin
                          |LED2_Pin|TKEY_LED_Pin|LED_SDB_A_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : B_USER_Pin */
  GPIO_InitStruct.Pin = B_USER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(B_USER_GPIO_Port, &GPIO_InitStruct);

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
