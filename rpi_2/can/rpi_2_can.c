#include <stdio.h>
#include <string.h>
#include <unistd.h> // POSIX api

#include <sys/socket.h> //socket 관련
#include <sys/ioctl.h>

#include <net/if.h> // struct ifreq 

#include <linux/can.h>
#include <linux/can/raw.h>
#include "rpi_2_can.h"

#define END_ID 0x666 // sender의 보내는 메세지들 중 마지막 메세지 can_id
#define RESPONSE_ID 0x555 // RPI2 RPC 서버의 응답 can_id

int socket_descripter;
unsigned int received_can_id = 0x000; // sender의 마지막 메세지가 아닌 can_id

int can_init() {
    struct ifreq ifr;
    struct sockaddr_can addr;

    socket_descripter = socket(AF_CAN, SOCK_RAW,CAN_RAW);

    if (socket_descripter < 0) {
        perror("Socket() fail");
        return -1;
    }

    strcpy(ifr.ifr_name, "can0");
    ioctl(socket_descripter, SIOCGIFINDEX, &ifr);

    memset(&addr, 0, sizeof(struct sockaddr_can));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(socket_descripter, (struct sockaddr*)&addr, sizeof(struct sockaddr_can)) < 0) { 
        perror("Bind() fail");
        return -1;
    }

    return 0;
}

// can socket 닫기
int can_free() {
    if (close(socket_descripter) < 0) {
        perror("Close() fail");
        return -1;
    }
}

int can_send(char* buffer, int length) {

    struct can_frame frame;

    frame.can_dlc = length;
    frame.can_id = RESPONSE_ID; //응답메세지 can_id설정
    memcpy(frame.data, buffer, length);

    int tmp = write(socket_descripter, &frame, sizeof(struct can_frame));
    if(tmp != sizeof(struct can_frame)) {
        perror("Write() fail");
        return -1;
    }

    // DEBUG용
    // printf("data: %d\n", *(int*)&frame.data[0]);
    // printf("data2 : %d\n", frame.data[4]);
    
    // printf("0x%03X [%d] sendData: %d %d\n",frame.can_id, frame.can_dlc, *(int*)&frame.data[0], *(int*)&frame.data[4]);

    return 0;
}

int can_listen(char* buf) {

    struct can_frame frame;

    while (1) {
        int nBytesRead;
        nBytesRead = read(socket_descripter, &frame, sizeof(struct can_frame));   

        if (nBytesRead < 0) {
            perror("READ() fail");
            return -1;
        }

        // loopback echo 문제 + 응답메세지 필터링
        if ((frame.can_id == received_can_id) || frame.can_id == RESPONSE_ID) {
            received_can_id = 0x000;
            continue;
        }

        received_can_id = frame.can_id;
        
        // printf("0x%03X [%d] %dBytesRead\n",frame.can_id, frame.can_dlc, nBytesRead);
        memcpy(buf, frame.data, frame.can_dlc);

        buf+= frame.can_dlc; // 뒤이어 오는 메세지 저장

        // 마지막 메세지면 listen 종료
        if (frame.can_id == END_ID) break;
    }
}