/*
 * drv_MCP.c
 *
 *  Created on: Dec 4, 2024
 *      Author: vincent
 */
#include <stdio.h>
#include <stdlib.h>
#include "spi.h"



void WriteRegister(uint8_t reg, uint8_t data)
{
	uint8_t control_byte = MCP_CONTROL_BYTE(MCP23S17_CONTROL_ADDR, VU_WRITE); // Address = 0b000

	uint8_t buffer[2] = {reg, data};
	HAL_StatusTypeDef status;

	// Transmit control byte
	status = HAL_SPI_Transmit(hMCP23S17.hspi, &control_byte, 1, HAL_MAX_DELAY);
	if (status != HAL_OK) {
		HAL_GPIO_WritePin(VU_nCS_GPIO_Port, VU_nCS_Pin, GPIO_PIN_SET); // Deassert chip select
		printf("Error: Failed to transmit control byte (HAL_SPI_Transmit returned %d)\r\n", status);
		Error_Handler(); // Handle the error
		return; // Prevent further execution
	}


}




void xpdr_Init(void);
void Set_LED_id(uint8_t led);
void Toggle_LED_id(uint8_t led);
void Set_LEDs(uint16_t leds);
void Update_LEDs(void);
void level_R(int level);
void level_L(int level);
