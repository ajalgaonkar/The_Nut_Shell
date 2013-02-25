/*
	---------------- Juned Project3  -----------------
    Simple singly linked list used for maintaining the
    Environment Variables dynamically. 
*/
 
#include "linkedlist.h"

// Returns the lenght of the Linked_List
int linkedlist_len(Node *node_head)
{
    Node *curr = node_head;
    int len = 0;
     
    while(curr)
    {
        ++len;
        curr = curr -> next;
    }
    return len;
}
 

// Inserts Key,Value into the Linked_List 
void linkedlist_insert(Node **node_head, linkedlist_data key, linkedlist_data value)
{
	if(linkedlist_find(node_head, key)) {
		Node *node_curr = *node_head;
	
		do {
			if(strcmp(node_curr -> key, key) == 0) {
				node_curr -> value = value;
				return;
			}
			node_curr = node_curr -> next;
		}while(node_curr);
	}
	else {
	    Node *node_new = malloc(sizeof(Node));
	     
	    node_new -> key = key;
	    node_new -> value = value;
	    node_new -> next = *node_head;
	    *node_head = node_new;
	}
}
 
// Removes the Node of the specified key
linkedlist_data linkedlist_delete_node(Node **node_head, linkedlist_data key)
{
	linkedlist_data result = "No Node Found !";
	Node *node_curr = *node_head;
	Node *node_prev = NULL;
	
	while(node_curr) {
		if(strcmp(node_curr -> key, key) == 0) {
			result = node_curr -> value;
			if(node_prev == NULL) { 
				*node_head = node_curr -> next;
			}
			else {
				node_prev -> next = node_curr -> next;
			}
			free(node_curr);
			return result;
		}
		node_prev = node_curr;
		node_curr = node_curr -> next;
	}
}
 
// Prints the Linked_List
void linkedlist_print(Node **node_head)
{
    Node *node_curr = *node_head;
     
    if(!node_curr)
        puts("The Linked_List is Empty");
    else
    {
        while(node_curr)
        {
            printf("Variable=%s, Word=%s\n", node_curr -> key, node_curr -> value); //set for integers, modifiable
            node_curr = node_curr -> next;
        }
        putchar('\n');
    }
}
 
// Empties the Linked_List
void linkedlist_clear(Node **node_head)
{ 
    while(*node_head) {
   		Node *node_prev = *node_head;
        *node_head = node_prev -> next;
        free(node_prev);
    }
}
 
// Snocs the Node at the end.
void linkedlist_snoc(Node **node_head, linkedlist_data key, linkedlist_data value)
{
    Node *node_curr = *node_head;
     
    if(!node_curr)
        linkedlist_insert(node_head, key, value);
    else
    {
        //find the last node
        while(node_curr -> next)
            node_curr = node_curr -> next;
        //build the node after it
        linkedlist_insert(&(node_curr -> next), key, value);
    }
}
 
// Returns 1 if Node with k key is present in the Linked_List
int linkedlist_find(Node **node_head, linkedlist_data k)
{
    Node *node_curr = *node_head;
     
    while(node_curr)
    {
        if(strcmp(node_curr -> key, k) == 0) //set for numbers, modifiable
            return 1;
        else
            node_curr = node_curr -> next;
    }
    return 0;
}