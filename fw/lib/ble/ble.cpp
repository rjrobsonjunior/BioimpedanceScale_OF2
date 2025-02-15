#include "ble.h"

BLEServer *pServer = nullptr;
BLECharacteristic *pCharacteristicTX = nullptr; // Para enviar dados (TX)
BLECharacteristic *pCharacteristicRX = nullptr; // Para receber dados (RX)
bool deviceConnected = false;
String lastReceivedData = "";  // Variável para armazenar a última mensagem recebida

// Classe para manipular conexões BLE
class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) override {
    deviceConnected = true;
  }
  void onDisconnect(BLEServer* pServer) override {
    deviceConnected = false;
  }
};

// Callback para receber mensagens via BLE
class MyCharacteristicCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) override {
    String receivedData = pCharacteristic->getValue().c_str();
    Serial.print("Mensagem recebida: ");
    Serial.println(receivedData);

    // Atualiza a variável global com a última mensagem recebida
    lastReceivedData = receivedData;
  }
};

// Função para inicializar o Bluetooth
void ble_setup() {
  BLEDevice::init("ESP32");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Característica para enviar dados (TX)
  pCharacteristicTX = pService->createCharacteristic(
                          CHARACTERISTIC_UUID_TX,
                          BLECharacteristic::PROPERTY_NOTIFY
                      );
  pCharacteristicTX->addDescriptor(new BLE2902());

  // Característica para receber dados (RX)
  pCharacteristicRX = pService->createCharacteristic(
                          CHARACTERISTIC_UUID_RX,
                          BLECharacteristic::PROPERTY_WRITE
                      );
  pCharacteristicRX->setCallbacks(new MyCharacteristicCallbacks());

  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->start();
}

// Função para enviar dados via Bluetooth
void ble_send(String data) {
  if (deviceConnected) {
    pCharacteristicTX->setValue(data.c_str());
    pCharacteristicTX->notify();
  }
}

// **Função para obter a última mensagem recebida**
String ble_get_last_received() {
  String temp = lastReceivedData;  // Copia o valor para evitar alterações concorrentes
  lastReceivedData = "";  // Limpa a variável após a leitura
  return temp;
}
