/*
 * SGTL5000.c
 *
 *  Created on: Dec 10, 2024
 *      Author: vincent
 */


#include "SGTL5000.h"
#include "i2c.h"
#include <stdio.h>
#include <stdlib.h>

#define LOGS 0
#define DEBUG 1

typedef struct {
	I2C_HandleTypeDef * hi2c;
	uint16_t chip_id;
} h_SGTL5000_t;

h_SGTL5000_t hSGTL5000;



void SGTL5000_ErrorHandler(const char* message)
{
	printf("SGTL5000 Error: %s\r\n", message);
	Error_Handler();
	NVIC_SystemReset();
}


void SGTL5000_i2c_ReadRegister(uint16_t address, uint8_t* pData, uint16_t length)
{
	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(hSGTL5000.hi2c, SGTL5000_CODEC,
			address, SGTL5000_MEM_SIZE, pData, length, HAL_MAX_DELAY);

	if (status != HAL_OK) {
		printf("Error: Failed to read from address 0x%04X\r\n", address);
		SGTL5000_ErrorHandler("ReadRegister failed");
	}
}


void SGTL5000_i2c_WriteRegister(uint16_t address, uint16_t value)
{
	uint8_t data[2] = { (uint8_t)(value >> 8), (uint8_t)(value & 0xFF) };
	HAL_StatusTypeDef status = HAL_I2C_Mem_Write(hSGTL5000.hi2c, SGTL5000_CODEC,
			address, SGTL5000_MEM_SIZE, data, 2, HAL_MAX_DELAY);

	// Handle all possible I2C errors
	switch (status) {
	case HAL_OK:
		// Write successful
#if LOGS
		printf("Successfully wrote 0x%04X to address 0x%04X\r\n", value, address);
#endif
		break;

	case HAL_ERROR:
		// General HAL error
		printf("Error: HAL_ERROR while writing 0x%04X to address 0x%04X\r\n", value, address);
		SGTL5000_ErrorHandler("General HAL_ERROR during WriteRegister");
		break;

	case HAL_BUSY:
		// HAL busy error
		printf("Error: HAL_BUSY, I2C bus is busy while writing 0x%04X to 0x%04X\r\n", value, address);
		SGTL5000_ErrorHandler("I2C bus busy during WriteRegister");
		break;

	case HAL_TIMEOUT:
		// Timeout error
		printf("Error: HAL_TIMEOUT while writing 0x%04X to address 0x%04X\r\n", value, address);
		SGTL5000_ErrorHandler("Timeout during WriteRegister");
		break;

	default:
		// Unexpected error code
		printf("Error: Unknown error (status code: %d) while writing 0x%04X to address 0x%04X\r\n", status, value, address);
		SGTL5000_ErrorHandler("Unknown error during WriteRegister");
		break;
	}
}

