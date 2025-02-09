#ifndef BLE_H
#define BLE_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// Defina o UUID do serviço e da característica
#define SERVICE_UUID           "12345678-1234-1234-1234-1234567890ab"
#define CHARACTERISTIC_UUID_TX "23456789-1234-1234-1234-1234567890ab"
#define CHARACTERISTIC_UUID_RX "34567890-1234-1234-1234-1234567890ab"

// Variáveis globais para o BLE
extern BLEServer *pServer;
extern BLECharacteristic *pCharacteristicTX;
extern BLECharacteristic *pCharacteristicRX;
extern bool deviceConnected;

// Funções BLE
void ble_setup();
void ble_send(String data);
String ble_get_last_received();  // Nova função para obter a última mensagem recebida

#endif
