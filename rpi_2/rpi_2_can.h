#ifndef RPI2_CAN_H_
#define RPI2_CAN_H_

int can_init();
int can_send(char* buffer, int length);
int can_listen();
int can_free();

#endif