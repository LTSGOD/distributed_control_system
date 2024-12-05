#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include "rpi_2_lcd.h"
#include "rpi_2_stub.h"

int displayText(int lineNum, char* text) {
    
    int length = (int)strlen(text);
    // 첫번째 줄 출력시 지우고 WRITE
    if (lineNum == 1) {
      sendBitsToLCD(0x01, LCD_RS_INST | LCD_RW_WRITE);
      currentCursorPos = 0;
    } else {

      // 2번째 줄 맨앞으로 커서이동
      for (int i =0; i < 40 - currentCursorPos; i++) {
        sendBitsToLCD(0x14, LCD_RS_INST); // 오른쪽으로 커서 이동
      }
    }

    while (*text) {
      if (*text == '\n') {
      text++;
      continue;
      }
      sendBitsToLCD(*text, LCD_RS_DATA);
      currentCursorPos++; // 커서 위치 저장용
      text++;
    }

    return length;
}

int displayTextHandler(char* buf) {
    int lineNum = buf[4];
    char* text = &buf[8];

    int r_value = displayText(lineNum, text);

    printf("RPC request 'displayText(%d, %s)' received and processed\n", lineNum, text);

    return r_value;
}

void init_display() {
    
    deviceHandle = wiringPiI2CSetup(Detected_DEVICE_ID_BY_I2C);

    currentCursorPos = 0;

    register_RPC_handler(displayTextHandler, 1);

    // Instruction: Function Set '001D NF--' 
    sendBitsToLCD(0x33, LCD_RS_INST | LCD_RW_WRITE);       // "0011 0011" (8-bit mode, 1 line, 5x8 dots)

    // Instruction: Function Set '001D NF--'
    sendBitsToLCD(0x32, LCD_RS_INST | LCD_RW_WRITE);       // "0011 0010" (8-bit mode, 1 line, 5x8 dots)
    
    // Instruction: Entry Mode Set '0000 01IS'
    sendBitsToLCD(0x06, LCD_RS_INST | LCD_RW_WRITE);       // "0000 0110" (cursor move direction is increment, normal shift operation)

    // Instruction: Display ON/OFF Control '0000 1DCB'
    sendBitsToLCD(0x0C, LCD_RS_INST | LCD_RW_WRITE);       // "0000 1100" (display on, cursor Off, blink off)

    // Instruction: FUnction Set '001D NF--'
    sendBitsToLCD(0x28, LCD_RS_INST | LCD_RW_WRITE);       // "0010 1000" (4-bit mode, 2 lines, 5x8 dots)

    // Instruction: Clear Display
    sendBitsToLCD(0x01, LCD_RS_INST | LCD_RW_WRITE);       // "0000 0001" (clear display)

    sendBitsToLCD(0x80, LCD_RS_INST | LCD_RW_WRITE);       

    delayMicroseconds(500);
}

void sendBitsToLCD(int eightBits, int mode)
{
  int highFourBits;
  int lowFourBits;

  highFourBits = mode | (eightBits & 0xF0) | LCD_BACKLIGHT_ON;                // Let's always turn on the backlight
  lowFourBits = mode | ((eightBits << 4) & 0xF0) | LCD_BACKLIGHT_ON;          // Let's always turn on the backlight
  
  wiringPiI2CWrite(deviceHandle, highFourBits);  // 'xxxx abcd': xxxx is the high 4 bits of eightBits
  toggleLCDEnable(highFourBits);

  wiringPiI2CWrite(deviceHandle, lowFourBits);   // 'yyyy abcd': yyyy is the low 4 bits of eightBits
  toggleLCDEnable(lowFourBits);
}

void toggleLCDEnable(int eightBits)
{
  // Toggle enable pin on LCD display
  delayMicroseconds(500);
  wiringPiI2CWrite(deviceHandle, (eightBits | LCD_ENABLE));
  delayMicroseconds(500);
  wiringPiI2CWrite(deviceHandle, (eightBits | LCD_DISABLE));
  delayMicroseconds(500);
}