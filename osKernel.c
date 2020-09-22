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
#define INCTRL			(*((volatile uint32_t *)0xE000ED04))



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

void osThreadYield(void){
	
	SysTick->VAL = 0;
	// setPending SysTick correspond to bit 26
	INCTRL = 0x04000000;
	
}

void osSemaphoreBinaryCreate(Semaphore_t * sem){
	
	// Create a binary Semaphore and initialise by 0
	
	*sem = 0;
}

void osSemaphoreGive(Semaphore_t * sem){
	if(*sem != 1){
		
	__disable_irq();
	*sem = 1;	// semaphore set to 1
	__enable_irq();
	}
	
}

void osSemaphoreTake(Semaphore_t * sem){
	
	while(*sem != 1){
	osThreadYield();	// task sleep	
	}
	// once we have semaphore set so
	__disable_irq();
	*sem = 0;	// semaphore reset to 0
	__enable_irq();
}

void osMutexCreate(Semaphore_t *mutex){

	/* Create a mutex and initialise to 1 to be taken by anyone at first*/
	*mutex = 1;
}
void osMutexTake(Semaphore_t *mutex){

		while(*mutex != 1){
	osThreadYield();	// task sleep	
	}
		/* Once we have the mutex we need to disable all interrupts */
	
		__disable_irq();
	/* set mutex to 0*/
	*mutex = 0;
}
void osMutexGive(Semaphore_t *mutex){
	/* set mutex to 1*/
	*mutex = 1;
	/* Enable Interrupts*/
	__enable_irq();

}
void osSemaphoreGiveFromISR(Semaphore_t *sem){

	*sem = 1;

}
inline void osSemaphoreTakeFromISR(Semaphore_t *sem){

	if(*sem != 1) return;
}
