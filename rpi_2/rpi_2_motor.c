#include <stdio.h>
#include <softPwm.h>
#include <wiringPi.h>
#include "rpi_2_stub.h"
#include "rpi_2_motor.h"

int moveMotor(int inputValue) {

    float dutyCycle;
    int pulseWidth;         

    dutyCycle = inputValue / 180.0 * 5 + 5.0;
    pulseWidth = dutyCycle * (PWM_RANGE / 100);

    softPwmWrite(PIN, pulseWidth);

    return 0;
}

// 모터 세팅 및 motor handler 등록
void init_motor() {

    pinMode(PIN, OUTPUT);
    // Set PWM range (= the total number of clock ticks during a period)
    softPwmCreate(PIN, 0, PWM_RANGE);

    register_RPC_handler(moveMotorHandler, 0); // Motor handler 등록
}

// buf데이터 unMarshalling 전달
int moveMotorHandler(char* buf) {
    int value = buf[4]; // 첫번째 인수전달

    int return_value = moveMotor(value); // 실제 함수 호출

    printf("RPC request 'moveMotor(%d)' received and processed\n", value);

    return return_value;
}