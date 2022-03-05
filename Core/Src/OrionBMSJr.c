/***********************************************
* @file OrionBMSJr.c
* @brief Orion BMS Jr Library - UART and CAN API
* @author Oliver Moore
* @version 1.0
* @date 03-05-2022
***********************************************/

/*
 * UART API Testing:
 *
 *   #    	Testing   Complete    	Function
 *  1.1.1 		[x] 		[]		xx
 *  1.1.2 		[x] 		[]		xx
 *  1.1.3 		[x] 		[]		xx
 *  1.1.4 		[x] 		[]		xx
 *  1.1.5 		[x] 		[]		xx
 *  1.1.6 		[x] 		[]		xx
 *  1.1.7 		[x] 		[]		xx
 *
 *  1.1.8 		[x] 		[] 		xx
 *  1.1.9 		[x] 		[]		xx
 *  1.1.10 		[x] 		[]		xx
 *  1.1.11 		[x] 		[]		xx
 *  1.1.12 		[x] 		[]		xx
 *  1.1.13 		[x] 		[]		xx
 *  1.1.14 		[x] 		[]		xx
 *  1.1.15 		[x] 		[]		xx
 *  1.1.16 		[x] 		[]		xx
 *  1.1.17 		[x] 		[]		xx
 *  1.1.18 		[x] 		[]		xx
 *  1.1.19 		[x] 		[]		xx
 *  1.1.20 		[x] 		[]		xx
 *  1.1.21 		[x] 		[]		xx
 *  1.1.22 		[x] 		[]		xx
 *  1.1.23 		[x] 		[]		xx
 *
 *
 * CAN API Testing:
 *   #    	Testing   Complete    	Function
 *  2.1.1 		[x] 		[]		xx
 *  2.1.2 		[x] 		[]		xx
 *  2.1.3 		[x] 		[]		xx
 *  2.1.4 		[x] 		[]		xx
 *  2.1.5 		[x] 		[]		xx
 *  2.1.6 		[x] 		[]		xx
 *  2.1.7 		[x] 		[]		xx
 *  2.1.8 		[x] 		[]		xx
 *  2.1.9 		[x] 		[]		xx
 *  2.1.10 		[x] 		[]		xx
 *  2.1.11 		[x] 		[]		xx
 *  2.1.12 		[x] 		[]		xx
 *  2.1.13 		[x] 		[]		xx
 *  2.1.14 		[x] 		[]		xx
 *  2.1.15 		[x] 		[]		xx
 *  2.1.16 		[x] 		[]		xx
 *  2.1.17 		[x] 		[]		xx
 *  2.1.18 		[x] 		[]		xx
 *  2.1.19 		[x] 		[]		xx
 *
 */

/**************** Header Files ****************/
#include "OrionBMSJr.h"

/*************** Static Function Prototypes **************/

/***************** External Handles ******************/
extern UART_HandleTypeDef huart2;
extern CAN_HandleTypeDef hcan1;

/****************** Orion BMS Jr CAN Identifiers ******************/
//Initialize Orion BMS Jr Request/Response ExtId's to Default
uint32_t OrionBMSExtID_Request = 0;
uint32_t OrionBMSExtID_Response = 0;

/* ********************************
 *  Orion BMS Jr UART Communication API
 * ******************************** */
/* Note1: UART configuration: baudrate x bit/s, x data bits, x stop bit, x parity, x flow control. */


/* ********************************
 *  Orion BMS Jr CAN Communication API
 * ******************************** */
/* Note1: CAN bitrate x kbit/s. Default node ID after firmware update is x. */

