/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  *Controlling stepper motor by L239D driver and "Black pill", full step, half step.
  *Builded for testing stepper motors.
  *For communication used USB CDC and type C port of "black pill"
  *Need to change  function in file below
  *----------------usbd_cdc_if.c--------------
  **static int8_t CDC_Receive_FS(uint8_t* Buf, uint32_t *Len)
	{
    USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &Buf[0]);
    USBD_CDC_ReceivePacket(&hUsbDeviceFS);
    memset (buffer, '\0', 64);  // clear the buffer
    uint8_t len = (uint8_t)*Len;
    memcpy(buffer, Buf, len);  // copy the data to the buffer
    memset(Buf, '\0', len);   // clear the Buf also
    return (USBD_OK);}

    Used lesson from https://controllerstech.com/send-and-receive-data-to-pc-without-uart-stm32-usb-com/
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"
#include <stdio.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
uint8_t buffer[64]; //receive buffer from USB CDC
int choise;         //way to make choise of test program (speed of rotation)		
int stepdelay;      //delay between steps - test
int8_t thisStep = 0;
char buff[16];      //transmittion buffer
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
void stepper_half_drive();
void stepper_full_drive(int step, int stepdelay);
void stepper_hold(void);
extern uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */
  CDC_Transmit_FS((uint8_t*)"Configured",15);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  //HAL_Delay(200); //delay 100ms

	  if (buffer[strlen(buffer)-2] == '/') { snprintf(buff, sizeof(buff), "%d\r\n\0",strlen(buffer)); // return number of characters in recived buffer
		  	  	  	  	  	  CDC_Transmit_FS(buff, sizeof(buff));   //receiving four bytes with ending character
	  	  	  	  	  	  	  	  	  	 HAL_Delay(1000);}

	  //snprintf(buff, sizeof(buff), "%.1f, %d\r\n\0",degree, TIM4->CNT); //output example of float and int number

	 /* uint32_t num = 0;
	  for(int i=0; i<4; i++)
	  {
	      num <<= 8;
	      num |= outstr[i];
	  }*/
	  //memset (buffer, '\0', 64);





      if (buffer[0] == '1') {choise=1;}
      if (buffer[0] == '2') {choise=2;}
      if (buffer[0] == '3') {choise=3;}
      if (buffer[0] == '4') {choise=4;}
      if (buffer[0] == '9') {choise=9;}

      switch (choise){
	  	  	  case 1:
	  	      stepdelay=10;
	  	      CDC_Transmit_FS((uint8_t*)"Speed test 8\n\r",16);
	  	  		 for(int k=0; k<100; k++) {

	  	  				  /*for (int step=0; step<4; step++)
	  	  								{
	  	  									stepper_half_drive(step);
	  	  					  	  	  	  	//stepper_full_drive(step,stepdelay);

	  	  								}*/
	  	  			  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13); //Toggle the state of pin PC9
	  	  			  stepper_half_drive();
	  	  			  thisStep ++;
	  	  			  HAL_Delay(5000);

	  	  			  }

	  	  	      CDC_Transmit_FS((uint8_t*)"Ready case 1\n\r",16);
	  	  	      memset (buffer, '\0', 64);
	  	  	      //thisStep=0;
	  	  	      choise=0;
				  break;

			  case 2:
			  stepdelay=20;
			  CDC_Transmit_FS((uint8_t*)"Speed test 16\n\r",17);
				 for(int k=0; k<50; k++) {

						  for (int step=0; step<4; step++)
										{
											//stepper_half_drive(step);
											stepper_full_drive(step,stepdelay);

										}
					  }

				  CDC_Transmit_FS((uint8_t*)"Ready case 2\n\r",16);
				  memset (buffer, '\0', 64);
				  choise=0;
				  break;

				  case 3:
				  stepdelay=31;
				  CDC_Transmit_FS((uint8_t*)"Speed test 25\n\r",17);
					 for(int k=0; k<50; k++) {

							  for (int step=0; step<4; step++)
											{
												//stepper_half_drive(step);
												stepper_full_drive(step,stepdelay);

											}
						  }

					  CDC_Transmit_FS((uint8_t*)"Ready case 3\n\r",16);
					  memset (buffer, '\0', 64);
					  choise=0;
					  break;

					  case 4:
					  stepdelay=41;
					  CDC_Transmit_FS((uint8_t*)"Speed test 33\n\r",17);
						 for(int k=0; k<50; k++) {

								  for (int step=0; step<4; step++)
												{
													//stepper_half_drive(step);
													stepper_full_drive(step,stepdelay);

												}
							  }

						  CDC_Transmit_FS((uint8_t*)"Ready case 4\n\r",16);
						  memset (buffer, '\0', 64);
						  choise=0;
						  break;


					  case 9:

					  					  CDC_Transmit_FS((uint8_t*)"HOLDING\n\r",11);
					  						//stepper_hold();

					  					 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 0);   // IN1
										  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0);   // IN2
										  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 0);   // IN3
										  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 0);


					  						HAL_Delay(2000);
					  						  CDC_Transmit_FS((uint8_t*)"FREE\n\r",8);
					  						  memset (buffer, '\0', 64);
					  						  choise=0;
					  						  break;





      }




	  	 //HAL_Delay(3000);


	  	//  stepper_hold();
	  //	HAL_Delay(3000);


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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

