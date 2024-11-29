#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "../01.Linked_Lists/list.h"
#include "../03.Sets/set.h"

// Graph_init 

void graph_init(Graph *graph, int (*match)(const void *key1, const void *key2), void (*destroy)(void *data))
{
	// Initialize the graph 
	
	graph->vcount = 0;
	graph->ecount = 0;
	graph->match = match;
	graph->destroy = destroy;

	// Initialize the list of adjacency-list structures.
	list_init(&graph->adjlists, NULL);

	return;
}

// Graph_destroy 

void graph_destroy(Graph *graph) {
	AdjList *adjlist;

	// Remove each adjacency-list structure and destroy its adajcency list. 
	
	while (list_size(&graph->adjlists) > 0) {
		if (list_rem_next(&graph->adjlists, NULL, (void **)&adjlist) == 0)
		{
			set_destroy(&adjlist->adjacent);

			if (graph->destroy != NULL)
				graph->destroy(adjlist->vertex);

			free(adjlist);
		}
	}
	// Destroy the list of adjacency-list structures, which is now empty. 
	list_destroy(&graph->adjlists);
	// No operations are allowed now, but clear the structure as a recaution.
	memset(graph, 0 ,sizeof(Graph));

	return;
}

// Graph_ins_vertex

