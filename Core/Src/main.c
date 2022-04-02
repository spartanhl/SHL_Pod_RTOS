/***********************************************
* @file main.c
* @brief Spartan Hyperloop RTOS
* @author Oliver Moore
* @version 1.0
* @date 03-19-2022
***********************************************/

#include "main.h"

static void GPIO_Init(void);
static void USART2_Init(void);
static void USART3_Init(void);

void vTaskA(void *pvParameters);
void vTaskB(void *pvParameters);

//static void vTask1(void* pvParameters);
//static void vTask2(void* pvParameters);

RTC_HandleTypeDef hrtc;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* Stores the handle of the task that will be notified when the transmission is complete. */
static TaskHandle_t xTaskToNotify = NULL;

//Demo
TaskHandle_t xTaskAHandle = NULL;
TaskHandle_t xTaskBHandle = NULL;

//Transmission Tasks
TaskHandle_t xTask1Handle = NULL;
TaskHandle_t xTask2Handle = NULL;

void printmsg(char *msg);
char usr_msg[250] = {0};


/* The index within the target task's array of task notifications
to use. */
const UBaseType_t xArrayIndex = 1;

int main(void) {
	BaseType_t status;

	/* Resets all peripherals, initializes the flash interface and Systick. */
	HAL_Init();

	/* Reconfigure SYSCLK to 108MHz */
	SystemClock_Config_HSI(SYS_CLOCK_FREQ_108MHZ);

	/* Initialize all configured peripherals */
	GPIO_Init();
	USART2_Init();
	USART3_Init();

	vTraceEnable(TRC_START);

	status = xTaskCreate(vTaskA, "TaskA", 500, NULL, 1, &xTaskAHandle);
	configASSERT((status = pdPASS));

	status = xTaskCreate(vTaskB, "TaskB", 500, NULL, 2, &xTaskBHandle);
	configASSERT((status = pdPASS));

	/* status = xTaskCreate(vTask1, "Task1", 500, NULL, 1, &xTask1Handle);
	configASSERT((status = pdPASS));

	status = xTaskCreate(vTask2, "Task2", 500, NULL, 1, &xTask2Handle);
	configASSERT((status = pdPASS)); */

	/* FreeRTOS v10.4.6 */
	vTaskStartScheduler();

	//If the control comes here, then the launch of the scheduler has failed due to insufficient memory in the heap
  	while(1) {}
}

void SystemClock_Config_HSI(uint8_t clock_freq) {
	RCC_OscInitTypeDef osc_init = {0};
	RCC_ClkInitTypeDef clk_init = {0};
	uint8_t flash_latency = 0;

	/* Configure the main internal regulator output voltage */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	/* VOS[1:0] = 0b11 (Scale 1 mode (reset value))
	   Max f_hclk = 180MHz but can be extended to 216MHz by activating over-drive mode.
	   Wait States (WS LATENCY) Table (RM p79) - Refer to Voltage Range: 2.7V - 3.6V
	*/

	/* Using HSI = 16MHz to derive PLL */
	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	osc_init.HSIState = RCC_HSI_ON;
	osc_init.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	osc_init.PLL.PLLState = RCC_PLL_ON;
	osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSI;

	switch(clock_freq) {
	case SYS_CLOCK_FREQ_54MHZ: {
		osc_init.PLL.PLLM = 8;
		osc_init.PLL.PLLN = 54;
		osc_init.PLL.PLLP = RCC_PLLP_DIV2;
		osc_init.PLL.PLLQ = 2;

		clk_init.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
							  RCC_CLOCKTYPE_PCLK1  | RCC_CLOCKTYPE_PCLK2);
		clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1; 	//HCLK = 54MHz (216MHz max)
		clk_init.APB1CLKDivider = RCC_HCLK_DIV1;	//APB1 = 54MHz (54MHz max)
		clk_init.APB2CLKDivider = RCC_HCLK_DIV1;	//APB2 = 54MHz (108MHz max) **can't reach max APB2 clock with this config**
		flash_latency = FLASH_LATENCY_1; //30 < HCLK <= 60
		break;
	}
	case SYS_CLOCK_FREQ_108MHZ: {
		osc_init.PLL.PLLM = 8;
		osc_init.PLL.PLLN = 108;
		osc_init.PLL.PLLP = RCC_PLLP_DIV2;
		osc_init.PLL.PLLQ = 2;

		clk_init.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
							  RCC_CLOCKTYPE_PCLK1  | RCC_CLOCKTYPE_PCLK2);
		clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1; 	//HCLK = 108MHz (216MHz max)
		clk_init.APB1CLKDivider = RCC_HCLK_DIV2; 	//APB1 = 54MHz (54MHz max)
		clk_init.APB2CLKDivider = RCC_HCLK_DIV1; 	//APB2 = 108MHz (108MHz max)
		flash_latency = FLASH_LATENCY_3; //90 < HCLK <= 120
		break;
	}
	case SYS_CLOCK_FREQ_216MHZ: {
		osc_init.PLL.PLLM = 8;
		osc_init.PLL.PLLN = 216;
		osc_init.PLL.PLLP = RCC_PLLP_DIV2;
		osc_init.PLL.PLLQ = 2;

		clk_init.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
							  RCC_CLOCKTYPE_PCLK1  | RCC_CLOCKTYPE_PCLK2);
		clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1; 	//HCLK = 216MHz (216MHz max)
		clk_init.APB1CLKDivider = RCC_HCLK_DIV4; 	//APB1 = 54MHz (54MHz max)
		clk_init.APB2CLKDivider = RCC_HCLK_DIV2; 	//APB2 = 108MHz (108MHz max)
		flash_latency = FLASH_LATENCY_7; //210 < HCLK <= 216
		break;
	}
	default:
		return;
	}

	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK) {
		Error_Handler();
	}

	/* Activate the Over-Drive mode for f_sysclk_max 216MHz*/
	if(clock_freq == SYS_CLOCK_FREQ_216MHZ) {
		if (HAL_PWREx_EnableOverDrive() != HAL_OK) {
			Error_Handler();
		}
	}

	if(HAL_RCC_ClockConfig(&clk_init, flash_latency) != HAL_OK) {
		Error_Handler();
	}
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

