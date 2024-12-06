#ifndef RPI1_CAN_H_
#define RPI1_CAN_H_

int can_init(); // CAN 초기화
int can_send(char* buffer, int length); // CAN buffer 메세지 보내기
int can_listen(); // CAN 반환 대기
int can_free(); // CAN 종료

#endif