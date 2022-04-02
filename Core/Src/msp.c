/***********************************************
* @file msp.c
* @brief MSP Init and Deinit
* @author Oliver Moore
* @version 1.0
* @date 03-19-2022
***********************************************/

#include "main.h"
#include "FreeRTOS.h"

void HAL_MspInit(void) {

	/* Low level processor specific inits */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_RCC_SYSCFG_CLK_ENABLE();

}

void HAL_UART_MspInit(UART_HandleTypeDef* huart) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

	if(huart->Instance == USART2) {
		/** Initializes the peripherals clock */
		PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART2;
		PeriphClkInitStruct.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
		if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
			Error_Handler();
		}

		/* Peripheral clock enable */
		__HAL_RCC_USART2_CLK_ENABLE();
		__HAL_RCC_GPIOD_CLK_ENABLE();

		/** USART2 GPIO Configuration
		PD5     ------> USART2_TX
		PD6     ------> USART2_RX
		*/
		GPIO_InitStruct.Pin = (USART2_TX_Pin | USART2_RX_Pin);
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
		HAL_GPIO_Init(USART2_GPIO_Port, &GPIO_InitStruct);
	}

	if(huart->Instance == USART3) {
		/** Initializes the peripherals clock */
		PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3;
		PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
		if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
			Error_Handler();
		}

		/* Peripheral clock enable */
		__HAL_RCC_USART3_CLK_ENABLE();
		__HAL_RCC_GPIOD_CLK_ENABLE();

		/** USART3 GPIO Configuration
		PD8     ------> USART3_TX
		PD9   	------> USART3_RX
		*/
		GPIO_InitStruct.Pin = (USART3_TX_Pin | USART3_RX_Pin);
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
		HAL_GPIO_Init(USART3_GPIO_Port, &GPIO_InitStruct);
	}
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart) {
	if(huart->Instance == USART2) {
		/* Peripheral clock disable */
		__HAL_RCC_USART2_CLK_DISABLE();

		/** USART2 GPIO Configuration
		PD5     ------> USART2_TX
		PD6     ------> USART2_RX
		*/
		HAL_GPIO_DeInit(USART2_GPIO_Port, (USART2_TX_Pin | USART2_RX_Pin));

		/* USART2 interrupt DeInit */
		HAL_NVIC_DisableIRQ(USART2_IRQn);
	}

	if(huart->Instance == USART3) {
		/* Peripheral clock disable */
		__HAL_RCC_USART3_CLK_DISABLE();

		/** USART3 GPIO Configuration
		PD8     ------> USART3_TX
		PD9     ------> USART3_RX
		*/
		HAL_GPIO_DeInit(USART3_GPIO_Port, (USART3_TX_Pin | USART3_RX_Pin));

		/* USART3 interrupt DeInit */
		HAL_NVIC_DisableIRQ(USART3_IRQn);
	}
}

void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc) {
	if(hrtc->Instance == RTC) {
		__HAL_RCC_RTC_ENABLE();
	}
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* hrtc) {
	if(hrtc->Instance == RTC) {
		__HAL_RCC_RTC_DISABLE();
	}
}
