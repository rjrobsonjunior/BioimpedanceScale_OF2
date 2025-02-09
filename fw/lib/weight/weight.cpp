#include "weight.h"

HX711 scale;
void hx711_setup() {
    scale.begin(PIN_DT, PIN_SCK);
    
    if (!scale.is_ready()) {
        Serial.println("HX711 is not ready");
        while (1); // Loop infinito em caso de erro
    }

    Serial.println("HX711 is ready");
    scale.set_scale(CALIBRATION); // Defina o fator de calibração
    scale.tare(); // Zera a balança
}

float hx711_get_weight() {
    if (scale.is_ready()) {
        return scale.get_units();
    } else {
        Serial.println("HX711 is not ready");
        return -1;
    }
}