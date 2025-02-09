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
    Serial.begin(9600); // Inicializa a comunicação serial
    hx711_setup(); // Configura o HX711
    ble_setup(); // Configura o BLE
}

void loop() {
  
    String received = ble_get_last_received();
    if (received != "") {
        Serial.print("Processando mensagem recebida: ");
        Serial.println(received);
        if (received == "ler peso") {
            Serial.println("Lendo peso...");
            float peso = 0.0;
            for (int i = 0; i < 10; i++) {
                peso += hx711_get_weight();
                delay(100);
            }
            peso /= 10;
            ble_send("Peso: " + String(peso) + " kg");
        }
    }

    delay(500);  // Pequeno delay para evitar sobrecarga da CPU
}
