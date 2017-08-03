/*
 ============================================================================
 Name        : myqueue.c
 Author      : cph
 Version     : 2.1
 Copyright   : Copyright from Chi Pham Hoang
 Description : Implementation of a data queue
 	 	 	   First come, First serve structure ( or FIFO)
 	 	 	   Dynamic memory
 	 	 	   Thread-safe
 ============================================================================
 */

//#define _GNU_SOURCE // This define is default in g++
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 
#include "myqueue.h"

/*
 * The real definition of 'a queue struct'
 */ 
struct queue {
  q_element_t *arr; 	// dynamic array containing data elements
  int max_size;		// maximum size of the queue
  int current_size; // Counts number of current elements in the queue
  int front, rear;	// Position of front & rear element in queue
  
  element_copy_func *element_copy;  	//callback function for clone a queue element
  element_free_func *element_free;  	//callback function for free a queue element
  element_print_func *element_print;	//callback function for print a queue element to stdout

  // Extra field to make it a thread-safe queue
  pthread_mutex_t data_mutex;  			// For thread-safe
};

void mem_alloc_check(void *p, char *msg) {
	if(p == NULL) {
		fprintf(stderr, "\n%s: memory allocation error\n", msg);
		exit(-1);
	}
}

void queue_check(queue_t* queue, char *msg) {
	if(queue == NULL) {
		fprintf(stderr, "\n%s: input pointer is NULL\n", msg);
		exit(-1);        
	}
}

void pthread_check( int err_code, char *msg, char *file_name, int line_nr )
{
	if ( 0 != err_code ) {
		fprintf( stderr, "\n%s failed with error code %d in file %s at line %d\n", msg, err_code, file_name, line_nr );		
	}
}

/*
 **  Creates (memory allocation!) and initializes the queue and prepares it for usage
 **  Return a pointer to the newly created queue
 **  Returns NULL if queue creation failed
 */
queue_t* queue_create(const int QUEUE_SIZE, element_copy_func *element_copy, element_free_func *element_free, element_print_func *element_print)
{	
  // implementation goes here	
	queue_t* myqueue;	
	myqueue = (queue_t *) malloc(sizeof(queue_t)); // Queue memory allocated
	if(myqueue == NULL) {return NULL;}	
	myqueue->max_size = QUEUE_SIZE;
	myqueue->arr = (q_element_t *) malloc(QUEUE_SIZE*sizeof(q_element_t));
	mem_alloc_check((q_element_t *) myqueue->arr, "Error queue_create()");
	myqueue->current_size = 0;
	myqueue->front = 0;
	myqueue->rear = -1;		
	
	// init function pointers
	myqueue->element_copy = element_copy;
	myqueue->element_free = element_free;	
	myqueue->element_print = element_print;
	
	// init thread-safe mutex
	pthread_mutex_init(&(myqueue->data_mutex), NULL);	
	return myqueue;
}

/*
 **  Delete the queue from memory; set queue to NULL
 **  The queue can no longer be used unless queue_create is called again
 */
void queue_free(queue_t** queue)
{
	// Check if the queue is NULL
	queue_check(*queue, "queue_free()");
	int i=0;
	// Check if the queue is not empty
	if((*queue)->current_size != 0)	{
		// free element in each node of the queue	
		for(i = 0; i < (*queue)->max_size; i++) {
			if((*queue)->arr[i] != NULL) {
				(*queue)->element_free(&((*queue)->arr[i]));
			}
		}
	}	
	
	pthread_mutex_destroy( &((*queue)->data_mutex) );	
	free((*queue)->arr); 	//free element array in queue
	free(*queue); 			//free the queue
	*queue = NULL; 		
}

/*  
 **  Add an element to the queue
 **  (Does nothing if queue is full)
 **  Update 22-Jul-15: If the queue is full, overide the the rear when the allowed flag is set. Otherwise, do nothing
 */
