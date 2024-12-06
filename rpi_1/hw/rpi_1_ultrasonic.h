#ifndef RPI1_ULTRASONIC_H_
#define RPI1_ULTRASONIC_H_

#define LED_PIN 17
#define Trig    23
#define Echo    24

float measureDistance(void);
void* isObjectDetected(void*);
void init_ultrasonic();

#endif