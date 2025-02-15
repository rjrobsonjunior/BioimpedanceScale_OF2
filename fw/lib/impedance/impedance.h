#ifndef IMPEDANCE_H
#define IMPEDANCE_H

#include <Wire.h>
#include <Arduino.h>
#include <stdint.h>

// Definição de registradores do AD5933
#define SLAVEADDR 0x0D
#define ADDRPTR 0xB0
#define STARTFREQ_R1 0x82
#define STARTFREQ_R2 0x83
#define STARTFREQ_R3 0x84
#define FREGINCRE_R1 0x85
#define FREGINCRE_R2 0x86
#define FREGINCRE_R3 0x87
#define NUMINCRE_R1 0x88
#define NUMINCRE_R2 0x89
#define NUMSCYCLES_R1 0x8A
#define NUMSCYCLES_R2 0x8B
#define REDATA_R1 0x94
#define REDATA_R2 0x95
#define IMGDATA_R1 0x96
#define IMGDATA_R2 0x97
#define CTRLREG 0x80
#define CTRLREG2 0x81
#define STATUSREG 0x8F

typedef enum {
  WAITING_START = 0,
  READING_USERDATA,
  MEASURING,
  SENDING_RESULTS
} STATES;

void impedance_setup();
STATES impedance_runSweep();
void impedance_programReg();
void impedance_writeData(int addr, int data);
int impedance_readData(int addr);
byte impedance_getFrequency(float freq, int n);

extern STATES actual_state;
extern double impedance_50khz;
extern double impedance_100khz;

#endif
