#include "queue.h"

static Semaphore_t semWrite, semRead;

bool IsFull(Queue_t * q,uint32_t size);
bool IsEmpty(Queue_t * q);

void initQueue(Queue_t *q);

bool enQueue(Queue_t *q, void * data,uint32_t size_Max);
void * deQueue(Queue_t *q);

void initQueue(Queue_t *q){
    q->size = 0;
	q->head = NULL;
	q->tail = NULL;
}
bool IsFull(Queue_t * q,uint32_t size){
return q->size == size;
}
bool IsEmpty(Queue_t * q){
return q->size == 0;

}
bool enQueue(Queue_t *q, void * data,uint32_t size_Max){

    if(IsFull(q,size_Max))return FALSE;
   else {
        q->size++;
      node * newNode = (node *)malloc(sizeof(node));

	if(newNode == NULL)return FALSE;
		 
	newNode->data = data;
	newNode->next = q->head;

	if(q->tail != NULL){
	q->tail->next = newNode;
	}
	q->tail = newNode;

	if(q->head == NULL){
	q->head = newNode;
	}



	return TRUE;
    }

}

void *  deQueue(Queue_t *q){

	void * result;
	node * temp = q->head;

	result = temp->data;
	q->head = q->head->next;
	if(q->head == NULL){
        q->tail = NULL;
	}
	q->size--;
	free(temp);
return result;
}

queue_state osQueueCreate(Queue_t ** q,uint32_t length){
	/* Create Queue*/
	*q = (Queue_t* )malloc(sizeof(Queue_t));
	if(*q==NULL)return Memory_Allocation_Failed;
	/*Queue Initialisation*/
	initQueue(*q);
	
	(*q)->lengthMax = length;
	osSemaphoreBinaryCreate(&semWrite);	// Create a semaphore
	osSemaphoreBinaryCreate(&semRead);	// Create a semaphore
	osSemaphoreGive(&semWrite);
	osSemaphoreGive(&semRead);

return Memory_Allocation_Succed;
}

void osQueueSend(Queue_t *q, void ** data, uint32_t timeToWait){
uint32_t timeTicks = 0;
	
	
	// Necessity of timer to count time
	setTick(timeTicks);
	while(IsFull(q,q->lengthMax) && timeTicks <= timeToWait){
		osThreadYield();	// task sleep
		timeTicks = getTick();
	}

	if(!IsFull(q,q->lengthMax)){
	osSemaphoreTake(&semWrite);
	
	enQueue(q,*data,q->lengthMax);
	
	osSemaphoreGive(&semRead);
	}
	
}

void osQueueReceive(Queue_t *q, void ** data, uint32_t timeToWait){
	uint32_t timeTicks=0;
	// Necessity of timer to count time
	setTick(timeTicks);
	// if queueMessage is empty we have to wait  
	while(IsEmpty(q) && timeTicks <= timeToWait){
		osThreadYield();	// task sleep
		timeTicks = getTick();
	}

	if(!IsEmpty(q)){
	osSemaphoreTake(&semRead);
	*data = deQueue(q);
	osSemaphoreGive(&semWrite);
	}
	
	
}

