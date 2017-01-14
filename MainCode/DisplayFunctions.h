#ifndef DISPLAYFUNCTIONS
#define DISPLAYFUNCTIONS

#define DEBUG     0       // Set to 1 for Serial Debugging

#define I2C_ADDR    0x3F // <<----- Add your address here.  Find it from I2C Scanner
#define BACKLIGHT_PIN     3
#define En_pin    2
#define Rw_pin    1
#define Rs_pin    0
#define D4_pin    4
#define D5_pin    5
#define D6_pin    6
#define D7_pin    7

#define PIN_CLK   2     // Used for generating interrupts using CLK signal
#define PIN_DT    4     // Used for reading DT signal
#define PIN_SW    9     // Used for the push button switch
#define PIN_MULT  8
#define PIN_WAVE  7

void MainDisplay();
void isr ();
void SetupLCD();
void ChangeWaveform();


extern volatile unsigned short int switchValue;

extern short unsigned int lastState;
extern long unsigned lastBlink;
extern volatile long int frequency;
extern volatile int multiplier;
extern int lastCount;
#endif
