/*
 * kernel.h
 *
 *  Created on: 8 sept. 2020
 *      Author: 33766
 */

#ifndef _OS_KERNEL_H_
#define _OS_KERNEL_H_
#include <stdint.h>
#include "stm32l1xx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef uint32_t Semaphore_t;




void osKernelStackInit(int i);
void osKernelLaunch(uint32_t quanta);
void osKernelInit(void);

uint8_t osKernelAddThreads(void(*task0)(void),
		void(*task1)(void),
		void(*task2)(void));

void osThreadYield(void);
		
void osSemaphoreBinaryCreate(Semaphore_t * sem);
void osSemaphoreTake(Semaphore_t * sem);
void osSemaphoreGive(Semaphore_t * sem);

void osMutexCreate(Semaphore_t *mutex);
void osMutexTake(Semaphore_t *mutex);
void osMutexGive(Semaphore_t *mutex);		

void osSemaphoreGiveFromISR(Semaphore_t *sem);
inline void osSemaphoreTakeFromISR(Semaphore_t *sem);		
#endif /* RT_KERNEL_OSKERNEL_H_ */
