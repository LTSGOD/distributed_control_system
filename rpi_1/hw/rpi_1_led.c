#include <stdio.h>
#include <wiringPi.h>
#include <pthread.h>
#include "rpi_1_led.h"

extern int lock;

void* LED(void* args) {

    while (1) {
        if (lock == 0) {
            digitalWrite(LED_PIN, 1);
            delay(1000);
            digitalWrite(LED_PIN, 0);
            delay(1000);
        }
    }

    return NULL;
} // LED 스레드 함수

void init_LED() {
    pinMode(LED_PIN, OUTPUT);
    
    pthread_t LED_thread;

    pthread_create(&LED_thread, NULL, LED,  NULL ); // LED 스레드 생성
}