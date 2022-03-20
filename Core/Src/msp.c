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

	//1. Set up the priority grouping of the arm cortex mx processor
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4); //default setting

	//2. Enable the required system exceptions of the arm cortex mx processor
	//System Control Block (SCB) -> System Handler Control and State Register (SHCSR)
	SCB->SHCSR |= (0x7 << 16);  //Set bits 16,17,18 (MEMFAULTENA, BUSFAULTENA, USGFAULTENA)

	//3. Configure the priority for the system exceptions
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	if(huart->Instance == USART2) {
		__HAL_RCC_USART2_CLK_ENABLE();
		__HAL_RCC_GPIOD_CLK_ENABLE();

		/** TinyBMS (USART2)
		 *  USART2 GPIO Configuration
			PD5     ------> USART2_TX
			PD6     ------> USART2_RX
		*/

		GPIO_InitStruct.Pin = (USART2_TX_Pin | USART2_RX_Pin);
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
		HAL_GPIO_Init(USART2_GPIO_Port, &GPIO_InitStruct);

		HAL_NVIC_SetPriority(USART2_IRQn, 15, 0);
		HAL_NVIC_EnableIRQ(USART2_IRQn);

	} else if(huart->Instance == USART3) {
		__HAL_RCC_USART3_CLK_ENABLE();
		__HAL_RCC_GPIOD_CLK_ENABLE();

		/** ST-LINK Debug (USART3)
		 *  USART3 GPIO Configuration
			PD8     ------> USART3_TX
			PD9     ------> USART3_RX
		*/
		GPIO_InitStruct.Pin = (USART3_TX_Pin | USART3_RX_Pin);
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
		HAL_GPIO_Init(USART3_GPIO_Port, &GPIO_InitStruct);
	}
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *huart) {
	if(huart->Instance == USART2) {
		__HAL_RCC_USART2_CLK_DISABLE();

		/** TinyBMS (USART2)
		 *  USART2 GPIO Configuration
			PD5     ------> USART2_TX
			PD6     ------> USART2_RX
		*/
		HAL_GPIO_DeInit(USART2_GPIO_Port, (USART2_TX_Pin | USART2_RX_Pin));
		HAL_NVIC_DisableIRQ(USART2_IRQn);

	} else if(huart->Instance == USART3) {
		__HAL_RCC_USART3_CLK_DISABLE();

		/** ST-LINK Debug (USART3)
		 *  USART3 GPIO Configuration
			PD8     ------> USART3_TX
			PD9     ------> USART3_RX
		*/
		HAL_GPIO_DeInit(USART3_GPIO_Port, (USART3_TX_Pin | USART3_RX_Pin));
	}
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
	if(htim->Instance == TIM6) {
		//1. Enable TIM6 Clock
		__HAL_RCC_TIM6_CLK_ENABLE();

		//2. Enable TIM6 IRQ
		HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);

		//3. Setup TIM6_DAC_IRQn priority
		HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 15, 0);
	}
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim) {
	if(htim->Instance == TIM6) {
		__HAL_RCC_TIM6_CLK_DISABLE();
		HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);
	}
}
