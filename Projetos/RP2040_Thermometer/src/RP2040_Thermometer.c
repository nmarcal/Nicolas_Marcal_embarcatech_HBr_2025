#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"

#define ADC_TEMPERATURE_CHANNEL 4
#define I2C_SDA 14    // Pino SDA do I2C (GPIO 14)
#define I2C_SCL 15    // Pino SCL do I2C (GPIO 15)

// Função para converter ADC para temperatura (continua retornando float para precisão)
float adc_to_temperature_celsius(uint16_t adc_value) {
    const float conversion_factor = 3.3f / (1 << 12);
    float voltage = adc_value * conversion_factor;
    return 27.0f - (voltage - 0.706f) / 0.001721f;
}

// Função modificada para exibir apenas valores inteiros
void display_temperature(float temp_celsius) {
    char temp_str[20];
    int temp_int = (int)(temp_celsius + 0.5f);  // Arredonda para o inteiro mais próximo
    
    snprintf(temp_str, sizeof(temp_str), "Temp %d C", temp_int);  // %d para inteiro

    struct render_area frame_area = {
        start_column: 0,
        end_column: ssd1306_width - 1,
        start_page: 0,
        end_page: ssd1306_n_pages - 1
    };
    calculate_render_area_buffer_length(&frame_area);

    uint8_t buffer[ssd1306_buffer_length];
    memset(buffer, 0, ssd1306_buffer_length);
    
    ssd1306_draw_string(buffer, 5, 0, temp_str);
    render_on_display(buffer, &frame_area);
}

int main() {
    stdio_init_all();
    
    // Inicializa I2C para o display OLED
    i2c_init(i2c1, 400000);  // 400kHz
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    
    // Inicializa o display
    ssd1306_init();
   
    // Configura ADC para sensor de temperatura
    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(ADC_TEMPERATURE_CHANNEL);

    while (true) {
        float temp_c = adc_to_temperature_celsius(adc_read());
        
        // Exibe no terminal (opcional, mantém float para debug)
        printf("Temperatura: %.1f C\n", temp_c);
        
        // Exibe no display OLED (apenas inteiro)
        display_temperature(temp_c);
        
        sleep_ms(1000);
    }

    return 0;
}