/***********************************************
* @file main.c
* @brief Spartan Hyperloop RTOS
* @author Oliver Moore
* @version 1.0
* @date 03-19-2022
***********************************************/

/*
 * Couple Ways to use Continuous Recording feature of SEGGER SystemView:
 *
 * 1) J-link based debug probe is required. The ST-link debug probe on the STM32-F7 nucleo board can be configured
 *    by changing the ST link firmware to J-link by using the ST-Link reflash utility provided by SEGGER. This method is
 *    not very useful because it removes the ability to flash code and debug the application using STM32CUBEIDE ST-Link.
 *
 * 2) RT Recording over UART communication (USART2 PA3, PD5) <---UART-to-USB---> Virtual COM to SystemViewer.
 * 	  **Nucleo boards do not require a UART to USB converter because it supports on-board virtual COM port (VCP)**
 */
#include "main.h"

void SystemClock_Config_HSI(uint8_t clock_freq);
void Error_Handler(void);

static void GPIO_Init(void);
static void UART_Init(void);
static void RTC_Init(void);
static void task1_handler(void* parameters);
static void task2_handler(void* parameters);

extern void SEGGER_UART_init(uint32_t);

RTC_HandleTypeDef hrtc;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

TaskHandle_t task1_handle = NULL;
TaskHandle_t task2_handle = NULL;

void printmsg(char *msg);
char usr_msg[250] = {0};

int main(void) {

	BaseType_t status;

	/* Resets all peripherals, initializes the flash interface and Systick. */
	HAL_Init();

	/* Configure SYSCLK to 50MHZ */
	SystemClock_Config_HSI(SYS_CLOCK_FREQ_50MHZ); //Should increase freq for SEGGER SystemView

	/* Initialize all configured peripherals */
	GPIO_Init();
	RTC_Init();
	UART_Init();

	/* Segger SystemView v3.30 Related Config */
	//SEGGER Event Time-Stamps
	//Enable the CYCCNT counter (to assist with timestamp logging for SEGGER SystemView)
	DWT_CTRL |= (1 << 0);

	SEGGER_UART_init(500000); //uncomment if using USART based continuous recording
	SEGGER_SYSVIEW_Conf();
	//SEGGER_SYSVIEW_Start(); //comment out if using UART based continuous recording
	//traceSTART();

	/* Task Creation */
	status = xTaskCreate(task1_handler, "Task-1", 200, "Hello World from Task-1", 2, &task1_handle);
	configASSERT(status == pdPASS);

	status = xTaskCreate(task2_handler, "Task-2", 200, "Hello World from Task-2", 2, &task2_handle);
	configASSERT(status == pdPASS);

	/* FreeRTOS v10.4.6 */
	vTaskStartScheduler();

	//If the control comes here, then the launch of the scheduler has failed due to insufficient memory in the heap
	sprintf(usr_msg, " Failed... \r\n");
	printmsg(usr_msg);

  	while(1) {}
}

void SystemClock_Config_HSI(uint8_t clock_freq) {
	RCC_OscInitTypeDef osc_init = {0};
	RCC_ClkInitTypeDef clk_init = {0};
	uint8_t flash_latency = 0;

	//Using HSI to derive PLL
	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	osc_init.HSIState = RCC_HSI_ON;
	osc_init.PLL.PLLState = RCC_PLL_ON;
	osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSI;

	/** Activate the Over-Drive mode
	if (HAL_PWREx_EnableOverDrive() != HAL_OK) {
		Error_Handler();
	}
	*/
	switch(clock_freq) {
	case SYS_CLOCK_FREQ_50MHZ: {
		osc_init.PLL.PLLM = 16;
		osc_init.PLL.PLLN = 100;
		osc_init.PLL.PLLP = RCC_PLLP_DIV2;
		osc_init.PLL.PLLQ = 2;

		clk_init.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
							  RCC_CLOCKTYPE_PCLK1  | RCC_CLOCKTYPE_PCLK2);
		clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
		clk_init.APB2CLKDivider = RCC_HCLK_DIV2;
		flash_latency = 1;
		break;
	}
	case SYS_CLOCK_FREQ_84MHZ: {
		osc_init.PLL.PLLM = 16;
		osc_init.PLL.PLLN = 168;
		osc_init.PLL.PLLP = RCC_PLLP_DIV2;
		osc_init.PLL.PLLQ = 2;

		clk_init.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
							  RCC_CLOCKTYPE_PCLK1  | RCC_CLOCKTYPE_PCLK2);
		clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
		clk_init.APB2CLKDivider = RCC_HCLK_DIV2;
		flash_latency = 2;
		break;
	}
	case SYS_CLOCK_FREQ_120MHZ: {
		osc_init.PLL.PLLM = 16;
		osc_init.PLL.PLLN = 240;
		osc_init.PLL.PLLP = RCC_PLLP_DIV2;
		osc_init.PLL.PLLQ = 2;

		clk_init.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
							  RCC_CLOCKTYPE_PCLK1  | RCC_CLOCKTYPE_PCLK2);
		clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		clk_init.APB1CLKDivider = RCC_HCLK_DIV4;
		clk_init.APB2CLKDivider = RCC_HCLK_DIV2;
		flash_latency = 3;
		break;
	}
	default:
		return;
	}

	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK) {
		Error_Handler();
	}

	if(HAL_RCC_ClockConfig(&clk_init, flash_latency) != HAL_OK) {
		Error_Handler();
	}

	//Configure the SYSTICK timer interrupt frequency for every 1ms
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);
	//Configure SYSTICK
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	//SYSTICK IRQn interrupt configuration
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

