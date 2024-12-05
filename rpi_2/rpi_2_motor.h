#ifndef RPI2_MOTOR_H_
#define RPI2_MOTOR_H_

#define PIN 18
#define PWM_RANGE 200

int moveMotor(int inputValue);
int moveMotorHandler(char* buf);
void init_motor();

#endif