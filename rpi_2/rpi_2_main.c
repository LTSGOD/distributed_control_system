#include <stdio.h>
#include <wiringPi.h>
#include "rpi_2_can.h"
#include "rpi_2_stub.h"
#include "rpi_2_motor.h"
#include "rpi_2_lcd.h"
#include "rpi_2_terminate.h"

void init_hw();

int main() {
    init_hw();

    printf("RPI #2 is ready to accept RPC request\n");
    init_stub();
    printf("Terminating Rpi #2\n");
}

void init_hw() {   
    wiringPiSetupGpio();
    init_motor();
    init_display();
    init_terminate();
}