void SGTL5000_Init(void)
{
	hSGTL5000.hi2c = &hi2c2;

	uint8_t chip_id_data[2];
	SGTL5000_i2c_ReadRegister(SGTL5000_CHIP_ID, chip_id_data, SGTL5000_MEM_SIZE);
	hSGTL5000.chip_id = (chip_id_data[0] << 8) | chip_id_data[1];
	printf("Valeur CHIP_ID (16 bits) : 0x%04X\n",hSGTL5000.chip_id);

	if (hSGTL5000.chip_id != 0xA011) { // Example CHIP_ID, replace with actual expected ID
		SGTL5000_ErrorHandler("Invalid CHIP_ID detected");
	}

	uint16_t mask;



	//mask = (1 << 5) | (1 << 6);//0x0060
	//mask = (1 << 6) | (1 << 5) | (4); //0x0064
	mask = (1 << 6) | (1 << 5) | (1 << 3) | (1 << 2); // 0x006C
	SGTL5000_i2c_WriteRegister(SGTL5000_CHIP_LINREG_CTRL, mask);
#if (DEBUG)
	printf("SGTL5000_CHIP_LINREG_CTRL set as: 0x%04X\r\n", mask);
#endif


	// Définir le masque pour 0x009C
	mask = (1 << 7) | (1 << 4) | (1 << 3) | (1 << 2); // 0x009C
	SGTL5000_i2c_WriteRegister(SGTL5000_CHIP_REF_CTRL, mask);
#if (DEBUG)
	printf("SGTL5000_CHIP_REF_CTRL set as: 0x%04X\r\n", mask);
#endif


	mask = 0x031E;
	SGTL5000_i2c_WriteRegister(SGTL5000_CHIP_LINE_OUT_CTRL, mask);
#if (DEBUG)
	printf("SGTL5000_CHIP_LINE_OUT_CTRL set as: 0x%04X\r\n", mask);
#endif

	mask = 0x1106;	// MODE_CM = 2, MODE_LR = 1, LVLADJC = 200mA, LVLADJL = 75mA, LVLADJR = 50mA
	SGTL5000_i2c_WriteRegister(SGTL5000_CHIP_SHORT_CTRL, mask);
#if (DEBUG)
	printf("SGTL5000_CHIP_SHORT_CTRL set as: 0x%04X\r\n", mask);
#endif

	// Enable Zero-cross detect if needed for HP_OUT (bit 5) and ADC (bit 1)
	// Write CHIP_ANA_CTRL 0x0133
	mask = 0x0004;	// Unmute all + SELECT_ADC = LINEIN
	//	mask = 0x0000;	// Unmute all + SELECT_ADC = MIC
	SGTL5000_i2c_WriteRegister(SGTL5000_CHIP_ANA_CTRL, mask);
#if (DEBUG)
	printf("SGTL5000_CHIP_ANA_CTRL set as: 0x%04X\r\n", mask);
#endif


	// Power up LINEOUT, HP, ADC, DAC
	// Write CHIP_ANA_POWER 0x6AFF
	mask = 0x6AFF;	// LINEOUT_POWERUP, ADC_POWERUP, CAPLESS_HEADPHONE_POWERUP, DAC_POWERUP, HEADPHONE_POWERUP, REFTOP_POWERUP, ADC_MONO = stereo
	// VAG_POWERUP, VCOAMP_POWERUP = 0, LINREG_D_POWERUP, PLL_POWERUP = 0, VDDC_CHRGPMP_POWERUP, STARTUP_POWERUP = 0, LINREG_SIMPLE_POWERUP,
	// DAC_MONO = stereo
	SGTL5000_i2c_WriteRegister(SGTL5000_CHIP_ANA_POWER, mask);
#if (DEBUG)
	printf("SGTL5000_CHIP_ANA_POWER set as: 0x%04X\r\n", mask);
#endif

	mask = 0x0073;	// I2S_IN_POWERUP, I2S_OUT_POWERUP, DAP_POWERUP, DAC_POWERUP, ADC_POWERUP
	SGTL5000_i2c_WriteRegister(SGTL5000_CHIP_DIG_POWER, mask);
#if (DEBUG)
	printf("SGTL5000_CHIP_DIG_POWER set as: 0x%04X\r\n", mask);
#endif

	//----------------Set LINEOUT Volume Level-------------------

	mask = 0x1111;	// TODO recalculer
	SGTL5000_i2c_WriteRegister(SGTL5000_CHIP_LINE_OUT_VOL, mask);
#if (DEBUG)
	printf("SGTL5000_CHIP_LINE_OUT_VOL set as: 0x%04X\r\n", mask);
#endif

	/* System MCLK and Sample Clock */


	mask = 0x0004;	// SYS_FS = 48kHz
	SGTL5000_i2c_WriteRegister(SGTL5000_CHIP_CLK_CTRL, mask);
#if (DEBUG)
	printf("SGTL5000_CHIP_CLK_CTRL set as: 0x%04X\r\n", mask);
#endif
	// Configure the I2S clocks in master mode

	mask = 0x0130;	// DLEN = 16 bits
	SGTL5000_i2c_WriteRegister(SGTL5000_CHIP_I2S_CTRL, mask);
#if (DEBUG)
	printf("SGTL5000_CHIP_I2S_CTRL set as: 0x%04X\r\n", mask);
#endif

	/* PLL Configuration */

	mask = 0x0000;	// Unmute
	SGTL5000_i2c_WriteRegister(SGTL5000_CHIP_ADCDAC_CTRL, mask);
#if (DEBUG)
	printf("SGTL5000_CHIP_ADCDAC_CTRL set as: 0x%04X\r\n", mask);
#endif

	mask = 0x3C3C;
	//	mask = 0x4747;
	SGTL5000_i2c_WriteRegister(SGTL5000_CHIP_DAC_VOL, mask);
#if (DEBUG)
	printf("SGTL5000_CHIP_DAC_VOL set as: 0x%04X\r\n", mask);
#endif

	mask = 0x0251;	// BIAS_RESISTOR = 2, BIAS_VOLT = 5, GAIN = 1
	SGTL5000_i2c_WriteRegister(SGTL5000_CHIP_MIC_CTRL, mask);
#if (DEBUG)
	printf("SGTL5000_CHIP_MIC_CTRL set as: 0x%04X\r\n", mask);
#endif

	//	for (int i = 0 ; register_map[i] != SGTL5000_DAP_COEF_WR_A2_LSB ; i++)
	//	{
	//		uint16_t reg = 0;
	//		sgtl5000_i2c_read_register(h_sgtl5000, register_map[i], &reg);
	//		printf("%02d: [0x%04x] = 0x%04x\r\n", i, register_map[i], reg);
	//	}

#if (LOGS)
	printf("SGTL5000 initialized successfully, CHIP_ID: 0x%04X\r\n", hSGTL5000.chip_id);
#endif
}
