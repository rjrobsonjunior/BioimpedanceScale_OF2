#include <BluetoothSerial.h>
#include <Wire.h>
#include "HX711.h"

// Objeto Bluetooth para comunicação
BluetoothSerial SerialBT;

// Configuração do I2C
const int I2C_SDA_PIN = 21; // Pino SDA
const int I2C_SCL_PIN = 22; // Pino SCL
const int I2C_ADDRESS = 0x3C; // Endereço do dispositivo I2C (ajuste conforme necessário)

// Configuração do HX711
const int HX711_DOUT_PIN = 13; // Pino de dados (DT) do HX711
const int HX711_SCK_PIN = 12; // Pino de clock (SCK) do HX711
HX711 scale;

void setup() {
  // Configuração da serial para monitoramento via USB
  Serial.begin(115200);
  Serial.println("Iniciando ESP32 com Bluetooth, I2C e HX711...");

  // Inicialização do Bluetooth
  if (!SerialBT.begin("ESP32_BT")) { // Nome do dispositivo Bluetooth
    Serial.println("Erro ao inicializar o Bluetooth!");
    while (1) {
      // Loop infinito em caso de erro na inicialização do Bluetooth
    }
  }
  Serial.println("Bluetooth inicializado. Conecte-se ao ESP32_BT.");

  // Inicialização do I2C
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN); // Configura pinos SDA e SCL
  Serial.println("I2C inicializado.");

  // Inicialização do HX711
  scale.begin(HX711_DOUT_PIN, HX711_SCK_PIN);
  if (!scale.is_ready()) {
    Serial.println("HX711 não está pronto. Verifique a conexão.");
    while (1) {
      // Loop infinito em caso de erro na inicialização do HX711
    }
  }
  Serial.println("HX711 inicializado.");
  scale.set_scale(2280.f); // Defina o fator de escala (ajuste para calibrar)
  scale.tare(); // Zera a balança
}

void loop() {
  // Recebe dados via Bluetooth
  if (SerialBT.available()) {
    String receivedData = SerialBT.readString();
    Serial.println("Recebido via Bluetooth: " + receivedData);

    // Envia dados via I2C para um dispositivo escravo
    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(receivedData.c_str());
    Wire.endTransmission();
  }

  // Recebe dados do dispositivo I2C
  Wire.requestFrom(I2C_ADDRESS, 8);
  while (Wire.available()) {
    char c = Wire.read();
    Serial.print(c);
    SerialBT.print(c);
  }

  // Leitura do peso usando o HX711
  if (scale.is_ready()) {
    float weight = scale.get_units(5); // Lê o peso em unidades configuradas
    Serial.print("Peso: ");
    Serial.print(weight);
    Serial.println(" g");

    // Envia o peso lido via Bluetooth
    SerialBT.print("Peso: ");
    SerialBT.print(weight);
    SerialBT.println(" g");
  } else {
    Serial.println("HX711 não está pronto para leitura.");
  }

  delay(1000); // Delay para evitar sobrecarga de leitura
}
