/*
 * drv_uart1.h
 *
 *  Created on: 7 nov. 2022
 *      Author: laurentf
 */

#ifndef DRV_UART1_H_
#define DRV_UART1_H_

#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"


uint8_t drv_uart1_receive(char * pData, uint16_t size);
uint8_t drv_uart1_transmit(const char * pData, uint16_t size);

SemaphoreHandle_t drv_sem_init(void);
void give_sem_uart(void);


#endif /* DRV_UART1_H_ */