static void GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();

	/* Configure LED GPIO pin Output Level */
	HAL_GPIO_WritePin(LED_GPIO_Port, (LED1_Pin | LED2_Pin | LED3_Pin), GPIO_PIN_RESET);

	/* Configure User Button GPIO pin : USER_Btn_Pin */
	GPIO_InitStruct.Pin = USER_Btn_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

	/* Configure GPIO pins : LED1_Pin LED2_Pin LED3_Pin */
	GPIO_InitStruct.Pin = (LED1_Pin | LED2_Pin | LED3_Pin);
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 6, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

static void UART_Init(void) {
	/* USART2: SEGGER SystemView PD5 PD6 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if(HAL_UART_Init(&huart2) != HAL_OK) {
		Error_Handler();
	}

	/* USART3: PD8 PD9 for ST-LINK debugging (printf ITM) */
	huart3.Instance = USART3;
	huart3.Init.BaudRate = 115200;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.Parity = UART_PARITY_NONE;
	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart3.Init.Mode = UART_MODE_TX_RX;
	huart3.Init.OverSampling = UART_OVERSAMPLING_16;
	huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if(HAL_UART_Init(&huart3) != HAL_OK) {
		Error_Handler();
	}
}

static void RTC_Init(void) {

	/* Initialize RTC Only */
	hrtc.Instance = RTC;
	hrtc.Init.HourFormat = RTC_HOURFORMAT_12;
	hrtc.Init.AsynchPrediv = 127;
	hrtc.Init.SynchPrediv = 255;
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	if(HAL_RTC_Init(&hrtc) != HAL_OK) {
		Error_Handler();
	}
}

static void task1_handler(void* parameters) {

	char msg[100];
	while(1) {
		SEGGER_SYSVIEW_PrintfTarget("Task 1");
		printf("%s\n", (char*)parameters);

		//Formatting string before using SEGGER's Printf
		snprintf(msg, 100, "%s\n", (char*)parameters);
		SEGGER_SYSVIEW_PrintfTarget(msg);

		taskYIELD(); //cooperative scheduling with configUSE_PREEMPTION = 0
	}

	/* Clean up before exiting */
	vTaskDelete(NULL);
}

static void task2_handler(void* parameters) {

	char msg[100];
	while(1) {
		SEGGER_SYSVIEW_PrintfTarget("Task 2");
		printf("%s\n", (char*)parameters);

		//Formatting string before using SEGGER's Printf
		snprintf(msg, 100, "%s\n", (char*)parameters);
		SEGGER_SYSVIEW_PrintfTarget(msg);

		taskYIELD(); //cooperative scheduling with configUSE_PREEMPTION = 0
	}

	/* Clean up before exiting */
	vTaskDelete(NULL);
}

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM6 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	/* USER CODE BEGIN Callback 0 */

	/* USER CODE END Callback 0 */
	if (htim->Instance == TIM6) {
		HAL_IncTick();
	}
	/* USER CODE BEGIN Callback 1 */

	/* USER CODE END Callback 1 */
}

void printmsg(char *msg) {
	for(uint32_t i = 0; i < strlen(msg); i++) {
		HAL_UART_Transmit(&huart2, (uint8_t*)&msg[i], sizeof(msg[i]), HAL_MAX_DELAY);
	}
}

void Error_Handler(void) {
	__disable_irq();
	while(1);
}

void vAssertCalled(unsigned long ulLine, const char * const pcFileName) {
	volatile uint32_t ulSetToNonZeroInDebuggerToContinue = 0;

    /* Parameters are not used. */
    (void)ulLine;
    (void)pcFileName;

    taskENTER_CRITICAL(); {
        /* You can step out of this function to debug the assertion by using
        the debugger to set ulSetToNonZeroInDebuggerToContinue to a non-zero
        value. */
        while( ulSetToNonZeroInDebuggerToContinue == 0 ) {}
    }
    taskEXIT_CRITICAL();
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line) {
  /* USER CODE BEGIN 6 */
  /* User can add their own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
