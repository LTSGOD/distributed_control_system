#ifndef RPI1_DIJKSTRA_H_
#define RPI1_DIJKSTRA_H_

#define ALPH_NUM 26
#include "rpi_1_linkedlist.h"

void readInput();
void findShortestPath(int source, int destination, char buffer[], int* len);
void path_formatting(char buffer[], char send_buffer[], int len);
void init_djkstra();
#endif