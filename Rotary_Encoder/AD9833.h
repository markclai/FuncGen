#ifndef AD9833H
#define AD9833H

#define SELECT_SINE     1
#define SELECT_TRI      2
#define SELECT_SQR      3

void resetAD9833();
void setFreq(long freq);
void setWaveform(int waveform);


#endif
