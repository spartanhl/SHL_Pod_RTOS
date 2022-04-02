# SHL_Pod_RTOS
### STM32-F7 based RTOS test environment to be used for research and development purposes for the Spartan Hyperloop pod design.

## Brief
Task Notification Demo using FreeRTOS ported to STM32F746xx Arm Cortex-M7.

## Hardware
- STM32 Nucleo-144 Development Board
	- STM32F746ZG MCU with ARM Cortex-M7 processor
- Orion Jr BMS (CAN/UART)
	- Primary Li Battery Pack (14S10P)
- Energus TinyBMS s516 (CAN/UART)
	- Secondary Li Battery Pack (7S20P)
- 2x ElCon Chargers (CAN)
- Sevcon Gen 4 ESC (CAN)
	- PMAC Motor 48V 275A
- Braking System (TBD)
	- Stepper Motors + controllers
- Various Sensors around the Pod (UART/I2C/RS485)

## Middleware/Software
- FreeRTOS v10.4.6
- Percepio Tracealyzer

## IDE + Debug
STM32CubeIDE 1.4.0 with ST-LINK GDB over USART3

## Planned Improvements
- Replace trace Snapshot Mode with Streaming Mode
- Notify task from hardware ISR to read and process CAN/UART network data within the RTOS state machine application
