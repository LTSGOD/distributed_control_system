#ifndef RPI1_HEAPQ_H_
#define RPI1_HEAPQ_H_
#include "rpi_1_linkedlist.h"

typedef struct Heap {
    Node* heapq[100];
    int length;
} Heap;

void insert_heap(Heap* heap, int value, int node_id);
void print_heap(Heap* heap);
Node pop(Heap* heap);
void destroy_heap(Heap* heap);
#endif