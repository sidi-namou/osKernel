#include "osKernel.h"
#include "queue.h"


#define QUANTA	100 // 100ms

extern Semaphore_t semISR;
static volatile uint32_t count0,count1,count2, state=0;
//static char  data[30];
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

static Semaphore_t sem12; 	// semaphore between task1 and task2
static Semaphore_t	sem21;

/* Queue Message between task 1 and task 2	*/
static Queue_t * queue12;
/* Private macro */

/* Private variables */


/* Private function prototypes */
void DelayS(uint32_t seconds);
void GPIO_Init(void);
uint32_t GetTick(void);
void ledOn(void);
void ledOff(void);
void orangeOn(void);
void orangeOff(void);
int getButtonValue(void);

void Task0(void);
void Task1(void);
void Task2(void);
/* Private functions */


/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/

void Task0(void){
	void * dat;
	
		while(1){
			
			count0++;
			dat = (void *) (&count0);
			
		//	osSemaphoreGive(&sem12);
			osQueueSend(queue12,&dat,2);
			ledOff();
		//	osSemaphoreTake(&sem21);
			//osSemaphoreTake(&sem21);
			
					}	
		
}
void Task1(void)
	{
	void * dat = 0;
	static uint32_t *val;
	 
			while(1){
			// wait for task1 to give semaphore	
		//	osSemaphoreTake(&sem12);
				
				ledOn();
				osQueueReceive(queue12,&dat,2);
				val = (uint32_t *)dat;
				count1 = *val;
				
			//osSemaphoreGive(&sem21);
				}
			
			
}
void Task2(void){
	
	while(1){
		
		osSemaphoreTake(&semISR);
		count2++;
		orangeOn();
		
	
	}
	
}
int main(){

	GPIO_Init();
	/*Initialisation of timer*/
	timer_Init();
	/*Create a Queue of Messages	*/
	//queue12 = (Queue_t *)malloc(sizeof(Queue_t));
	if(osQueueCreate(&queue12,3)==Memory_Allocation_Succed)state = 1; /*for debugging*/
	osSemaphoreBinaryCreate(&sem12);	// Create a semaphore
	osSemaphoreBinaryCreate(&sem21);	// Create a semaphore
	
	osSemaphoreBinaryCreate(&semISR);	// Create a semaphore
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
		while((RCC->CR & 0x02) == 0x02){
		}
		// select HSI as system clock
				RCC->CFGR |= 0x01;
	// MSI OFF
		RCC->CR &= (uint32_t)~(0x00000100);


	// GPIOA and GPIOC clocks enable
		RCC->AHBENR |= GPIOA_CLOCK|GPIOC_CLOCK;

		
		// GPIOA5 and GPIOA0 as output mode
		GPIOA->MODER |= GREEN_BIT|ORANGE_BIT;
		

}
inline void ledOn(void){
	GPIOA->ODR |= GREEN;
}
inline void ledOff(void){
	GPIOA->ODR &=~GREEN;
}
void orangeOn(void){
	GPIOA->ODR |= ORANGE;
}
void orangeOff(void){
	GPIOA->ODR &=~ORANGE;
}
