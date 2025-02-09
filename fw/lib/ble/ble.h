#ifndef BLE_H
#define BLE_H

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <Arduino.h>

#define SERVICE_UUID        "12345678-1234-1234-1234-123456789abc"
#define CHARACTERISTIC_UUID "87654321-4321-4321-4321-abc123456789"

extern BLEServer *pServer;
extern BLECharacteristic *pCharacteristic;
extern bool deviceConnected;

void ble_setup();
void ble_send(String data);

#endif