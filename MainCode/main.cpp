#include <Arduino.h>
#include <Wire.h>
//#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <math.h>
#include <SPI.h>
#include <FlexiTimer2.h>

#include "AD9833.h"                   // use hardware SPI
#include "DisplayFunctions.h"


// These are the menu switch cases

#define DISP_FREQ 0
#define DISP_AMP  1
#define DISP_OFFS 2
#define DISP_PWM  3

#define SAMPLING_FREQUENCY 80
short unsigned int lastState = 0;
long unsigned lastBlink = 0;
volatile long int frequency  = 0;
volatile int multiplier = 1;
int lastCount = 0;
unsigned char MenuState = DISP_FREQ;
// Switch for ISR
volatile unsigned short int switchValue = 0;


void sample() {
  switchValue = PIND;
}


void setup() {
  #if DEBUG
  Serial.begin(9600);
  #endif
  pinMode(PIN_CLK,INPUT);
  pinMode(PIN_DT, INPUT);
  pinMode(PIN_SW, INPUT);
  pinMode(PIN_MULT, INPUT);
  pinMode(PIN_WAVE, INPUT);
  attachInterrupt(0, isr, FALLING);   // interrupt 0 is always connected to pin 2 on Arduino UNO
  FlexiTimer2::set(SAMPLING_FREQUENCY, sample);
  FlexiTimer2::start();
  #if DEBUG
  Serial.println("Start");
  #endif


  SetupLCD();

  // -------- Initialize SPI AD9833 to sine wave at 1 Hz ----------
  // initialise SPI
  SPI.begin();
  resetAD9833();
  frequency = 1;
  setWaveform(SELECT_SINE);
  setFreq(frequency);
    } // setup

// -----------------------------------------------------------------------------


void loop() {
  // Check to see if switches are pressed
  ChangeWaveform();

  //TODO: Write functions for the case statements

  switch(MenuState){
    case DISP_FREQ:
      MainDisplay();
      break;

    case DISP_AMP:
      // AmpDisplay();
      break;

    case DISP_OFFS:
      // OffsetDisplay();
      break;

    case DISP_PWM:
      // PWMDisplay();
      break;
  }

} //loop
