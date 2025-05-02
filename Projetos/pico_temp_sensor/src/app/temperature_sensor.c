#include "temperature_sensor.h"

// Implementação da conversão
float adc_to_celsius(uint16_t adc_val) {
    const float v_ref = 3.3f;
    const float adc_max = 4095.0f;
    const float offset_voltage = 0.706f;
    const float temp_coefficient = 0.001721f;
    const float base_temp = 27.0f;
    
    float voltage = adc_val * (v_ref / adc_max);
    return base_temp - ((voltage - offset_voltage) / temp_coefficient);
}

// Implementação da leitura do ADC (somente para Pico)
#ifdef PICO_H
uint16_t read_internal_temp_adc() {
    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4); // Canal do sensor de temperatura
    return adc_read();
}
#endif