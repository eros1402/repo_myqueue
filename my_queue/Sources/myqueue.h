/*
 ============================================================================
 Name        : myqueue.h
 Author      : cph
 Version     : 2.1
 Copyright   : Copyright from Chi Pham Hoang
 Description : Implementation of a data queue
 	 	 	   First come, First serve structure ( or FIFO)
 	 	 	   Dynamic memory
 	 	 	   Thread-safe
 Note 	     : 1) User can define QUEUE_SIZE to set the maximum
	 	 	   size of the queue. In default, QUEUE_SIZE = 15
			   2) User must implement 3 void functions to work with this API:
			   - A copy function : to read out an element in Queue
			   - A free function : to free(delete) an element in Queue
			   - A print function: to print out an element to stdout
 ============================================================================
 */

#ifndef MYQUEUE_
#define MYQUEUE_

///*
// ** The default queue size is 15
// */
//#ifndef QUEUE_SIZE
//    #define QUEUE_SIZE 15
//#endif

extern int allowed_overwrite_flag; // This flag is defined by user

/*
 **  Define the type of element as void (can be any type) that will be stored in the queue
 */
typedef void* q_element_t;
typedef struct queue queue_t;

//*define CALLBACK function (function pointer)
typedef void element_copy_func(q_element_t *, q_element_t);
typedef void element_free_func(q_element_t *);
typedef void element_print_func(q_element_t);

/*
 **  Creates (memory allocation!) and initializes the queue and prepares it for usage
 **  Return a pointer to the newly created queue
 **  Returns NULL if queue creation failed
 */
queue_t* queue_create(const int QUEUE_SIZE, element_copy_func *element_copy, element_free_func *element_free, element_print_func *element_print);

/*  
 **  Add an element to the queue
 **  If queue is full, do nothing when QUEUE_OVERIDE_FLAG = 0
 **                  & overwrite the rear element when QUEUE_OVEWRITE_FLAG = 1 (default)
 */
void queue_enqueue(queue_t* queue, q_element_t element, const int QUEUE_OVEWRITE_FLAG = 1);

/*
 **  Delete the queue from memory; set queue to NULL
 **  The queue can no longer be used unless queue_create is called again
 */
void queue_free(queue_t** queue);

/*
 **  Return the number of elements in the queue
 */
int queue_size(queue_t* queue);

/*
 **  Return a pointer to the top element in the queue
 **  Returns NULL if queue is empty
 */
q_element_t* queue_top(queue_t* queue);

/*
 **  Remove the top element from the queue
 **  Does nothing if queue is empty
 */
void queue_dequeue(queue_t* queue);

/*
 **  Print all elements in the queue, starting from the front element
 */
void queue_print(queue_t *queue);

/*
 **  Check empty queue
 **  Return 1 if queue is empty
 **  Return 0 if queue is not empty
 */
int queue_isEmpty(queue_t *queue);

/*
 **  Check full queue
 **  Return 1 if queue is full
 **  Return 0 if queue is not full
 */
int queue_isFull(queue_t *queue);

#endif //MYQUEUE_


