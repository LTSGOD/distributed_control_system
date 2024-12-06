#ifndef RPI1_LINKEDLIST_H_
#define RPI1_LINKEDLIST_H_

typedef struct Node Node;

// 노드 정의
typedef struct Node {
    int node_id;
    int value;
    Node* next;
} Node;

void insert(Node** pp, int node_id, int value);
void print(Node*);
void destroy(Node*);

#endif