void stepper_half_drive ()
{
	switch (thisStep & 0b111){
	case 0:
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 1);   // IN1
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0);   // IN2
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 0);   // IN3
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 0);   // IN4
				  break;

			case 1:
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 1);   // IN1
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0);   // IN2
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 1);   // IN3
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 0);   // IN4
				  break;

			case 2:
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 0);   // IN1
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0);   // IN2
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 1);   // IN3
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 0);   // IN4
				  break;

			case 3:
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 0);   // IN1
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 1);   // IN2
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 1);   // IN3
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 0);   // IN4
				  break;

			case 4:
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 0);   // IN1
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 1);   // IN2
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 0);   // IN3
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 0);   // IN4
				  break;
			case 5:
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 0);   // IN1
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 1);   // IN2
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 0);   // IN3
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 1);   // IN4
				  break;
			case 6:
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 0);   // IN1
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0);   // IN2
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 0);   // IN3
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 1);   // IN4
				  break;
			case 7:
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 1);   // IN1
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0);   // IN2
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 0);   // IN3
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 1);   // IN4
				  break;
	}
}

void stepper_full_drive (int step, int stepdelay)
{
	switch (step){
				case 0:
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 1);
				  HAL_Delay(stepdelay);// IN1
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0);
				  HAL_Delay(stepdelay);// IN2
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 1);
				  HAL_Delay(stepdelay);// IN3
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 0);
				  HAL_Delay(stepdelay);// IN4
				  break;

			case 1:
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 0);
				  HAL_Delay(stepdelay);// IN1
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 1);
				  HAL_Delay(stepdelay);// IN2
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 1);
				  HAL_Delay(stepdelay);// IN3
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 0);
				  HAL_Delay(stepdelay);// IN4
				  break;

			case 2:
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 0);
				  HAL_Delay(stepdelay);// IN1
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 1);
				  HAL_Delay(stepdelay);// IN2
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 0);
				  HAL_Delay(stepdelay);// IN3
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 1);
				  HAL_Delay(stepdelay);// IN4
				  break;

			case 3:
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 1);
				  HAL_Delay(stepdelay);// IN1
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0);
				  HAL_Delay(stepdelay);// IN2
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 0);
				  HAL_Delay(stepdelay);// IN3
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 1);
				  HAL_Delay(stepdelay);// IN4
				  break;
	}
}

void stepper_hold(void){
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 1);   // IN1
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0);   // IN2
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 0);   // IN3
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 1);   // IN4
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