void queue_enqueue(queue_t* queue, q_element_t element, const int QUEUE_OVEWRITE_FLAG)
{
	int presult;
  // implementation goes here
	queue_check(queue, "Error queue_enqueue()");	
	
	presult = pthread_mutex_lock( &(queue->data_mutex) );	
	pthread_check( presult, "Error queue_enqueue(): pthread_mutex_lock", __FILE__, __LINE__ );
	
	if(queue->current_size == queue->max_size) {
		// printf("Queue is full !!!\n");
		if(QUEUE_OVEWRITE_FLAG) {
			// free the current rear
			queue->element_free(&(queue->arr[queue->rear]));
			// overide the current rear
			queue->element_copy(&(queue->arr[queue->rear]), element);	
		}		        
	}
	else {
		queue->current_size = queue->current_size + 1;	
		queue->rear = queue->rear + 1;
		if(queue->rear == queue->max_size) {
			queue->rear = 0; //circulating
		}		
		// Make a deep copy
		queue->element_copy(&(queue->arr[queue->rear]), element);
	}
	
	presult = pthread_mutex_unlock( &(queue->data_mutex) );
	pthread_check( presult, "Error queue_enqueue(): pthread_mutex_unlock", __FILE__, __LINE__ );
	// signal the condition variable
}

/*
 **  Return the number of elements in the queue
 */
int queue_size(queue_t* queue)
{
	queue_check(queue, "Error queue_size()");
    return queue->current_size;
}

/*
 **  Return a pointer to the top element in the queue
 **  Returns NULL if queue is empty
 */
q_element_t* queue_top(queue_t* queue){
	int presult;
	q_element_t *p;
	// implementation goes here
	queue_check(queue, "Error queue_top()");
	
	presult = pthread_mutex_lock( &(queue->data_mutex) );	
	pthread_check( presult, "Error queue_top(): pthread_mutex_lock", __FILE__, __LINE__ );
  	//Check if the queue is empty
  	if(queue->current_size != 0) {		
		p = &(queue->arr[queue->front]);
		presult = pthread_mutex_unlock( &(queue->data_mutex) );
		pthread_check( presult, "Error queue_top(): pthread_mutex_unlock", __FILE__, __LINE__ );
		return p;
	}
  	else {		
		presult = pthread_mutex_unlock( &(queue->data_mutex) );
		pthread_check( presult, "Error queue_top(): pthread_mutex_unlock", __FILE__, __LINE__ );
		return NULL;
	}
}

/*
 **  Remove the top element from the queue
 **  Does nothing if queue is empty
 */
void queue_dequeue(queue_t* queue)
{
	int presult;
   //Check the queue is Null
	queue_check(queue, "Error queue_dequeue(): ");
	
	presult = pthread_mutex_lock( &(queue->data_mutex) );	
	pthread_check( presult, "Error queue_dequeue(): pthread_mutex_lock", __FILE__, __LINE__ );
	if(queue->current_size != 0) {		
		queue->element_free(&(queue->arr[queue->front]));
		queue->front = queue->front + 1;
		if(queue->front == queue->max_size) {
			queue->front = 0; //circulating
		}
		queue->current_size = queue->current_size - 1;
	}	
	else {
		//Queue is empty, do nothing
		printf("Warning queue_dequeue(): Queue is empty !!!\n");
	}
	presult = pthread_mutex_unlock( &(queue->data_mutex) );
	pthread_check( presult, "Error queue_dequeue(): pthread_mutex_unlock", __FILE__, __LINE__ );
}

/*
 **  Print all elements in the queue, starting from the front element
 */
void queue_print(queue_t *queue)
{
  // implementation goes here  
  queue_check(queue, "Error queue_print()");
  if(queue->current_size == 0) {	  
	  printf("Warning queue_print(): Queue is empty !!!\n");
	  return;
  }
  
  int pos;  
  pos= queue->front;  
  q_element_t e = queue->arr[pos];  
  
  printf("********** queue_print() (from front to rear)\n");
  
  queue->element_print(e);
  while(pos != queue->rear)  
  {
	  pos++;	  
	  if(pos == queue->max_size) { pos = 0;}
	  e = queue->arr[pos];
	  queue->element_print(e);
  } 
}

/*
 **  Check empty queue
 **  Return 1 if queue is empty
 **  Return 0 if queue is not empty
 */
int queue_isEmpty(queue_t *queue)
{
	//Check the queue is Null
	queue_check(queue, "Error queue_isEmpty(): ");
	if(queue->current_size == 0) return 1;
	else return 0;
}

/*
 **  Check full queue
 **  Return 1 if queue is full
 **  Return 0 if queue is not full
 */
int queue_isFull(queue_t *queue)
{
	//Check the queue is Null
	queue_check(queue, "Error queue_isFull(): ");
	if(queue->current_size == queue->max_size) return 1;
	else return 0;
}
