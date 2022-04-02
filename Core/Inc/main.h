/***********************************************
* @file main.h
* @brief Spartan Hyperloop RTOS
* @author Oliver Moore
* @version 1.0
* @date 03-19-2022
***********************************************/

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "stm32f7xx_hal.h"


/* FreeRTOS v10.4.6 */
#include "FreeRTOS.h"
#include "task.h"

/*************** Macros ***************/
#define CMD_SUCCESS				0xAA
#define CMD_FAILURE				0xFF

#define SYS_CLOCK_FREQ_54MHZ 	54
#define SYS_CLOCK_FREQ_108MHZ 	108
#define SYS_CLOCK_FREQ_216MHZ 	216

#define FALSE 					0
#define TRUE 					1


/************  GPIO Macros ************/
/* HSE Bypass */
#define MCO_Pin GPIO_PIN_0
#define MCO_GPIO_Port GPIOH

/* Nucleo-144 onboard LED1,2,3 */
#define LED1_Pin GPIO_PIN_0
#define LED2_Pin GPIO_PIN_7
#define LED3_Pin GPIO_PIN_14
#define LED_GPIO_Port GPIOB

/* Nucleo-144 onboard user button */
#define USER_Btn_Pin GPIO_PIN_13
#define USER_Btn_GPIO_Port GPIOC

/* Serial Wire Debug (SWD) */
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SW0_Pin GPIO_PIN_3
#define SW0_GPIO_Port GPIOB

/* USART2: PD5 PD6 for SEGGER SystemView */
#define USART2_TX_Pin GPIO_PIN_5
#define USART2_RX_Pin GPIO_PIN_6
#define USART2_GPIO_Port GPIOD

/* USART3: PD8 PD9 for ST-LINK Debug */
#define USART3_TX_Pin GPIO_PIN_8
#define USART3_RX_Pin GPIO_PIN_9
#define USART3_GPIO_Port GPIOD

#define STLK_TX_Pin USART3_RX_Pin
#define STLK_RX_Pin USART3_TX_Pin
#define STLK_GPIO_Port USART3_GPIO_Port

/* CAN1 */
#define CAN1_RX_PIN	GPIO_PIN_0
#define CAN1_TX_PIN	GPIO_PIN_1
#define CAN1_GPIO_Port GPIOD

void SystemClock_Config_HSI(uint8_t clock_freq);
void printmsg(char *msg);
void Error_Handler(void);

void vApplicationMallocFailedHook(void);
void vApplicationIdleHook(void);
void vApplicationTickHook(void);

void StartTransmission(uint8_t *pcData, size_t xDataLength);
void vTransmitEndISR(void);
void vAFunctionCalledFromATask(uint8_t ucDataToTransmit, size_t xDataLength);