static void USART2_Init(void) {
	//SystemView USART interface
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 500000;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_8;
	huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart2) != HAL_OK) {
		Error_Handler();
	}
}

static void USART3_Init(void) {
	//ST-Link USART interface
	huart3.Instance = USART3;
	huart3.Init.BaudRate = 115200;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.Parity = UART_PARITY_NONE;
	huart3.Init.Mode = UART_MODE_TX_RX;
	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart3.Init.OverSampling = UART_OVERSAMPLING_16;
	huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart3) != HAL_OK) {
		Error_Handler();
	}
}

/* Blocked Tasks do not consume any CPU time */
void vTaskA(void* pvParameters) {
	while(1) {

		/* Do something */
		/* Notify TaskB, bringing it out of the Blocked State */
		xTaskNotifyGive(xTaskBHandle);

		/* Place TaskA in the blocked state until it is time to run again */
		vTaskDelay(pdMS_TO_TICKS(200));

		/* Block until TaskB notifies this task to unblock */
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	}
	vTaskDelete(NULL);
}

void vTaskB(void* pvParameters) {
	while(1) {

		/* Do something */

		/* Block until TaskA notifies this task to unblock */
		ulTaskNotifyTake(pdTRUE, (TickType_t)portMAX_DELAY);

		/* Notify TaskA, bringing it out of the blocked state */
		xTaskNotifyGive(xTaskAHandle);


	}
	vTaskDelete(NULL);
}

/* This is an example of a transmit function in a generic
peripheral driver.  An RTOS task calls the transmit function,
then waits in the Blocked state (so not using an CPU time)
until it is notified that the transmission is complete.  The
transmission is performed by a DMA, and the DMA end interrupt
is used to notify the task. */

/*
void vTask1(void* pvParameters) {

	BaseType_t status;

	size_t len = 8;
	uint8_t data = 0x11;

	while(1){
		status = xTaskNotifyWait(0,0,NULL,pdMS_TO_TICKS(400));
		if(status == pdTRUE) {
			//Handle event
			vAFunctionCalledFromATask(data, len);
		} else {
			//Clear errors, or take further action
		}
	}
	vTaskDelete(NULL);
}

void vTask2(void* pvParameters) {

	BaseType_t status;

	size_t len = 8;
	uint8_t data = 0x11;

	while(1){
		status = xTaskNotifyWait(0,0,NULL,pdMS_TO_TICKS(400));
		if(status == pdTRUE) {
			//Handle event
			vAFunctionCalledFromATask(data, len);
		} else {
			//Clear errors, or take further action
		}
	}
	vTaskDelete(NULL);
}
*/

/* The peripheral driver's transmit function. */
void StartTransmission(uint8_t *pcData, size_t xDataLength) {

	/* At this point xTaskToNotify should be NULL as no transmission
    is in progress.  A mutex can be used to guard access to the
    peripheral if necessary. */
    configASSERT( (xTaskToNotify == NULL) );

    /* Store the handle of the calling task. */
    xTaskToNotify = xTaskGetCurrentTaskHandle();

    /* Start the transmission - an interrupt is generated when the
    transmission is complete. */

    //UART or CAN Tx
    //vStartTransmit( pcData, xDataLength );
}
/*-----------------------------------------------------------*/

