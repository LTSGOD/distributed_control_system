#ifndef RPI2_LCD_H_
#define RPI2_LCD_H_

#define Detected_DEVICE_ID_BY_I2C       0x27        // Device ID detected by I2C 
                                                    // Seems that it maps to the device's address
#define LCD_BACKLIGHT_ON                0x08        // On             "0000 1000"
#define LCD_BACKLIGHT_OFF               0x00        // Off            "0000 0000"
#define LCD_ENABLE                      0x04        // Enable         "0000 0100"
#define LCD_DISABLE                     0x00        // Disable        "0000 0000"
#define LCD_RW_READ                     0x02        // Read           "0000 0010" 
#define LCD_RW_WRITE                    0x00        // Write          "0000 0000"
#define LCD_RS_DATA                     0x01        // Data           "0000 0001"
#define LCD_RS_INST                     0x00        // Instruction    "0000 0000"

static int deviceHandle;                                   // Seems that it maps to the register address of the device
static int currentCursorPos;

int displayText(int lineNum, char* text);
int displayTextHandler(char* buf);
void init_display();

void sendBitsToLCD(int eightBits, int mode);
void toggleLCDEnable(int eightBits);
#endif