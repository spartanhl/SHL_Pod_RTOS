# SHL_Pod_PrimaryBMS
### STM32 Nucleo-F746xx test environment with a library of supported API for the Primary BMS: *Orion BMS Jr*


## Connections / Wiring
**STM32 Nucleo-F746ZG**    <-**UART**->   **Orion BMS Jr** \
							  or \
**STM32 Nucleo-F746ZG**   <-**CANdapter**->  **Orion BMS Jr**

ElCon Charger, Orion BMS Jr (via CANdapter), and STM32F7 communication via CAN2.0b (extended ID)

## Battery Pack Configuration
**Primary Pack Characteristics** \
48V 26Ah 18C (14S 10P)

**Individual Cell**  \
*Sony US18650VTC5A* - Lithium Ion "18650" cylindrical cell

**Electrical Ratings** \
I_cell_rating = 25A_ctns,   I_pack_rating = 250A_ctns \
V_cell_empty = 3.0V,   V_cell_nominal = 3.6V,   V_cell_full = 4.2V \
V_pack_empty = 42.0V,   V_pack_nominal = 50.4V,   V_pack_full = 58.8V

**Module Construction** \
Each 'super-cell', in actuality, is a '5x2' (5 by 2) module which is comprised of 10 '18650' cells connected in parallel, but constructed as 2 rows of 5 cells.

**Pack Configuration** \
The primary battery pack is 14 of these 'super-cells' connected in series.


## Supported API
- UART API Testing:
	- 1.1.1 		xx
	- 1.1.2 		xx
	- 1.1.3 		xx
	- 1.1.4 		xx
	- 1.1.5 		xx
	- 1.1.6 		xx
	- 1.1.7 		xx
	- 1.1.8 		xx
	- 1.1.9 		xx
	- 1.1.10 		xx
	- 1.1.11 		xx
	- 1.1.12 		xx
	- 1.1.13 		xx
	- 1.1.14 		xx
	- 1.1.15 		xx
	- 1.1.16 		xx
	- 1.1.17 		xx
	- 1.1.18 		xx
	- 1.1.19 		xx
	- 1.1.20 		xx
	- 1.1.21 		xx
	- 1.1.22 		xx
	- 1.1.23 		xx
 
- CAN API Testing:
	- 2.1.1 		xx
	- 2.1.2 		xx
	- 2.1.3 		xx
	- 2.1.4 		xx
	- 2.1.5 		xx
	- 2.1.6 		xx
	- 2.1.7 		xx
	- 2.1.8 		xx
	- 2.1.9 		xx
	- 2.1.10 		xx
	- 2.1.11 		xx
	- 2.1.12 		xx
	- 2.1.13 		xx
	- 2.1.14 		xx
	- 2.1.15 		xx
	- 2.1.16 		xx
	- 2.1.17 		xx
	- 2.1.18 		xx
	- 2.1.19 		xx
