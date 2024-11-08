#include <stdlib.h>

#include "../01.Linked_Lists/list.h"
#include "queue.h"

//Queue_enqueue 

int queue_enqueue(Queue *queue,const void *data)
{
	// Enqueue the data
	
	return list_ins_next(queue, list_tail(queue), data);
}

//Queue_dequeue 
int queue_dequeue(Queue *queue, void **data)
{
	// Dequeue the data.
	return list_rem_next(queue, NULL, data);
}
