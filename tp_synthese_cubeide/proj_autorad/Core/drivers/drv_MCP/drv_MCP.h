/*
 * drv_MCP.h
 *
 *  Created on: Dec 4, 2024
 *      Author: vincent
 */

#ifndef DRIVERS_DRV_MCP_DRV_MCP_H_
#define DRIVERS_DRV_MCP_DRV_MCP_H_

#include <stdint.h>


#define REG_CONTROL_ADDR 0b000 //if a0 a1 a2 to GND
//logique inverse

//adress of each registers
#define REG_IODIRA  0x00 //address of I/O configuration register (configuration bits (8bits) GPA) : port direction as input or output
#define REG_IODIRB  0x01 // address of I/O configuration register (configuration bits (8bits) GPB): port direction input or output
#define REG_OLATA   0x14 // address of register read latch port A
#define REG_OLATB 	0x15 // address of register read latch port B

//all pins as input or output
#define REG_ALL_ON 0x00
#define REG_ALL_OFF 0xFF

typedef enum
{
	VU_WRITE = 0U,
	VU_READ
} MCP23S17_Mode;
// Builds the VU-Metre control byte
#define MCP_CONTROL_BYTE(adress, RW) ((0b0100 << 4) | (adress & 0b111 << 1) | RW)




void WriteRegister(uint8_t reg, uint8_t data);
void xpdr_Init(void);
void Set_LED_id(uint8_t led);
void Toggle_LED_id(uint8_t led);
void Set_LEDs(uint16_t leds);
void Update_LEDs(void);
void level_R(int level);
void level_L(int level);

#endif /* DRIVERS_DRV_MCP_DRV_MCP_H_ */
