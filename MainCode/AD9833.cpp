#include <Arduino.h>
#include "AD9833.h"

#include <SPI.h>                    // use hardware SPI

// AD9833 Control Register helpers
#define CR_B28_COMBINED      0x2000
#define CR_FSELECT_0         0x0000
#define CR_PSELECT_0         0x0000
#define CR_RESET             0x0100
#define CR_OPBITEN           0x0020
#define CR_DIV2              0x0008
#define CR_MODE_D1_SINE      0x0000
#define CR_MODE_D1_TRIANGLE  0x0002

// Mnemonics for wave forms
//#define SINE                 (CR_B28_COMBINED | CR_MODE_D1_SINE)
//#define SQUARE               (CR_B28_COMBINED | CR_OPBITEN)
//#define FAST_SQUARE          (SQUARE | CR_DIV2)
//#define TRIANGLE             (CR_B28_COMBINED | CR_MODE_D1_TRIANGLE)

#define FREQ0                0x4000
#define PHASE0               0xC000
#define REF_FREQ             25000000.0
#define SPI_CLOCK_SPEED      12000000

// CUSTOM DEFINED VALUES
#define DEFAULT_CONTROL      0x2000
#define SINE                 0x0000
#define TRIANGLE             0x0002
#define SQUARE               0x0068


void resetAD9833()
{
  SPI.beginTransaction(SPISettings(SPI_CLOCK_SPEED, MSBFIRST, SPI_MODE2));
  SPI.transfer(0x2100);
  SPI.transfer(0x50C7);
  SPI.transfer(0x4000);
  SPI.transfer(0xC000);
  SPI.transfer(0x2000);

  SPI.endTransaction();

}

void setFreq(long freq)
{
  long freq_data = (freq * pow(2, 28)) / REF_FREQ;
  int freq_MSB = (int)(freq_data >> 14) | FREQ0;
  int freq_LSB = (int)(freq_data & 0x3FFF) | FREQ0;

  SPI.beginTransaction(SPISettings(SPI_CLOCK_SPEED, MSBFIRST, SPI_MODE2));

  digitalWrite(SS, LOW);

  SPI.transfer16(0x2000);
  SPI.transfer16(freq_LSB);
  SPI.transfer16(freq_MSB);


  digitalWrite(SS, HIGH);
  SPI.endTransaction();


}

void setWaveform(int waveform)
{
  SPI.beginTransaction(SPISettings(SPI_CLOCK_SPEED, MSBFIRST, SPI_MODE2));

  digitalWrite(SS, LOW);

  switch(waveform){
    case 1:
    SPI.transfer16(DEFAULT_CONTROL|SINE);
    break;
    case 2:
    SPI.transfer16(DEFAULT_CONTROL|TRIANGLE);
    break;
    case 3:
    SPI.transfer16(DEFAULT_CONTROL|SQUARE);
    break;
  }
  digitalWrite(SS, HIGH);
  SPI.endTransaction();

}
