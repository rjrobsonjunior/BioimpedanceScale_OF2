#ifndef WEIGHT_H
#define WEIGHT_H

#include <Arduino.h>
#include <HX711.h>

// Definição dos pinos do HX711
#define PIN_DT 12
#define PIN_SCK 13
//#define CALIBRATION 25440.0
#define CALIBRATION 19440.0

// Declaração da variável do HX711
extern HX711 scale;

// Funções para configuração e leitura do HX711
void hx711_setup();
float hx711_get_weight();

#endif 
