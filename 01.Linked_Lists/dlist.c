#include <stdlib.h>
#include <string.h>

#include "dlist.h"

// dlist_init 

void dlist_init(DList *list, void (*destroy)(void *data)) {
	// Initialize the list 
	
	list->size = 0;
	list->destroy = destroy;
	list->head = NULL;
	list->tail = NULL;

	return;
}

//dlist_destroy 

void dlist_destroy(DList *list)
{
	void *data;

	//destroy each element
	
	while(dlist_size(list) > 0)
	{
		if(dlist_remove(list, dlist_tail(list), (void **)&data) == 0 && list->destroy != NULL) {
			// Call a user-defined function to free dynamically allocated data.
			list->destroy(data);
		}
	}
	
	// No operation are allowed now, but clear the structure as a precaution.
	
	memset(list, 0, sizeof(DList));

	return;
}

//dlist_ins_next


