#include <Wire.h>
#include <Arduino.h>
#include <stdint.h>

// Register locations
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

typedef enum
{
  WAITING_START = 0,
  READING_USERDATA,
  MEASURING,
  SENDING_RESULTS
} STATES;


void programReg();
STATES runSweep();
void writeData(int addr, int data);
int readData(int addr);
byte getFrequency(float freq, int n);

const float MCLK = 16.776 * pow(10, 6); // AD5933 Internal Clock Speed 16.776 MHz
const float startfreq = 50 * pow(10, 3); // Frequency Start
const float increfreq = 1 * pow(10, 3); // Frequency Increment
const int increnum = 50; // Number of increments

//sem circuito auxiliar
//const double gain_factor_50khz  = 2.043 * pow(10, -7); // Gain Factor at 50 kHz | Known Impedance: ~1 kOhm (981 Ohm)
//const double gain_factor_100khz = 2.044 * pow(10, -7);  // Gain Factor at 100 kHz | Known Impedance: ~1 kOhm (981 Ohm)
//com circuito auxiliar
//const double gain_factor_50khz  = 6.277 * pow(10, -7); // Gain Factor at 50 kHz | Known Impedance: ~1 kOhm (981 Ohm)
//const double gain_factor_100khz = 6.275 * pow(10, -7);  // Gain Factor at 100 kHz | Known Impedance: ~1 kOhm (981 Ohm)
//com circuito auxiliar 500ohm
const double gain_factor_50khz  = 8.043 * pow(10, -7); // Gain Factor at 50 kHz | Known Impedance: ~0,5 kOhm (584,4 Ohm)
const double gain_factor_100khz = 1.33 * pow(10, -6);  // Gain Factor at 100 kHz | Known Impedance: ~0,5 kOhm (584,4 Ohm)


// Impedance values
double impedance_50khz;
double impedance_100khz;

STATES actual_state = MEASURING;

void setup() {
  Wire.begin();
  Serial.begin(9600);

  writeData(CTRLREG, 0x0); // Clear Control Register
  writeData(CTRLREG2, 0x10); // Reset / Internal clock
  programReg();
  Serial.println("I2C inicializado.");
}

void loop() {

  switch (actual_state) {
    case MEASURING:
      Serial.println("Start Sweep");
      actual_state = runSweep();
      break;

    case SENDING_RESULTS:

      actual_state = WAITING_START;
      break;

    default:
      actual_state = WAITING_START;
      break;
  }
}

void programReg() {
  // Set Range 1, PGA gain 1
  writeData(CTRLREG, 0x01);

  // Set settling cycles
  writeData(NUMSCYCLES_R1, 0x07);
  writeData(NUMSCYCLES_R2, 0xFF);

  // Start frequency of 50 kHz
  writeData(STARTFREQ_R1, getFrequency(startfreq, 1));
  writeData(STARTFREQ_R2, getFrequency(startfreq, 2));
  writeData(STARTFREQ_R3, getFrequency(startfreq, 3));

  // Increment by 1 kHz
  writeData(FREGINCRE_R1, getFrequency(increfreq, 1));
  writeData(FREGINCRE_R2, getFrequency(increfreq, 2));
  writeData(FREGINCRE_R3, getFrequency(increfreq, 3));

  // Points in frequency sweep (100), max 511
  writeData(NUMINCRE_R1, (increnum & 0x001F00) >> 0x08);
  writeData(NUMINCRE_R2, (increnum & 0x0000FF));
}

