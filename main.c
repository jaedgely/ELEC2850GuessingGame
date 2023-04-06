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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <time.h>
#include "projectfunctions.h"
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//NAME - JACK EDGELY
//DATE - 4/6/2023
//ELEC2850 - Microcontrollers using C programs
//LED guessing game

//Zero means that they have not guessed the LED. One means they have guessed the LED
uint8_t releasedNumber = 0;
//0 = Blue, 1 = Red, 2 = Orange, 3 = Green
uint8_t ledGuessed[4] = {0, 0, 0, 0};

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
  /* USER CODE BEGIN 2 */
	column_signal_adjuster(0);
	seven_segment_control(10);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, 1);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, 0);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, 1);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, 0);
	delay(DELAY);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, 0);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, 1);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, 0);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, 1);
	delay(DELAY);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, 1);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, 0);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, 1);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, 0);
	delay(DELAY);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, 0);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, 1);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, 0);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, 1);
	delay(DELAY);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

	//correctNumbers is the bank of numbers
	srand(time(0));
	uint8_t correctNumbers[4];
	random_generator(correctNumbers);
	uint8_t streak = 0;
	uint8_t keyPressed = 0;
	uint8_t guessedCorrectly = 0;
	uint8_t wonTheGame = 0; // 0 means still taking input, 1 means WON, 2 means LOST
	uint8_t finalGuessArray[4] = {0, 0, 0, 0};
	while (1)
	{
		while (releasedNumber == 0)
		{
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, ledGuessed[0]);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, ledGuessed[1]);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, ledGuessed[2]);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, ledGuessed[3]);
			victory_or_defeat(wonTheGame);
			while (streak == 4 && wonTheGame == 0){
				if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15) == 1)
				{
					keyPressed = key_finder_function(GPIO_PIN_15);
					seven_segment_control(keyPressed);
					wonTheGame = final_stage(keyPressed, correctNumbers, finalGuessArray);
					while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15) == 1);
				}
				else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) == 1)
				{
					keyPressed = key_finder_function(GPIO_PIN_13);
					seven_segment_control(keyPressed);
					wonTheGame = final_stage(keyPressed, correctNumbers, finalGuessArray);
					while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) == 1);
				}
				else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == 1)
				{
					keyPressed = key_finder_function(GPIO_PIN_11);
					seven_segment_control(keyPressed);
					wonTheGame = final_stage(keyPressed, correctNumbers, finalGuessArray);
					while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == 1);
				}
			}
		}
		while (releasedNumber != 0)
		{
			led_blinker(releasedNumber);
			delay(SHORTDELAY);
			led_blinker(0);
			delay(SHORTDELAY);
			if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15) == 1)
			{
				keyPressed = key_finder_function(GPIO_PIN_15);
				seven_segment_control(keyPressed);
				while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15) == 1);
				guessedCorrectly = guess_evaluator(correctNumbers, streak, keyPressed);
			}
			else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) == 1)
			{
				keyPressed = key_finder_function(GPIO_PIN_13);
				seven_segment_control(keyPressed);
				while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) == 1);
				guessedCorrectly = guess_evaluator(correctNumbers, streak, keyPressed);
			}
			else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == 1)
			{
				keyPressed = key_finder_function(GPIO_PIN_11);
				seven_segment_control(keyPressed);
				while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == 1);
				guessedCorrectly = guess_evaluator(correctNumbers, streak, keyPressed);
			}
			else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == 1)
			{
				keyPressed = key_finder_function(GPIO_PIN_10);
				if (keyPressed == 12) //Fringe case for if the key pressed was the asterisk
				{
					seven_segment_control(correctNumbers[streak]);
					delay(SHORTDELAY);
					seven_segment_control(10);
					continue;
				}
				if (keyPressed == 11) //Fringe case for the hashtag
				{
					continue;
				}
				seven_segment_control(keyPressed);
				guessedCorrectly = guess_evaluator(correctNumbers, streak, keyPressed);
				while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == 1);
			}
			//If the guess_evaluator function returns 1 [the guess was correct]
			//guessedCorrectly gets reset. The global array, ledGuessed, logs that the LED has been guessed
			//Streak is increased by one. The global variable, releasedNumber, is reset.
			if (guessedCorrectly == 1)
			{
				guessedCorrectly = 0;
				ledGuessed[15 - releasedNumber] = 1;
				streak++;
				releasedNumber = 0;
			}
		}
	}

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 50;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
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
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, CS_I2C_SPI_Pin|GPIO_PIN_9|GPIO_PIN_11|GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(OTG_FS_PowerSwitchOn_GPIO_Port, OTG_FS_PowerSwitchOn_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin
                          |GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4
                          |GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pins : CS_I2C_SPI_Pin PE9 PE11 PE13 */
  GPIO_InitStruct.Pin = CS_I2C_SPI_Pin|GPIO_PIN_9|GPIO_PIN_11|GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : OTG_FS_PowerSwitchOn_Pin PC8 PC9 */
  GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PDM_OUT_Pin */
  GPIO_InitStruct.Pin = PDM_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(PDM_OUT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : I2S3_WS_Pin */
  GPIO_InitStruct.Pin = I2S3_WS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
  HAL_GPIO_Init(I2S3_WS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SPI1_SCK_Pin SPI1_MISO_Pin SPI1_MOSI_Pin */
  GPIO_InitStruct.Pin = SPI1_SCK_Pin|SPI1_MISO_Pin|SPI1_MOSI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : BOOT1_Pin */
  GPIO_InitStruct.Pin = BOOT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BOOT1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PB10 PB11 PB13 PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_13|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : LD4_Pin LD3_Pin LD5_Pin LD6_Pin
                           PD1 PD2 PD3 PD4
                           PD5 PD6 PD7 */
  GPIO_InitStruct.Pin = LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin
                          |GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4
                          |GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : I2S3_MCK_Pin I2S3_SCK_Pin I2S3_SD_Pin */
  GPIO_InitStruct.Pin = I2S3_MCK_Pin|I2S3_SCK_Pin|I2S3_SD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : VBUS_FS_Pin */
  GPIO_InitStruct.Pin = VBUS_FS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(VBUS_FS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : OTG_FS_ID_Pin OTG_FS_DM_Pin OTG_FS_DP_Pin */
  GPIO_InitStruct.Pin = OTG_FS_ID_Pin|OTG_FS_DM_Pin|OTG_FS_DP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : Audio_SCL_Pin Audio_SDA_Pin */
  GPIO_InitStruct.Pin = Audio_SCL_Pin|Audio_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	//Need to solve the issue of not allowing numbers to be repeated
	//This can be solved by either making the ledGuessed variable a global variable
	//Potential to solve it by using pointers?
	while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 1)
	{
		led_blinker(12);
	    delay(LONGDELAY);
	    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 0 && ledGuessed[3] == 0)
	    {
	    	releasedNumber = 12;
	    	break;
	    }
	    led_blinker(13);
	    delay(LONGDELAY);
	    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 0 && ledGuessed[2] == 0)
	    {
	    	releasedNumber = 13;
	    	break;
	    }
	    led_blinker(14);
	    delay(LONGDELAY);
	    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 0 && ledGuessed[1] == 0)
	    {
	    	releasedNumber = 14;
	    	break;
	    }
	    led_blinker(15);
	    delay(LONGDELAY);
	    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 0 && ledGuessed[0] == 0)
	    {
	    	releasedNumber = 15;
	    	break;
	    }
	    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 0 && ledGuessed[0] == 1)
	    {
	    	//If the green LED has been guessed
	    	led_blinker(12);
		    delay(LONGDELAY);
	    	if (ledGuessed[3] == 0)
	    	{
	    		releasedNumber = 12;
	    		break;
	    	}
	    	led_blinker(13);
		    delay(LONGDELAY);
	    	if (ledGuessed[2] == 0)
	    	{
	    		releasedNumber = 13;
	    		break;
	    	}
	    	led_blinker(14);
		    delay(LONGDELAY);
	    	if (ledGuessed[1] == 0)
	    	{
	    		releasedNumber = 14;
	    		break;
	    	}
	    	else
	    	{
	    		releasedNumber = 0;
	    		break;
	    	}
	    }
	}
	//If all the LEDs were already guessed
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
