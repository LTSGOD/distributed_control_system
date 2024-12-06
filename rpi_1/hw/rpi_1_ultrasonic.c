#include <stdio.h>
#include <wiringPi.h>
#include <pthread.h>
#include <sys/time.h>
#include "rpi_1_ultrasonic.h"

extern int lock;

float measureDistance(void) 
{   
    // trig 발사
    digitalWrite(Trig, 1);
    // 10 micro seconds wait
    delayMicroseconds(10);
    // trig 끄기
    digitalWrite(Trig, 0);


    struct timeval tv1;
    struct timeval tv2;

    long start;
    long end;

    while (digitalRead(Echo) == LOW) {
        gettimeofday(&tv1, NULL);
        start = tv1.tv_sec * 1000000 + tv1.tv_usec;
    }

    while (digitalRead(Echo) == HIGH) {
        gettimeofday(&tv2, NULL);
        end = tv2.tv_sec * 1000000 + tv2.tv_usec;
    }

    float distance = (end - start) / 2000000.0 * 34300;

    return distance;
}

void* isObjectDetected(void*) {
    float distance;

    while (1) {
        distance = measureDistance();

        // printf("%0.2f cm\n", distance);
        if (distance < 20.0) {
            int i = 0;
            lock = 1;
            while (i < 5) {
                digitalWrite(LED_PIN, 1);
                delay(100);
                digitalWrite(LED_PIN, 0);
                delay(100);
                i++;
            }
            lock = 0;
        }
        delay(1000);
    }

    return NULL;
}

void init_ultrasonic() {
    pinMode(Echo, INPUT);
    pinMode(Trig, OUTPUT);

    pthread_t UltraSonic_thread;
    pthread_create(&UltraSonic_thread, NULL, isObjectDetected,  NULL );
}