#include <Wire.h>
#include <stdint.h>
#include "../lib/weight/weight.h"
#include "../lib/ble/ble.h"

/*
void setup() {
    Serial.begin(9600);
    hx711_setup();
}

void loop() {
    Serial.print("Peso: ");
    Serial.print(hx711_get_weight());
    Serial.println(" g");
    delay(1000);
}
*/
void setup() {
    // Inicializa a comunicação serial para monitoramento
    Serial.begin(9600);

    // Configura o BLE (inicializa o BLE, cria o servidor, etc.)
    ble_setup();
}

void loop() {
    // Testa a conexão BLE e envia dados
    if (deviceConnected) {
        // Envia uma string de exemplo via BLE
        String data = "Mensagem via BLE!";
        ble_send(data);  // Função para enviar dados via BLE
        Serial.println("Mensagem enviada via BLE: " + data);
    }

    delay(1000);  // Atraso de 1 segundo
}