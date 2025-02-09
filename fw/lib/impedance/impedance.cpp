#include "impedance.h"

const float MCLK = 16.776 * pow(10, 6); // AD5933 Internal Clock Speed 16.776 MHz
const float startfreq = 50 * pow(10, 3); // Frequência inicial de 50 kHz
const float increfreq = 1 * pow(10, 3); // Incremento de frequência de 1 kHz
const int increnum = 50; // Número de incrementos

// Fatores de ganho calculados para frequências específicas
const double gain_factor_50khz  = 0.0894676; // Gain Factor at 50 kHz  Known Impedance: ~1 kOhm (985 Ohm)
const double gain_factor_100khz = 0.09138455; // Gain Factor at 100 kHz  Known Impedance: ~1 kOhm (985 Ohm)

// Variáveis globais de impedância
double impedance_50khz;
double impedance_100khz;

// Estado atual da máquina de estados
STATES actual_state = MEASURING;

void impedance_setup() {
  Wire.begin(); //SDA - PIN 21, SCL - PIN 22
  Serial.begin(9600);

  impedance_writeData(CTRLREG, 0x0); // Limpa o registrador de controle
  impedance_writeData(CTRLREG2, 0x10); // Reset / Usa o clock interno
  impedance_programReg();
  Serial.println("I2C inicializado.");
}

void impedance_programReg() {
   // Set Range 1, PGA gain 1
  impedance_writeData(CTRLREG, 0x01);

  // Set settling cycles
  impedance_writeData(NUMSCYCLES_R1, 0x07);
  impedance_writeData(NUMSCYCLES_R2, 0xFF);

  // Start frequency of 50 kHz
  impedance_writeData(STARTFREQ_R1, impedance_getFrequency(startfreq, 1));
  impedance_writeData(STARTFREQ_R2, impedance_getFrequency(startfreq, 2));
  impedance_writeData(STARTFREQ_R3, impedance_getFrequency(startfreq, 3));
  
  // Increment by 1 kHz
  impedance_writeData(FREGINCRE_R1, impedance_getFrequency(increfreq, 1));
  impedance_writeData(FREGINCRE_R2, impedance_getFrequency(increfreq, 2));
  impedance_writeData(FREGINCRE_R3, impedance_getFrequency(increfreq, 3));
  
  // Points in frequency sweep (100), max 511
  impedance_writeData(NUMINCRE_R1, (increnum & 0x001F00) >> 0x08);
  impedance_writeData(NUMINCRE_R2, (increnum & 0x0000FF));
}

STATES impedance_runSweep() {
  short re, img;
  float freq;
  double mag;
  int i = 0;

  impedance_50khz = 0;
  impedance_100khz = 0;

  impedance_programReg();

  // Inicializa e executa a varredura
  impedance_writeData(CTRLREG, (impedance_readData(CTRLREG) & 0x07) | 0xB0);
  impedance_writeData(CTRLREG, (impedance_readData(CTRLREG) & 0x07) | 0x10);
  impedance_writeData(CTRLREG, (impedance_readData(CTRLREG) & 0x07) | 0x20);

  while ((impedance_readData(STATUSREG) & 0x07) < 4) { 
    delay(100);
    
    if ((impedance_readData(STATUSREG) & 2) == 2) {
      byte R1 = impedance_readData(REDATA_R1);
      byte R2 = impedance_readData(REDATA_R2);
      re = (R1 << 8) | R2;

      R1 = impedance_readData(IMGDATA_R1);
      R2 = impedance_readData(IMGDATA_R2);
      img = (R1 << 8) | R2;

      Serial.print("Real: "); Serial.print(re);
      Serial.print(" Imag: "); Serial.println(img);

      freq = startfreq + i * increfreq;
      mag = sqrt(pow(double(re), 2) + pow(double(img), 2));

      if (freq / 1000 == 50) {
        impedance_50khz = gain_factor_50khz * mag;
      }

      if (freq / 1000 == 100) {
        impedance_100khz = gain_factor_100khz * mag;
      }

      if ((impedance_readData(STATUSREG) & 0x07) < 4) {
        impedance_writeData(CTRLREG, (impedance_readData(CTRLREG) & 0x07) | 0x30);
        i++;
      }
    }
  }

  impedance_writeData(CTRLREG, (impedance_readData(CTRLREG) & 0x07) | 0xA0); // Power down

  return SENDING_RESULTS;
}

void impedance_writeData(int addr, int data) {
  Wire.beginTransmission(SLAVEADDR);
  Wire.write(addr);
  Wire.write(data);
  Wire.endTransmission();
  delay(1);
}

int impedance_readData(int addr) {
  int data = -1;
  Wire.beginTransmission(SLAVEADDR);
  Wire.write(ADDRPTR);
  Wire.write(addr);
  Wire.endTransmission();
  delay(1);
  Wire.requestFrom(SLAVEADDR, 1);

  if (Wire.available() >= 1) {
    data = Wire.read();
  }

  Serial.print("Data Addr: "); Serial.print(addr);
  Serial.print(" Data: "); Serial.println(data);
  delay(1);
  return data;
}

byte impedance_getFrequency(float freq, int n) {
  long val = long((freq / (MCLK / 4)) * pow(2, 27));
  byte code;

  switch (n) {
    case 1: code = (val & 0xFF0000) >> 0x10; break;
    case 2: code = (val & 0x00FF00) >> 0x08; break;
    case 3: code = (val & 0x0000FF); break;
    default: code = 0;
  }

  return code;
}
