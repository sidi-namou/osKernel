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

void osKernelStackInit(int i);
void osKernelLaunch(uint32_t quanta);
void osKernelInit(void);

uint8_t osKernelAddThreads(void(*task0)(void),
		void(*task1)(void),
		void(*task2)(void));

void osThreadYield(void);
		
		
#endif /* RT_KERNEL_OSKERNEL_H_ */
