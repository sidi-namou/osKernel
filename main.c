#include "osKernel.h"

#define QUANTA	100 // 100ms
uint32_t count0,count1,count2;

/* Private typedef */
enum {
	PIN_RESET = 0,
	PIN_SET
};
/* Private define  */

// Green Led connected to PA5
#define GREEN (1U << 5)
#define GREEN_BIT (1U << 10)
#define GPIOA_CLOCK (1U)
#define GPIOC_CLOCK (1U << 2)
#define pushButton (1U << 13)
#define ORANGE (1U)
#define ORANGE_BIT (1U)

/* Private macro */

/* Private variables */


/* Private function prototypes */
void DelayS(uint32_t seconds);
void GPIO_Init();
uint32_t GetTick(void);
void ledOn(void);
void ledOff(void);
void orangeOn(void);
void orangeOff(void);
int getButtonValue(void);


/* Private functions */


/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/

void Task0(void){
	
		while(1){
			count0++;
			ledOff();
			osThreadYield();
					}	
		
}
void Task1(void){

			while(1){
				
			count1++;
				
			ledOn();
			osThreadYield();				
				}
			
			
}
void Task2(void){
	
	while(1){
		
		count2++;
		orangeOn();
		
	
	}
	
}
int main(){

	GPIO_Init();
	osKernelInit();
	osKernelAddThreads(&Task0,&Task1,&Task2);
	osKernelLaunch(QUANTA);
	
	while(1){
	
	}

	
}
void GPIO_Init(){

	// HSI ON
		RCC->CR |= 0x01;
		// wait for HSI to be ready
		while((RCC->CR & 0x02) == 0x02);
		// select HSI as system clock
				RCC->CFGR |= 0x01;
	// MSI OFF
		RCC->CR &= ~(0x00000100);


	// GPIOA and GPIOC clocks enable
		RCC->AHBENR |= GPIOA_CLOCK|GPIOC_CLOCK;

		
		// GPIOA5 and GPIOA0 as output mode
		GPIOA->MODER |= GREEN_BIT|ORANGE_BIT;
		

}
void ledOn(void){
	GPIOA->ODR |= GREEN;
}
void ledOff(void){
	GPIOA->ODR &=~GREEN;
}
void orangeOn(void){
	GPIOA->ODR |= ORANGE;
}
void orangeOff(void){
	GPIOA->ODR &=~ORANGE;
}