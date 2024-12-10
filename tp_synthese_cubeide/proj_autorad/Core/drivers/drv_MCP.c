/*
 * drv_MCP.c
 *
 *  Created on: Dec 4, 2024
 *      Author: vincent
 */
#include <stdio.h>
#include <stdlib.h>
#include "drv_MCP.h"

#include "spi.h"

typedef struct {
	SPI_HandleTypeDef* hspi;
	uint8_t GPA;	// LED array in GPIOA
	uint8_t GPB;	// LED array in GPIOB
} h_MCP_t;

h_MCP_t hMCP;

void WriteRegister(uint8_t reg, uint8_t data)
{
	uint8_t control_byte = MCP_CONTROL_BYTE(REG_CONTROL_ADDR, VU_WRITE); // Address = 0b000

	uint8_t buffer[2] = {reg, data};
	HAL_StatusTypeDef status;


	//Down chip select to 0
	HAL_GPIO_WritePin(VU_nCS_GPIO_Port, VU_nCS_Pin, GPIO_PIN_RESET);



	// Transmit control byte
	status = HAL_SPI_Transmit(hMCP.hspi, &control_byte, 1, HAL_MAX_DELAY);

	if (status != HAL_OK) {

		HAL_GPIO_WritePin(VU_nCS_GPIO_Port, VU_nCS_Pin, GPIO_PIN_SET); // Deassert chip select
		printf("Error: Failed transmit control byte (status returned %d)\r\n", status);
		Error_Handler(); // Handle the error
		return; // Prevent further execution
	}

	// Transmit register address and data
	status = HAL_SPI_Transmit(hMCP.hspi, buffer, 2, HAL_MAX_DELAY);
	if (status != HAL_OK) {
		HAL_GPIO_WritePin(VU_nCS_GPIO_Port, VU_nCS_Pin, GPIO_PIN_SET); // Deassert chip select
		printf("Error: Failed transmit register data (status returned %d)\r\n", status);
		Error_Handler(); // Handle the error
		return; // Prevent further execution
	}


	// UP chip select to 1
	HAL_GPIO_WritePin(VU_nCS_GPIO_Port, VU_nCS_Pin, GPIO_PIN_SET);



}

void Update_LEDs(void){

	WriteRegister(MCP_OLATA, hMCP.GPA);
	WriteRegister(MCP_OLATB, hMCP.GPB);

}
void Set_LED_id(uint8_t led){
	if (led > 7)
	{
		hMCP.GPB = ~(1 << led%8);
		hMCP.GPA = 0xFF; // All LEDs on GPIOA OFF
	}
	else
	{
		hMCP.GPA = ~(1 << led);
		hMCP.GPB = 0xFF; // All LEDs on GPIOB OFF
	}

	Update_LEDs();
}

void Toggle_LED_id(uint8_t led){
	if (led > 7)
	{
		hMCP.GPB = (hMCP.GPB & ~(1 << led%8)) | (~hMCP.GPB & (1 << led%8));
	}
	else
	{
		hMCP.GPA = (hMCP.GPA & ~(1 << led)) | (~hMCP.GPA & (1 << led));
	}

	Update_LEDs();
}
void Set_LEDs(uint16_t leds){

	hMCP.GPB = (0xFF00 & leds) >> 8;
	hMCP.GPA = 0xFF & leds;

	Update_LEDs();
}




void xpdr_Init(void){

	hMCP.hspi = &hspi3;

	HAL_SPI_Init(hMCP.hspi);

	// nRESET to base state
	HAL_GPIO_WritePin(VU_nRESET_GPIO_Port, VU_nRESET_Pin, GPIO_PIN_SET);

	// nCS to reset state
	HAL_GPIO_WritePin(VU_nCS_GPIO_Port, VU_nCS_Pin, GPIO_PIN_SET);

	// Set all GPIOA and GPIOB pins as outputs
	WriteRegister(MCP_IODIRA, MCP_ALL_ON); // GPA as output
	WriteRegister(MCP_IODIRB, MCP_ALL_ON); // GPB as output

	hMCP.GPA = 0xFF;	// All LEDs on GPIOA OFF
	hMCP.GPB = 0xFF;	// All LEDs on GPIOB OFF

	Update_LEDs();

}



void level_R(int level){
	if (level <= 100)
	{
		hMCP.GPA = 0xFF & (0x00FF << (int)(8*level/100));

		MCP_Update_LEDs();
	}
}
void level_L(int level){
	if (level <= 100)
	{
		hMCP.GPB = 0xFF & (0x00FF << (int)(8*level/100));

		MCP_Update_LEDs();
	}
}
