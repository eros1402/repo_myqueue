//============================================================================
// Name        : main.cpp
// Author      : Pham Hoang Chi
// Version     : 1.0
// Copyright   : Copyright from Pham Hoang Chi
// Description : Test myqueue.cpp
//============================================================================

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "myqueue.h"

using namespace std;

void int_element_copy(q_element_t *dest_element, q_element_t src_element);
void int_element_print(q_element_t element);
void int_element_free(q_element_t *element);

void test_int_queue ()
{
	  q_element_t *p=NULL;
	  queue_t* queue = NULL;
	  int *a;
	  int *b;
	  int *c;
	  a = (int *)malloc(sizeof(int));
	  b = (int *)malloc(sizeof(int));
	  c = (int *)malloc(sizeof(int));

	  *a= 1; *b=2; *c=3;

	  int m =4, n=5, t=6;

	  queue = queue_create(15, &int_element_copy, &int_element_free, &int_element_print);

	  queue_enqueue(queue, (q_element_t)a);
	  queue_enqueue(queue, (q_element_t)b);
	  queue_enqueue(queue, (q_element_t)c);

	  queue_enqueue(queue, (q_element_t)(&m));
	  queue_enqueue(queue, (q_element_t)(&n));
	  queue_enqueue(queue, (q_element_t)(&t));

	//  int i = 0;
	//  for(i = 0; i < 20; i++){
	//	  *a = i;
	//	  queue_enqueue(queue, (element_t)a, 1);
	//  }
	  queue_print(queue);
	  printf("Current size of the queue: %d\n", queue_size(queue));

	  p= queue_top(queue);
	  if(p != NULL)
	  {
		  printf("Top element: ");
		  int_element_print(*p);
		  printf("\n");
	  }
	  else
	  {
		  printf("Queue is empty - Top element: NULL\n");
	  }

	//  element_copy((element_t *)copied_element,*p);
	//  printf("Coppied element value: %d\n",*copied_element);
	//  printf("Free copyied element\n");
	//  element_free((element_t *)copied_element);

	  queue_dequeue(queue);
	  queue_print(queue);
	  printf("Current size of the queue: %d\n", queue_size(queue));
	  p= queue_top(queue);
	  if(p != NULL)
	  {
		  printf("Top element: ");
		  int_element_print(*p);
		  printf("\n");
	  }
	  if(queue_isFull(queue) == 1) printf("Queue Full\n");
	  else printf("Queue not Full\n");
	  if(queue_isEmpty(queue) == 1) printf("Queue empty\n");
	  else printf("Queue not empty\n");

	  queue_dequeue(queue);
	  queue_print(queue);
	  printf("Current size of the queue: %d\n", queue_size(queue));
	  p= queue_top(queue);
	  if(p != NULL)
	  {
		  printf("Top element: ");
		  int_element_print(*p);
	  }
	  if(queue_isEmpty(queue) == 1) printf("Queue empty\n");
	  else printf("Queue not empty\n");

	  queue_dequeue(queue);
	  queue_dequeue(queue);
	  queue_dequeue(queue);
	  queue_dequeue(queue);
	  queue_dequeue(queue);
	  queue_dequeue(queue);
	  queue_print(queue);
	  printf("Current size of the queue: %d\n", queue_size(queue));
	  p= queue_top(queue);
	  if(p != NULL)
	  {
		  printf("Top element: ");
		  int_element_print(*p);
	  }
	  if(queue_isEmpty(queue) == 1) printf("Queue empty\n");
	  else printf("Queue not empty\n");

	  queue_free(&queue);
}

int main( void )
{


   printf("\n**********************************************************************");
   printf("\n=================== Start Debugging \n");

   test_int_queue();

   printf("\n=================== End Debugging");
   printf("\n********************************************************************\n");
   return 0;
}


/*
 * Implement here private functions to copy, to print and to destroy an element. Do you understand why you need these functions?
 * Later you will learn how you could avoid this by using funtion pointers.
 *
 */

/*
 * Print 1 element to stdout.
 * If the definition of element_t changes, then this code needs to change as well.
 */
void int_element_print(q_element_t element)
{
  // implementation goes here
	if(element != NULL) {
		printf("int_element_print(): %5d\n", *((int *) element));
	}
}

/*
 * Copy the content (e.g. all fields of a struct) of src_element to dst_element.
 * dest_element should point to allocated memory - no memory allocation will be done in this function
 * If the definition of element_t changes, then this code needs to change as well.
 */
void int_element_copy(q_element_t *dest_element, q_element_t src_element)
{
  // implementation goes here
  *dest_element = src_element;
}

/*
 * Free the memory allocated to an element (if needed)
 * If the definition of element_t changes, then this code needs to change as well.
 */
void int_element_free(q_element_t *element)
{
	// implementation goes here
	if(*element != NULL) {
//		free(*element); // For int element, don't need to free memory
		*element = NULL;
	}
}


/*
 * Compare two element elements; returns 0(equal) or 1(unequal)  or -1 (one of elements is NULL)
 */
int queue_element_compare(q_element_t x, q_element_t y)
{
//	if(x == NULL || y == NULL) return -1;
//
//	if((((sensor_data_t *)x)->id == ((sensor_data_t *)y)->id) && (((sensor_data_t *)x)->value == ((sensor_data_t *)y)->id) && (((sensor_data_t *)x)->ts == ((sensor_data_t *)y)->ts)) return 0;
//	else return 1;
	return 0;
}
