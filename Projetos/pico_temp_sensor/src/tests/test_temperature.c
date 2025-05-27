#include "unity/unity.h"
#include "../src/temperature_sensor.h"
#include <math.h>

void setUp(void) {
    // Inicializações necessárias antes de cada teste
}

void tearDown(void) {
    // Limpeza após cada teste
}

// Teste de conversão básica
void test_adc_conversion(void) {
    // Teste com valor conhecido (0.706V -> 27°C)
    uint16_t adc_val = (uint16_t)(0.706f * 4095 / 3.3f); // ~875
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 27.0f, adc_to_celsius(adc_val));
}

// Teste de valores extremos
void test_adc_extremes(void) {
    // Para ADC = 0 (0V)
    float expected = 27.0f - ((0.0f - 0.706f) / 0.001721f); // ~437.23°C
    TEST_ASSERT_FLOAT_WITHIN(1.0f, expected, adc_to_celsius(0));
    
    // Para ADC = 4095 (3.3V)
    expected = 27.0f - ((3.3f - 0.706f) / 0.001721f); // ~-147.7°C
    TEST_ASSERT_FLOAT_WITHIN(1.0f, expected, adc_to_celsius(4095));
}

// Teste de valores intermediários
void test_adc_intermediate(void) {
    // Valor correspondente a 1.0V
    uint16_t adc_val = (uint16_t)(1.0f * 4095 / 3.3f);
    float expected_temp = 27.0f - ((1.0f - 0.706f) / 0.001721f);
    TEST_ASSERT_FLOAT_WITHIN(0.5f, expected_temp, adc_to_celsius(adc_val));
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_adc_conversion);
    RUN_TEST(test_adc_extremes);
    RUN_TEST(test_adc_intermediate);
    
    return UNITY_END();
}