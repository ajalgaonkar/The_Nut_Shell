/*
    Simple singly linked list used for maintaining the
    Environment Variables dynamically. 
*/
 
#include <stdio.h>
#include <stdlib.h>

typedef char* linkedlist_data;

typedef struct linkedlist Node; 																//short name for the linkedlist type
struct linkedlist 																				//linkedlist structure format
{
    linkedlist_data key;
    linkedlist_data value;
    Node *next;
};
 
extern int linkedlist_len(Node *node_head); 													// Linkedlist length
extern void linkedlist_insert(Node **node_head, linkedlist_data key, linkedlist_data value); 	// Pushes a value d onto the linkedlist
extern linkedlist_data linkedlist_delete_node(Node **node_head, linkedlist_data key); 			// Removes the Node from the linkedlist & returns its value
extern void linkedlist_print(Node **node_head); 												// Prints all the linkedlist data
extern void linkedlist_clear(Node **node_head); 												// Clears the linkedlist of all elements
extern void linkedlist_snoc(Node **node_head, linkedlist_data key, linkedlist_data value); 		// Appends a node
extern int linkedlist_find(Node **node_head, linkedlist_data k); 								// Checks for an element
extern linkedlist_data alias_find(Node **node_head, linkedlist_data k);							// Returns alias value if Node with k key is present in the Linked_List of Alias