STATES runSweep() {
  int16_t re;
  int16_t img;
  float freq;
  double mag;
  int i = 0;

  impedance_50khz = 0;
  impedance_100khz = 0;

  programReg();

  // 1. Standby '10110000' Mask D8-10 of avoid tampering with gains
  writeData(CTRLREG, (readData(CTRLREG) & 0x07) | 0xB0);
  // 2. Initialize sweep
  writeData(CTRLREG, (readData(CTRLREG) & 0x07) | 0x10);
  // 3. Start sweep
  writeData(CTRLREG, (readData(CTRLREG) & 0x07) | 0x20);

  while ((readData(STATUSREG) & 0x07) < 4 ) { // Check that status reg != 4, sweep not complete
    delay(100); // Delay between measurements
    
    int flag = readData(STATUSREG) & 2;
    //Serial.print("Flag: "); Serial.println(flag);
    if (flag == 2) {
      byte R1 = readData(REDATA_R1);
      byte R2 = readData(REDATA_R2);
      re = (R1 << 8) | R2;
      R1 = readData(IMGDATA_R1);
      R2 = readData(IMGDATA_R2);
      img = (R1 << 8) | R2;
      // if(freq == 1000 || freq == 50000 || freq == 100000)
      // {
      //   Serial.print("Real: "); Serial.print(re);
      //   Serial.print(" Imag: "); Serial.println(img);
      // }

      freq = startfreq + i * increfreq;

      

      if (freq / 1000 == 50) { //50 kHz
        mag = sqrt(pow(double(re), 2) + pow(double(img), 2));
        impedance_50khz = 1/(gain_factor_50khz * mag);
        //Serial.print("freq: "); Serial.print(freq);
        //Serial.print("Real: "); Serial.print(re);
        //Serial.print(" Imag: "); Serial.println(img);
        //Serial.print(" Mag: "); Serial.println(mag);
        //Serial.print(" imp 50k "); Serial.println(impedance_50khz);
      }
      int altura_cm = 183;
      int peso_kg = 73;
      int idade_anos = 22;
      bool is_homem =  true;


      if (freq / 1000 == 100) { //100 kHz
        float FM;
        float FFM;
        float TBW;
        impedance_100khz = 1 / (gain_factor_100khz * mag);
        TBW = 6.69 + (0.34573 * (pow(altura_cm, 2) / impedance_100khz)) + (0.17065 * peso_kg) - (0.11 * idade_anos) + (2.66 * is_homem);
        FFM = TBW / 0.73;
        FM = peso_kg - FFM;

        FM = abs(FM);
        FFM = abs(FFM);
        TBW = abs(TBW);
        
        Serial.print("Freq: "); Serial.print(freq);
        Serial.print("Real: "); Serial.print(re);
        Serial.print(" Imag: "); Serial.println(img);
        Serial.print(" Mag: "); Serial.println(mag);
        Serial.print(" imp 100k: "); Serial.println(impedance_100khz);
        Serial.print("TBW"); Serial.println(TBW);
        Serial.print("FM:"); Serial.println(FM);
        Serial.print("FFM"); Serial.println(FFM);
      }

      // Increment frequency
      if ((readData(STATUSREG) & 0x07) < 4 ) {
        writeData(CTRLREG, (readData(CTRLREG) & 0x07) | 0x30);
        i++;
      }
    }
  }

  writeData(CTRLREG, (readData(CTRLREG) & 0x07) | 0xA0); // Power down

  if (impedance_100khz != 0) {
    //Serial.print(" imp 100k: "); Serial.println(impedance_100khz);
    //Serial.print(" imp 50k "); Serial.println(impedance_50khz);
    return MEASURING;
  }
  return MEASURING;
}

void writeData(int addr, int data) 
{
  Wire.beginTransmission(SLAVEADDR);
  Wire.write(addr);
  Wire.write(data);
  Wire.endTransmission();
  delay(1);
}

int readData(int addr) {
  int data;
  Wire.beginTransmission(SLAVEADDR);
  Wire.write(ADDRPTR);
  Wire.write(addr);
  Wire.endTransmission();
  delay(1);
  Wire.requestFrom(SLAVEADDR, 1);

  if (Wire.available() >= 1) {
    data = Wire.read();
  } else {
    data = -1;
  }
  //Serial.print("Data Addr: "); Serial.print(addr);
  //Serial.print("data: "); Serial.println(data);
  delay(1);
  return data;
}

byte getFrequency(float freq, int n) {
  long val = long((freq / (MCLK / 4)) * pow(2, 27));
  byte code;

  switch (n) {
    case 1:
      code = (val & 0xFF0000) >> 0x10;
      break;
    case 2:
      code = (val & 0x00FF00) >> 0x08;
      break;
    case 3:
      code = (val & 0x0000FF);
      break;
    default:
      code = 0;
  }

  return code;
}