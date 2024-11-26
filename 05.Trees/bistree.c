#include <stdlib.h>
#include <string.h>

#include "bistree.h"
#include "bitree.h"
static void destroy_right(BisTree *tree, BiTreeNode *node);

//rotate_left
static void rotate_left(BiTreeNode **node)
{
	BiTreeNode *left,*grandchild;

	left = bitree_left(*node);

	if (((AvlNode *)bitree_data(left))->factor == AVL_LFT_HEAVY) {
		// Perform an LL rotation.

		bitree_left(*node) = bitree_right(left);
		bitree_right(left) = *node;
		((AvlNode *)bitree_data(*node))->factor = AVL_BALANCED;
		((AvlNode *)bitree_data(left))->factor = AVL_BALANCED;
		*node = left;
	}

	else {
		// Perform an LR rotation.

		grandchild = bitree_right(left);
		bitree_right(left) = bitree_left(grandchild);
		bitree_left(grandchild) = left;
		bitree_left(*node) = bitree_right(grandchild);
		bitree_right(grandchild) = *node;

		switch (((AvlNode *)bitree_data(grandchild))->factor) {
			case AVL_LFT_HEAVY:
				((AvlNode *)bitree_data(*node))->factor = AVL_RGT_HEAVY;
				((AvlNode *)bitree_data(left))->factor = AVL_BALANCED;
				break;

			case AVL_BALANCED:
				((AvlNode *)bitree_data(*node))->factor = AVL_BALANCED;
				((AvlNode *)bitree_data(left))->factor = AVL_BALANCED;
				break;
			
			case AVL_RGT_HEAVY:
				((AvlNode *)bitree_data(*node))->factor = AVL_BALANCED;
				((AvlNode *)bitree_data(left))->factor = AVL_LFT_HEAVY;
				break;
		}
		((AvlNode *)bitree_data(grandchild))->factor = AVL_BALANCED;
		*node = grandchild;
	}
	return ;
}

	// rotate_right
	static void rotate_right(BiTreeNode **node) {
		BiTreeNode *right,*grandchild;
		right = bitree_right(*node);
		if ((AvlNode *)bitree_data(right))->factor == AVL_RGT_HEAVY){
			// Perform an RR rotation.
			bitree_right(*node) = bitree_left(right);
			bitree_left(right) = *node;
			((AvlNode *)bitree_data(*node))->factor = AVL_BALANCED;
			((AvlNode *)bitree_data(right))->factor = AVL_BALANCED;
			*node = right;
		}
		else {
			// Perform an RL rotation.
			grandchild = bitree_left(right);
			bitree_left(right) = bitree_right(grandchild);
			bitree_right(grandchild) = right;
			bitree_right(*node) = bitree_left(grandchild);
			bitree_left(grandchild) = *node;

			switch (((AvlNode *)bitree_data(grandchild))->factor) {
				
				case AVL_LFT_HEAVY:

					((AvlNode *)bitree_data(*node))->factor = AVL_BALANCED;
					((AvlNode *)bitree_data(right))->factor = AVL_RGT_HEAVY;
					break;
				
				case AVL_BALANCED:
					((AVlNode *)bitree_data(*node))->factor = AVL_BALANCED;
					((AvlNode *)bitree_data(right))->factor  = AVL_BALANCED;
					break;

				case AVL_RGT_HEAVY:
					((AvlNode *)bitree_data(*node))->factor = AVL_LFT_HEAVY;
					((AvlNode *)bitree_data(right))->factor = AVL_BALANCED;
					break;
			}
			((AvlNode *)bitree_data(grandchild))->factor = AVL_BALANCED;
			*node = grandchild;
		}	
		return;
	}


// Destroy_Left 

static void destroy_left(BisTree *tree, BiTreeNode *node)
{
	BiTreeNode **position;
	// Do not allow destruction of an empty tree.
	if (bitree_size(tree) == 0)
		return 0;
	// Determine where to destroy nodes.
	
	if (node == NULL)
		position = &tree->root;
	else
		position = &node->left;
	// Destroy the nodes.
	if (*position != NULL)
	{
		destroy_left(tree, *position);
		destroy_right(tree, *position);

		if(tree->destroy != NULL)
		{
			// Call a user-defined function to free dyanmically allocated data.
			tree->destroy(((AvlNode *)(*position)->data)->data);
		}

		// Free the AVL data in the node, then free then node itself.

		free((*position)->data);
		free(*position);
		*position = NULL;

		// Adjust the size of the tree to account for the destroyed node. 

		tree->size--;
	}

	return;
}

//destroy_right 

static void destroy_right(BisTree *tree, BiTreeNode *node)
{
	BiTreeNode **position;

	// Do not allow destruction of an empty tree. 
	
	if (bitree_size(tree) == 0)
		return;

	// Determine where to destroy nodes. 
	if (node == NULL)
		position = &tree->root;
	else
		position = &node->right;

	//Destroy the nodes. 
	
	if(*position != NULL) {
		destroy_left(tree, *position);
		destroy_right(tree, *position);

		if(tree->destroy != NULL) {
			// Call a user-defined function to free dynamically allocated data. 

			tree->destroy(((AvlNode *)(*position)->data)->data);
		}
		//Free the AVL data in the ode, then free the node itself. 
		
		free((*position)->data);
		free(*position);
		*position = NULL;

		// Adjust the size of the tree to account for the destroyed node.

		tree->size--;
	}
	return;

}

// Insert 
static int insert(BisTree *tree, BiTreeNode **node, const void *data, int *balanced) {
	AvlNode *avl_data;

	int cmpval,
		retval;

	// Insert the data into the tree. 
	if (bitree_is_eob(*node)) {
		// Handle insertion into an empty tree.

		if ((avl_data = (AvlNode *)malloc(sizeof(AvlNode))) == NULL)
			return -1;

		avl_data->factor = AVL_BALANCED;
		avl_data->hidden = 0;
		avl_data->data = (void *)data;

		return bitree_ins_left(tree, *node, avl_data);
	}

	else {
		// Handle insertion into a tree that is not empty 
		cmpval = tree->compare(data, ((AvlNode *)bitree_data(*node))->data);

		if (cmpval < 0) {
			// Move to the left.

			if (bitree_is_eob(bitree_left(*node))) {
				if ((avl_data = (AvlNode *)malloc(sizeof(AvlNode))) == NULL)
					return -1;
				avl_data->factor = AVL_BALANCED;
				avl_data->hidden = 0;
				avl_data->data = (void *)data;

				if (bitree_ins_left(tree, *node, avl_data) != 0)
					return -1;
				*balanced = 0;
			}
			else {
				if ((retval = insert(tree,&bitree_left(*node), data, balanced)) != 0)
				{
					return retval;
				}

			}

			// Ensure that the tree remains balanced.

			if (!(*balanced)) {
				switch ((AvlNode *)bitree_data(*node))->factor) {
					case AVL_LFT_HEAVY:
						// Stop here
				}
			}
			

		}
	}
	
}



























