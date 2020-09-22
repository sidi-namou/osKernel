#ifndef _TIMER_H_
#define _TIMER_H_

#include <stdint.h>
#include "stm32l1xx.h"
#include "osKernel.h"



void setTick(uint32_t tick);
uint32_t getTick();

void timer_Init();


#endif /* RT_KERNEL_TIMER_H */