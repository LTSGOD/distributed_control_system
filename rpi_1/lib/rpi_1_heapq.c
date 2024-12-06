#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "rpi_1_heapq.h"


void insert_heap(Heap* heap, int value, int node_id) {

    Node* newNode = (Node*)malloc(sizeof(Node));
    int index = heap->length;

    newNode->value = value;
    newNode->node_id = node_id;

    heap->heapq[index] = newNode;

    int parentIndex = (index + 1) / 2 ;
    int myIndex = index + 1;

    // printf("parentIndex: %d myIndex: %d\n", parentIndex, myIndex);

    if (index + 1 == 1) {
        heap->heapq[index] = newNode;
    } else {
        while (parentIndex != 0) {
        
            // 부모가 더 작으면
            if (heap->heapq[myIndex-1]->value < heap->heapq[parentIndex-1]->value) {
                Node* tmp = heap->heapq[myIndex-1];
                heap->heapq[myIndex-1] = heap->heapq[parentIndex-1];
                heap->heapq[parentIndex-1] = tmp;
            } else {
                break;
            }

            myIndex = parentIndex;
            parentIndex = myIndex / 2;
        }
    }

    heap->length++;
}

void print_heap(Heap* heap) {
    printf("print_heap: ");
   for (int i = 0; i < heap->length; i++) {
        printf("%d ", heap->heapq[i]-> value);
   } 
   printf("\n");
}

Node pop(Heap* heap) {

    if (heap->length == 0) {
        Node n;
        perror("Empty heap");
        return n;
    }

    Node node;
    node.node_id = heap->heapq[0]->node_id;
    node.value = heap->heapq[0]->value;
    free(heap->heapq[0]);

//  첫번째 원소 pop + 마지막 원소 첫번째로 옮기기
    heap->heapq[0] = heap->heapq[heap->length - 1];
    heap->heapq[heap->length - 1] = NULL;
    heap->length--;

    int index = 1;

    while (index < heap->length) {

        Node* currentNode;
        Node* leftNode = NULL;
        Node* rightNode = NULL;
        int minIndex;

        if (heap->heapq[index*2 - 1] != NULL) {
            leftNode = heap->heapq[index*2 - 1];
        }

        if (heap->heapq[index*2] != NULL) {
            rightNode = heap->heapq[index*2];
        }
       
        // printf("index %d\n", index);
        // print_heap(heap);
        // printf("%p %p\n", leftNode, rightNode);

        // 자식노드 중 작은 노드 선정
        if ((leftNode == NULL) && (rightNode == NULL)) {
            break;
        } else if (leftNode == NULL) {
            minIndex = index * 2;
        } else if (rightNode == NULL) {
            minIndex = index * 2 - 1;
        } else {
            minIndex = leftNode->value > rightNode->value ? index * 2 : index*2 - 1;
        }

        if (heap->heapq[minIndex]->value < heap->heapq[index - 1]->value) {

            // swap
            Node* tmp = heap->heapq[minIndex];
            heap->heapq[minIndex] = heap->heapq[index - 1];
            heap->heapq[index - 1] = tmp;
        } else {
            break;
        }

        index = minIndex + 1;
    }

    return node;
}

void destroy_heap(Heap* heap) {
    for (int i = 0; i < heap->length; i++) {
        free(heap->heapq[i]);
    }
}