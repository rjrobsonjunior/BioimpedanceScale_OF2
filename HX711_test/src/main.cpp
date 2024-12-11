#include <HX711.h>

#define PIN_DT 12
#define PIN_SCK 13
#define LED_PIN 2

void blinkLED(int time) {
  digitalWrite(LED_PIN, HIGH); 
  delay(time);            
  digitalWrite(LED_PIN, LOW);   
  delay(time);            
}

const int WAIT_TIME = 500; 

HX711 scale; 

float calibration = 25440; //tested 18/11

char cmd;

void setup ()
{
  Serial.begin(9600);
  scale.begin(PIN_DT, PIN_SCK);
  pinMode(LED_PIN, OUTPUT);
  scale.tare();
}

void loop ()
{
  scale.set_scale(calibration);
  blinkLED(100);
  if (scale.is_ready())
  {
    Serial.print("Weight: ");
    Serial.print(scale.get_units(), 1);
    Serial.print(" kg");
    Serial.print(" \t Calibration = ");
    Serial.print(calibration);
    Serial.println();

    // change calibration "n"(-1), "m"(+1), "v"(-10), "b"(+10), "x"(-100), "c"(+100)
    if(Serial.available())
    {
      cmd = Serial.read();
      switch (cmd)
      {
        case 'x':
          calibration -= 100;
          break;
        case 'c':
          calibration += 100;
          break;
        case 'v':
          calibration -= 10;
          break;
        case 'b':
          calibration += 10;
          break;
        case 'n':
          calibration -= 1;
          break;
        case 'm':
          calibration += 1;
          break;
      }
    }
  }
  else
  {
    Serial.print("HX-711 busy");
  }
  delay(WAIT_TIME);
}
