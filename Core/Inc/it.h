/***********************************************
* @file it.h
* @brief ISR header file
* @author Oliver Moore
* @version 1.0
* @date 03-19-2022
***********************************************/

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void DebugMon_Handler(void);
