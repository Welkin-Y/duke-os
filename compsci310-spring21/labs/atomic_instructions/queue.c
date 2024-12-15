#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <atomic>
#include "queue.h"

#define DEQUEUE_FAIL -99

// this is not a type you need to think about yourself
// I use it to do the appropiate conversions in the cas function
union union_type {
    pointer_t parts;
    unsigned __int128 as_int128;
};

bool cas(pointer_t *src,
         pointer_t expected,
         pointer_t value)
{
    union union_type uexp = { .parts = expected };
    union union_type uvalue = { .parts = value };
    return __sync_bool_compare_and_swap((__int128*) src,
                                        uexp.as_int128,
                                        uvalue.as_int128);
}

void initialize(queue_t *new_queue) {
    node_t *node = (node_t*) malloc(sizeof(node_t));
    assert(node != NULL);
    node->next.pointer = NULL;
    node->next.counter = 0;
    new_queue->head.pointer = new_queue->tail.pointer = node;
    new_queue->head.counter = new_queue->tail.counter = 0;
    
}

/* Inserts an item in the queue */
void enqueue(queue_t *queue, int value) {
    node_t *node = (node_t*) malloc(sizeof(node_t));
    assert(node != NULL);
    node->value = value;
    node->next.pointer = NULL;

    queue->tail.pointer->next.pointer = node;
    queue->tail.pointer = node;
}

/**
Removes an item from the queue and returns its value, or DEQUEUE_FAIL
if the list is empty.

 */
int dequeue(queue_t *queue) {
    
    pointer_t dummy_head = queue->head;
    if(dummy_head.pointer->next.pointer == NULL) {
        // there is always a "dummy" in our list, so if a remove would
        // elliminate it that means the list is empty
        return DEQUEUE_FAIL;
    } else {
        // in a somewhat odd approach, the value we're returning is
        // the one after the dummy head
        // the old value node becomes the new dummy head
        int result = dummy_head.pointer->next.pointer->value;
        queue->head = dummy_head.pointer->next;
        free(dummy_head.pointer);
        return result;
        }
}

// frees resources associated with the queue
// not intented to be called in a concurrent context
void destroy(queue_t *queue) {
    while(queue->head.pointer != NULL) {
        node_t *to_free = queue->head.pointer;
        queue->head = to_free->next;
        free(to_free);
    }
}
