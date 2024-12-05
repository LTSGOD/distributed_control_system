#include <stdio.h>
#include "rpi_2_terminate.h"
#include "rpi_2_stub.h"
#include "rpi_2_can.h"
#include "rpi_2_lcd.h"

// terminateRPC 함수
int terminateRPC(char* text) {
    sendBitsToLCD(0x01, LCD_RS_INST | LCD_RW_WRITE); // lcd 화면 clear
    return 0;
}

// terminate 핸들러
int terminateRPCHandler(char* buffer) {

    char* text = &buffer[4]; // 데이터 unMarshalling

    terminateRPC(text);

    printf("RPC request 'QUIT' command received\n");

    return 0;
}

// terminate 핸들러등록
void init_terminate() {
    register_RPC_handler(terminateRPCHandler, 2);
}