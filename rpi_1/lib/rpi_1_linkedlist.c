#include <stdio.h>
#include <stdlib.h>
#include "rpi_1_linkedlist.h"

void insert(Node** pp, int node_id, int value) {

    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->node_id = node_id;
    new_node->value = value;
    new_node->next = *pp;

    *pp = new_node;
}

void print(Node* p) {
    while (p != NULL) {
        printf("%d ", p->node_id);
        p = p->next;
    }
    printf("\n");
}

void destroy(Node* p) {

    while (p != NULL) {
        Node* tmp = p;
        p = p->next;
        free(tmp);
    }
}