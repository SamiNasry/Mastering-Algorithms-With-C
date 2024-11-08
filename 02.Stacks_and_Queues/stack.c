#include <stdlib.h>

#include "../01.Linked_Lists/list.h"
#include "stack.h"

// Stack_Push

int stack_push(Stack *stack, const void *data)
{
	// Push the data onto the stack.
	
	return list_ins_next(stack,NULL, data);
}

// Stack_POP 

int stack_pop(Stack *stack, void **data)
{
	// Pop the data off the stack. 
	return list_rem_next(stack, NULL, data);
}
