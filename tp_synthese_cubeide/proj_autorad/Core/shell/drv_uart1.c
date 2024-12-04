/*
 * drv_uart1.c
 *
 *  Created on: 7 nov. 2022
 *      Author: laurentf
 */

#include "drv_uart1.h"

#include "usart.h"
#include "gpio.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

SemaphoreHandle_t semain;

//fonction pour créer le sémaphore
SemaphoreHandle_t drv_sem_init(void){

	// Crée un sémaphore binaire
	semain = xSemaphoreCreateBinary();

	//pas de gestion d'erreur = c'est un choix
	//printf("init sema");
	return semain;
}


void give_sem_uart(void){

	BaseType_t higher_priority_task_woken = pdFALSE;

	xSemaphoreGiveFromISR(semain, &higher_priority_task_woken);

	portYIELD_FROM_ISR(higher_priority_task_woken);
}

uint8_t drv_uart1_receive(char * pData, uint16_t size)
{
	HAL_UART_Receive_IT(&huart2, (uint8_t*)(pData), size);

	xSemaphoreTake(semain, portMAX_DELAY); // prendre le sémaphore

	return 0;	// Life's too short for error management
}

uint8_t drv_uart1_transmit(const char * pData, uint16_t size)
{
	HAL_UART_Transmit(&huart2, (uint8_t*)pData, size, HAL_MAX_DELAY);

	return 0;	// Srsly, don't do that kids
}


// ajouter fonction callback(c'est à moi de nommer cette callback)   non crmqui va donner le sémaphore (semaphoreGive)

///donner le semaphore

//callback a def ici

//ausssi une fonction init pour créer le seamphre



