#include "timer.h"

static volatile uint32_t count;
// We're using TIM2 to control sendQueue
// To do that we need to enable horloge to TIM2
void timer_Init(){
	// TIM2 Config
	RCC->APB1ENR |= 1;
	TIM2->PSC = 1600 - 1;
	TIM2->ARR = 1000 - 1;
	TIM2->CNT = 0;
	TIM2->CR1 = 1;	// Enable Timer 2
	// Enable Interrupt
	TIM2->DIER |= 1; // Update Interrupt Enable
	NVIC_EnableIRQ(TIM2_IRQn);
	NVIC_SetPriority(TIM2_IRQn,1);
	
	
	
}
void TIM2_IRQHandler(void){
	TIM2->SR = 0;
	count++;
	}

uint32_t getTick(){
	uint32_t var = count;
return var;
}
void setTick(uint32_t tick){
count = tick;
	// restart the counter
//TIM2->CNT = 0;
}