/* The transmit end interrupt. */
void vTransmitEndISR(void) {
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    /* At this point xTaskToNotify should not be NULL as
    a transmission was in progress. */
    configASSERT( (xTaskToNotify != NULL) );

    /* Notify the task that the transmission is complete. */
    vTaskNotifyGiveIndexedFromISR( xTaskToNotify, xArrayIndex, &xHigherPriorityTaskWoken );

    /* There are no transmissions in progress, so no tasks
    to notify. */
    xTaskToNotify = NULL;

    /* If xHigherPriorityTaskWoken is now set to pdTRUE then a
    context switch should be performed to ensure the interrupt
    returns directly to the highest priority task.  The macro used
    for this purpose is dependent on the port in use and may be
    called portEND_SWITCHING_ISR(). */
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}
/*-----------------------------------------------------------*/

void vAFunctionCalledFromATask(uint8_t ucDataToTransmit, size_t xDataLength) {
	uint32_t ulNotificationValue;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS( 200 );

	/* Start the transmission */
	StartTransmission( &ucDataToTransmit, xDataLength );

	/* Wait to be notified that the transmission is complete.  Note
	the first parameter is pdTRUE, which has the effect of clearing
	the task's notification value back to 0, making the notification
	value act like a binary (rather than a counting) semaphore.  */
	ulNotificationValue = ulTaskNotifyTakeIndexed(xArrayIndex, pdTRUE, xMaxBlockTime);
	if(ulNotificationValue == 1) {
		/* The transmission ended as expected. */
	} else {
		/* The call to ulTaskNotifyTake() timed out. */
	}
}

void printmsg(char *msg) {
	for(uint32_t i = 0; i < strlen(msg); i++) {
		HAL_UART_Transmit(&huart3, (uint8_t*)&msg[i], sizeof(msg[i]), HAL_MAX_DELAY);
	}
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
	if (htim->Instance == TIM6) {
		HAL_IncTick();
	}
}

/*********************************************************************
*
*       vApplicationMallocFailedHook
*
*  Function description
*    Called if a call to pvPortMalloc() fails because there
*    is insufficient free memory available in the FreeRTOS heap.
*    pvPortMalloc() is called internally by FreeRTOS API functions
*    that create tasks, queues, software timers, and semaphores.
*    The size of the FreeRTOS heap is set by the configTOTAL_HEAP_SIZE
*    configuration constant in FreeRTOSConfig.h
*
*/
void vApplicationMallocFailedHook(void) {
	taskDISABLE_INTERRUPTS();
	for(;;);
}

/*********************************************************************
*
*       vApplicationStackOverflowHook
*
*  Function description
*    Run time stack overflow checking is performed if
*    configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.
*    This hook function is called if a stack overflow is detected.
*/
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
	(void) pcTaskName;
	(void) xTask;
	taskDISABLE_INTERRUPTS();
	for(;;);
}

/*********************************************************************
*
*       vApplicationIdleHook
*
*  Function description
*    This function is called on each cycle of the idle task.
*    In this case it does nothing useful, other than report
*    the amount of FreeRTOS heap that remains unallocated.
*
*/
void vApplicationIdleHook(void) {
#if configSUPPORT_DYNAMIC_ALLOCATION == 1
	volatile size_t xFreeHeapSpace;

	xFreeHeapSpace = xPortGetFreeHeapSize();
	if(xFreeHeapSpace > 100) {
		//
		// By now, the kernel has allocated everything it is going to, so
		// if there is a lot of heap remaining unallocated then
		// the value of configTOTAL_HEAP_SIZE in FreeRTOSConfig.h can be
		// reduced accordingly.
	}
#endif
}


/*********************************************************************
*
*       vApplicationTickHook
*
*  Function description
*    A tick hook is used by the "Full" build configuration.
*    The Full and blinky build configurations share a FreeRTOSConfig.h
*    header file, so this simple build configuration also has to define
*    a tick hook - even though it does not actually use it for anything.
*
*/
void vApplicationTickHook(void) {
}

void Error_Handler(void) {
	__disable_irq();
	while(1);
}

void vAssertCalled(const char * const pcFileName, unsigned long ulLine ) {
	volatile uint32_t ulSetToNonZeroInDebuggerToContinue = 0;

    /* Parameters are not used. */
    (void) ulLine;
    (void) pcFileName;

    taskENTER_CRITICAL();
    {
        /* You can step out of this function to debug the assertion by using
        the debugger to set ulSetToNonZeroInDebuggerToContinue to a non-zero
        value. */
        while( ulSetToNonZeroInDebuggerToContinue == 0 )
        {
        }
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
