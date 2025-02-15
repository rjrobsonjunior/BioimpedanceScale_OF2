#include <HX711.h>
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID        "12345678-1234-1234-1234-123456789abc"
#define CHARACTERISTIC_UUID "87654321-4321-4321-4321-abc123456789"

BLEServer *pServer = nullptr;
BLECharacteristic *pCharacteristic = nullptr;
bool deviceConnected = false;

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) { deviceConnected = true; }
  void onDisconnect(BLEServer* pServer) { deviceConnected = false; }
};
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
  Serial.begin(115200);
  scale.begin(PIN_DT, PIN_SCK);
  pinMode(LED_PIN, OUTPUT);
  scale.tare();

  BLEDevice::init("ESP32");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
  
  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->start();

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
    if (deviceConnected) {
      String data = "Peso: " + String(scale.get_units(), 1) + " kg";
      pCharacteristic->setValue(data.c_str());
      pCharacteristic->notify();
      delay(2000);
  }

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

