#include <Wire.h>
#include <stdint.h>
#include "../lib/weight/weight.h"
#include "../lib/ble/ble.h"
#include "../lib/impedance/impedance.h"


void setup() {
    Serial.begin(9600);
    hx711_setup(); 
    ble_setup(); 
    impedance_setup();  
}

void loop() {
    double impFinal = 0;
    String received = ble_get_last_received();
    if (received != "") {
        
        Serial.print("Processando mensagem recebida: ");
        Serial.println(received);
        if (received == "start") {
            Serial.println("Lendo peso...");
            float peso = 0.0;
            for (int i = 0; i < 10; i++) {
                peso += hx711_get_weight();
                delay(100);
            }
            peso /= 10;
            Serial.println("peso: " + String(peso));
            //ble_send("Peso: " + String(peso) + " kg");
            //--------------------------------------
            Serial.println("Lendo impedancia...");
            //for (int i = 0; i < 5; i++) {
                STATES resultado = impedance_runSweep();
                if (resultado == SENDING_RESULTS) {
                    impedance_100khz = 1 / (gain_factor_100khz * mag);
                    impFinal += impedance_100khz;
                    Serial.print("Impedancia em 100 kHz: ");
                    Serial.print(impedance_100khz);
                    Serial.println(" Ohms");
                    Serial.println("mag: ");
                    Serial.println(mag);
                    float FM;
                    float FFM;
                    float TBW;
                    int altura_cm = 183;
                    int peso_kg = 73;
                    int idade_anos = 22;
                    bool is_homem =  true;
                    TBW = 6.69 + (0.34573 * (pow(altura_cm, 2) / impedance_100khz)) + (0.17065 * peso_kg) - (0.11 * idade_anos) + (2.66 * is_homem);
                    FFM = TBW / 0.73;
                    FM = peso_kg - FFM;
                    FM = abs(FM);
                    FFM = abs(FFM);
                    TBW = abs(TBW);
                    Serial.print(" imp 100k: "); Serial.println(impedance_100khz);
                    Serial.print("TBW"); Serial.println(TBW);
                    Serial.print("FM:"); Serial.println(FM);
                    Serial.print("FFM"); Serial.println(FFM);
                }
                else {
                Serial.println("Erro na medicao de impedancia.");
                }
            //}
            //impFinal /= 5;
            Serial.println("impedancia final: " + String(impFinal));
            ble_send("{\"m\":\"" + String(peso) + "\", \"Z\":\""+ String(impFinal) + "\"}");
        }
    }
    Serial.println("FIM.");
    delay(500);
    }
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
/*
void setup() {
    Serial.begin(9600);
    impedance_setup();  // Inicializa o sensor de impedância
}

void loop() {
    Serial.println("Iniciando medição de impedância...");
    
    // Executa a varredura de impedância
    STATES resultado = impedance_runSweep();
    
    // Exibe os resultados se a medição foi bem-sucedida
    if (resultado == SENDING_RESULTS) {
        Serial.print("Impedância em 50 kHz: ");
        Serial.print(impedance_50khz);
        Serial.println(" Ohms");

        Serial.print("Impedância em 100 kHz: ");
        Serial.print(impedance_100khz);
        Serial.println(" Ohms");
    } else {
        Serial.println("Erro na medição de impedância.");
    }

    delay(1000);  // Aguarda 1 segundo antes da próxima medição
}
*/
/*
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

    delay(500);  
}
*/