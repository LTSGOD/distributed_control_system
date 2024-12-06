#include <stdio.h>
#include <string.h>
#include "rpi_1_stub.h"
#include "rpi_1_can.h"

// 0번 RPC 함수
int moveMotor(int inputValue) {

    char buffer[8];
    int data[2];
    int return_value;

    data[0] = 0; // RPC id
    data[1] = inputValue; // 함수 매개변수

    memcpy(buffer, (char*)data, sizeof(data));// 포장

    // 메세지 보내기
    if (can_send(buffer, sizeof(data)) < 0) {
        perror("moveMotor call failed");
        return -1;
    }
    // 응답 대기
    return_value = can_listen();

    printf("Requested RPC moveMotor() and received return value %d\n", inputValue);

    return return_value;
}

// 1번 RPC 함수
int displayText(int lineNum, char* text) {

    char buffer[1000];
    int data[2];
    int return_value;

    data[0] = 1; // RPC id
    data[1] = lineNum; // 함수 매개변수1

    int text_length = (int)strlen(text);

    memcpy(buffer, data, sizeof(data));
    memcpy(&buffer[8], text, text_length + 1); // 널포함 복사

    // 메세지 보내기
    if (can_send(buffer, text_length + 9) < 0) {
        perror("displayText call failed");
        return -1;
    }

    // 응답 대기
    return_value = can_listen();

    printf("Requested RPC displayText() and received return value %d\n", return_value);
    return return_value;
}

// 2번 RPC 함수
int terminateRPC(char *text) {
    char buffer[1000];
    int data;
    int return_value;
    int text_length = (int)strlen(text);

    data = 2; // RPC id

    memcpy(buffer, &data, sizeof(int));
    memcpy(&buffer[4], text, text_length + 1);

    int tmp;
    memcpy(&tmp, buffer, 4);

    if (can_send(buffer, text_length + 5)) {
        perror("terminateRPC call failed");
        return -1;
    }

    // 응답 대기
    return_value = can_listen();

    return return_value;
}
