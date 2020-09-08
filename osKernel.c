/*
 * kernel.c
 *
 *  Created on: 8 sept. 2020
 *      Author: 33766
 */


// Scheduler
// TCB
// Stack

#include "osKernel.h"

#define NUM_OF_THREADS	3
#define STACK_SIZE		100
#define BUS_FREQ		16000000

#define SYSPRI3			(*((volatile uint32_t *)0xE000E400))

uint32_t MILLIS_PRESCALER;

void osSchedulerLaunch(void);
struct tcb{
	int32_t *stackPt;
	struct tcb *nextPt;
};

typedef struct tcb tcbType;
tcbType tcbs[NUM_OF_THREADS];
tcbType *currentPt;

int32_t TCB_STACK[NUM_OF_THREADS][STACK_SIZE];

void osKernelStackInit(int i){
	tcbs[i].stackPt = &TCB_STACK[i][STACK_SIZE-16];
	// Initialise xPSR to run thumb mode
	TCB_STACK[i][STACK_SIZE - 1] = 0x01000000;
}


uint8_t osKernelAddThreads(void(*task0)(void),
		void(*task1)(void),
		void(*task2)(void)){

	__disable_irq();


	tcbs[0].nextPt = &tcbs[1];
	tcbs[1].nextPt = &tcbs[2];
	tcbs[2].nextPt = &tcbs[0];

	osKernelStackInit(0);
	// PC point to  Task
	TCB_STACK[0][STACK_SIZE - 2] = (int32_t)(task0);

	osKernelStackInit(1);
	// PC point to  Task
	TCB_STACK[1][STACK_SIZE - 2] = (int32_t)(task1);

	osKernelStackInit(2);
	// PC point to  Task
	TCB_STACK[2][STACK_SIZE - 2] = (int32_t)(task2);
	
	currentPt = &tcbs[0];
	__enable_irq();
	return 1;
}


void osKernelInit(void){
	__disable_irq();
	MILLIS_PRESCALER = BUS_FREQ/1000;

}


void osKernelLaunch(uint32_t quanta){
	// Initialisation of SysTick Timer
	SysTick->CTRL = 0;
	SysTick->VAL = 0;

	SYSPRI3	= (SYSPRI3 & 0x00FFFFFF) | 0xE0000000; // Priority 7

	SysTick->LOAD = (quanta * MILLIS_PRESCALER) -1;
	SysTick->CTRL = 0x00000007;
	
	osSchedulerLaunch();
	
}

