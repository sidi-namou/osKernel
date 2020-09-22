#ifndef _QUEUE_H_
#define _QUEUE_H_
#include "timer.h"
#include "osKernel.h"
enum memory_Allocation_State{
	Memory_Allocation_Succed,
	Memory_Allocation_Failed
};
typedef enum memory_Allocation_State queue_state;

enum boolean {FALSE = 0,
TRUE=1};
typedef enum boolean bool;


typedef struct node{
	void * data;	/*a pointer to data */
	struct node * next;	/* pointer to the next element*/
}node;

typedef struct {
	node * head;	/*pointer to the head of the queue*/
	node * tail;	/*pointer to the tail of the queue*/
	uint32_t size;
	uint32_t lengthMax;
}Queue_t;




queue_state osQueueCreate(Queue_t ** q,uint32_t length);

/***********
queue a pointer to queue message
data a pointer to  data that will be stored in queue message
TimeToWait it's time to wait in s
***************/

void osQueueSend(Queue_t *q, void ** data, uint32_t timeToWait);

/***********
queue a pointer to queue message
data a pointer to where data will be stored
TimeToWait it's time to wait in s
***************/

void osQueueReceive(Queue_t *q, void ** data, uint32_t timeToWait);


#endif