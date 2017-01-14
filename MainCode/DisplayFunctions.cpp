#include "DisplayFunctions.h"
#include <Arduino.h>
#include "AD9833.h"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C  lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);


void MainDisplay() {
  if (!(digitalRead(PIN_SW))) {        // check if pushbutton is pressed
            frequency = 1;            // if YES, then reset counter to ZERO
            while (!digitalRead(PIN_SW)) {}  // wait til switch is released
            delay(10);                      // debounce
            #if DEBUG
            Serial.println("Reset");        // Using the word RESET instead of COUNT here to find out a buggy encoder
            #endif
            }
        if(!(digitalRead(PIN_MULT)))
        {
            if(multiplier == 100)
                multiplier = 1;
            else
                multiplier = multiplier * 10;
            while(!digitalRead(PIN_MULT)) {}
        }
        if (frequency != lastCount) {
            lastCount = frequency;
            setFreq(frequency);
            #if DEBUG
            Serial.print("Count");
            Serial.println(frequency);
            #endif
            for(int i = 0; i < 14; i++)
             {
               lcd.setCursor(i,1);
               lcd.print(" ");
             }

            lcd.setCursor(0,1);
            lcd.print(frequency,DEC);
            }
       lcd.setCursor(log10(frequency) - log10(multiplier),1);
       if(lastState == 0 && (millis() - lastBlink > 500))
       {
        lcd.cursor();
        lastState = 1;
        lastBlink = millis();
       }
       else if(lastState == 1 && (millis() - lastBlink > 500))
       {
        lcd.noCursor();
        lastState = 0;
        lastBlink = millis();
       }

}



// Interrupt Service Routine
void isr ()  {

    static unsigned long                lastInterruptTime = 0;

    unsigned long                       interruptTime = millis();

    // If interrupts come faster than 5ms, assume it's a bounce and ignore
    if (interruptTime - lastInterruptTime > 5) {
        if (!digitalRead(PIN_DT))   // Increment the frequency
            frequency = (frequency + 1*multiplier);
        else{
          if(frequency - 1 * multiplier > 0)  // Decrement the frequency
            frequency = frequency - 1*multiplier;
          else if (multiplier != 1){         // If sitting at a 0, decrement by one, decrease the multiplier
            multiplier = multiplier / 10;
            frequency -= multiplier;
          }

        }
      }
    lastInterruptTime = interruptTime;
    } // ISR
#define SW1 0x80

unsigned short int WaveState = 0;
bool WaveSelectPressed = false;

void ChangeWaveform() {
  // Check the switch values
  if(!(switchValue & SW1)) {
    if(!WaveSelectPressed){
      WaveSelectPressed = true;
      switch (WaveState) {
        case 0:
          setWaveform(SELECT_SINE);
          WaveState = 1;
          lcd.setCursor(10, 0);
          lcd.print("SINE  ");
          break;
        case 1:
          setWaveform(SELECT_TRI);
          WaveState = 2;
          lcd.setCursor(10, 0);
          lcd.print("TRI   ");
          break;
        case 2:
          setWaveform(SELECT_SQR);
          WaveState = 0;
          lcd.setCursor(10, 0);
          lcd.print("SQUARE");
          break;
      }
    }

  }
  else
    WaveSelectPressed = false;

}
void AmpDisplay(){
}
void SetupLCD() {
  lcd.begin (16,2); //  <<----- My LCD was 16x2
  // Switch on the backlight
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home (); // go home
  lcd.setCursor(0,0);
  lcd.print("Frequency");
  lcd.setCursor(10, 0);
  lcd.print("SINE");
  lcd.setCursor(14,1);
  lcd.print("Hz");
  lcd.setCursor(0,1);
  lcd.print(frequency, DEC);
  lastBlink = millis();
}
