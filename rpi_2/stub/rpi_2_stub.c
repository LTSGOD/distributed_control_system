#include <stdio.h>
#include <string.h>
#include "rpi_2_stub.h"
#include "rpi_2_can.h"

static RPC_handler_ptr handlers[100]; // RPC_handler 함수포인터 배열

int init_stub() {
    char buf[100]; // 데이터 받아올 버퍼
    char ret[4]; // 리턴값 배열

    can_init(); // can 통신 시작

    while (1) {

        // RPi#1 요청 listen
        if (can_listen(buf) < 0) {
            perror("can_listen error");
        }

        int RPC_handler_num = buf[0]; // 첫 번째 4바이트 -> handler 번호

        // 핸들러 등록x 예외처리
        if (handlers[RPC_handler_num] == NULL) {
            printf("handler not registered\n");
            return -1;
        }

        int return_value = handlers[RPC_handler_num](buf); // 요청에 맞는 RPC function call 및 데이터 buffer 전달

        memcpy(ret, &return_value, sizeof(int)); // 반환값 Marshalling
        
        // Rpi#1에 응답값 전송
        if (can_send(ret, sizeof(int)) < 0) {
            perror("can_send fail");
        }

        // TerminateRPC에 해당하는 2번 함수 호출 + 정상 응답 시 stub 종료
        if (RPC_handler_num == 2 && return_value == 0) {
            break;
        }

    }

    can_free(); // can socket 닫기
}

// RPC handler 등록
void register_RPC_handler(RPC_handler_ptr handler, int handler_num) {
    handlers[handler_num] = handler;
}
