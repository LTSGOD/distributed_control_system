#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "rpi_1_dijkstra.h"
#include "rpi_1_heapq.h"

#define FILE_NAME "map_data.txt"

int V, E; // 정점, 간선 개수
 
Node* adj_list[100]; // 인접리스트


// 파일 읽어와 인접리스트 생성
void readInput() {
    FILE* file = fopen(FILE_NAME, "r");

    fscanf(file, "%d %d",&V, &E);

    for (int i = 0; i < E; i++) {
        char from, to;
        int dist;
        fscanf(file," %c %c %d", &from, &to, &dist);

        insert(&adj_list[from -'A'], to - 'A', dist); // 인접리스트 넣기
        // insert(&adj_list[to], from, dist);
    }

    fclose(file);
}

void findShortestPath(int source, int destination, char buffer[], int* len) {
    
    readInput();

    bool visited[1000] = {false, }; //방문배열
    int distance[1000]; // 최단거리
    int prev[1000]; // 이전경로 저장
    int path_len = 0; // 최단거리 길이

    // 최단거리 배열, 이전경로 배열 초기화
    for (int i = 0; i < ALPH_NUM; i++) {
        distance[i] = 100000000;
        prev[i] = -1;
    }

    Heap heap; // 힙 생성
    memset(&heap.heapq, 0, sizeof(Heap));// 힙 초기화

    // 첫 노드 삽입
    distance[source] = 0;
    insert_heap(&heap, 0, source);

    while (heap.length != 0) {

        Node current_node = pop(&heap);

        if (visited[current_node.node_id] == true) {
            continue;
        } // 방문했으면 패스
        
        visited[current_node.node_id] = true; // 방문체크

        // 인접 노드 탐색
        Node* next_node = adj_list[current_node.node_id]; 
        while (next_node != NULL) {

            int next_distance = current_node.value + next_node->value;

            if (next_distance < distance[next_node->node_id]) { // 더짧은 거리 갱신
                prev[next_node->node_id] = current_node.node_id; // 이전 노드 정보 저장
                distance[next_node->node_id] = next_distance;
                insert_heap(&heap, next_distance, next_node->node_id); // 힙 삽입
            }
            next_node = next_node->next;
        }
    }

    // 최단거리 buffer에 저장
    int root = destination;
    
    char buffer_tmp[100];

    while (prev[root] != -1) {
        buffer_tmp[path_len] = root +'A';
        root = prev[root];
        path_len++;
    }

    if (len > 0) {
        buffer_tmp[path_len++] = source +'A';
    }

    *len = path_len;

    // 형식에 맞게 반환 ex) A B D to A -> B -> D
    path_formatting(buffer_tmp, buffer, *len);


    // for (int i = 0; i < V; i++) {
    //     printf("%d ", distance[i]);
    // }
    // printf("\n");

    // for (int i = 0; i < V; i++) {
    //     printf("%d ", prev[i]);
    // }
    // printf("\n");

    // for (int i = 0; i <path_len; i++) {
    //     printf("%c ", buffer[i]);
    // }
    // printf("\n");

    // 연결리스트 & 힙 메모리 해제
    for (int i = 0; i <= V; i++) {
        destroy(adj_list[i]);
    }
    destroy_heap(&heap);
}

void path_formatting(char buffer[], char send_buffer[], int len) {

    sprintf(send_buffer, "%c", buffer[len - 1]);
    for (int i = len - 2; i >= 0; i--) {
        char tmp[10];
        sprintf(tmp, " -> %c", buffer[i]);
        strcat(send_buffer, tmp);
    }
}

void init_djkstra() {

}