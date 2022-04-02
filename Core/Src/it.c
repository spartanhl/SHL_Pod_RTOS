/***********************************************
* @file it.c
* @brief ISR source file
* @author Oliver Moore
* @version 1.0
* @date 03-19-2022
***********************************************/

#include "it.h"
#include "main.h"

/******************************************************************************/
/*           Cortex-M7 Processor Interruption and Exception Handlers          */
/******************************************************************************/
void NMI_Handler(void) {
	while(1) {}
}

void HardFault_Handler(void) {
	while(1) {}
}

void MemManage_Handler(void) {
	while(1) {}
}

void BusFault_Handler(void) {
	while(1) {}
}

void UsageFault_Handler(void) {
	while(1) {}
}

void DebugMon_Handler(void) {

}

/******************************************************************************/
/* STM32F7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f7xx.s).                    */
/******************************************************************************/

