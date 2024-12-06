#include <stdio.h>
#include <wiringPi.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include "rpi_1_can.h"
#include "rpi_1_stub.h"
#include "rpi_1_led.h"
#include "rpi_1_ultrasonic.h"
#include "rpi_1_linkedlist.h"
#include "rpi_1_dijkstra.h"

int lock = 0; // ultrasonic 과 led 제어용 lock

void init_hw();

int main() {

    // Rpi1 hw 초기화(LED, 초음파 센서, CAN 통신)
    init_hw();


    // 최단경로 LCD 출력
    char dijkstra_buffer[100];
    int len;
    findShortestPath(0, 3, dijkstra_buffer, &len); // 다익스트라 경로 탐색

    displayText(1, dijkstra_buffer);
    displayText(2, "you made it !!");


    // 모터 제어
    moveMotor(180);
    delay(600);
    moveMotor(0);
    delay(600);
    moveMotor(180);
    delay(600);
    moveMotor(0);

    //  사용자 입력 LCD 출력
    while (1) {
        char input[1000];
        int length;
        printf("Enter your text to display on RPi #2's LCD: ");
        fgets(input, 1000, stdin);
        
        length = (int)strlen(input);
        input[length - 1] = '\0'; // 개행문자 제거

        if (strcmp(input, "quit") == 0) {
            
            if (terminateRPC(input) == 0) {
                digitalWrite(LED_PIN, 0); //LED 끄기
                break;
            }
        } else {
            if (displayText(1, input) == -1) {
                printf("displayText call fail");
            }
        }
    }

    printf("Terminating Rpi #1\n");
    can_free();

    return 0;
}

void init_hw() {
    wiringPiSetupGpio();

    init_LED(); // led 스레드 시작
    init_ultrasonic();  // ultrasonic 스레드 시작

    can_init(); //can 통신 세팅
}