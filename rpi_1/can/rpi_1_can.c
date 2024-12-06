#include <stdio.h>
#include <string.h>
#include <unistd.h> // POSIX api

#include <sys/socket.h> //socket 관련
#include <sys/ioctl.h>

#include <net/if.h> // struct ifreq 

#include <linux/can.h>
#include <linux/can/raw.h>

#include "rpi_1_can.h"


#define END_NUM 0x666

int socketDescripter;

int can_init() {
    struct ifreq ifr;
    struct sockaddr_can addr;

    socketDescripter = socket(AF_CAN, SOCK_RAW,CAN_RAW);

    if (socketDescripter < 0) {
        perror("Socket() fail");
        return -1;
    }

    strcpy(ifr.ifr_name, "can0");
    ioctl(socketDescripter, SIOCGIFINDEX, &ifr);

    memset(&addr, 0, sizeof(struct sockaddr_can));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(socketDescripter, (struct sockaddr*)&addr, sizeof(struct sockaddr_can)) < 0) { 
        perror("Bind() fail");
        return -1;
    }

    return 0;
}

int can_free() {
    if (close(socketDescripter) < 0) {
        perror("Close() fail");
        return -1;
    }
}

int can_send(char* buffer, int length) {

    int total_packet_num = length / 8;
    int last_packet_length = length % 8;

    struct can_frame frame;

    if (length > 7) {

        for (int i = 0; i < total_packet_num; i++) {
            frame.can_dlc = 8;

            // 8의 배수로 패킷이 들어오면 마지막 패킷에 end 메세지 표시
            if (last_packet_length == 0 && i == total_packet_num - 1) {
                frame.can_id = END_NUM;
            } else {
                frame.can_id = 0x001;
            }

            memcpy(frame.data, buffer + i*8, 8);
            
            // printf("0x%03X [%d] sendData: %d %d\n",frame.can_id, frame.can_dlc, buffer[0], buffer[4]);
            int tmp = write(socketDescripter, &frame, sizeof(struct can_frame));
            if(tmp != sizeof(struct can_frame)) {
                perror("Write() fail");
                return -1;
            }
        }
    }

    if (last_packet_length != 0) {
        frame.can_dlc = last_packet_length;

        frame.can_id = END_NUM;

        memcpy(frame.data, buffer + total_packet_num * 8, frame.can_dlc);
        
        // printf("0x%03X [%d] sendData: %d %d\n",frame.can_id, frame.can_dlc, buffer[0], buffer[4]);
        int tmp = write(socketDescripter, &frame, sizeof(struct can_frame));
        if(tmp != sizeof(struct can_frame)) {
            perror("Write() fail");
            return -1;
        }
    }

    return 0;
}

int can_listen() {
    struct can_frame frame;
    int data[2];

    // 루프백 처리
    read(socketDescripter, &frame, sizeof(struct can_frame));
    while (1) {

        int nBytesRead = read(socketDescripter, &frame, sizeof(struct can_frame));
        
        // 응답메세지만 필터링
        if (frame.can_id != 0x555) {
            continue;
        }

        if (nBytesRead < 0) {
            perror("canSocket read fail");
            return -1;
        }

        memcpy(&data, &frame.data, 4);
        break;
    }

    return data[0];
}
