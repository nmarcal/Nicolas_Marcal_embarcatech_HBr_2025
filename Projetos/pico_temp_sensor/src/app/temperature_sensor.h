#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include <stdint.h>

// Função de conversão ADC para Celsius
float adc_to_celsius(uint16_t adc_val);

// Função para leitura do sensor interno
#ifdef PICO_H
#include "pico/stdlib.h"
#include "hardware/adc.h"
uint16_t read_internal_temp_adc();
#endif

#endif // TEMPERATURE_SENSOR_H