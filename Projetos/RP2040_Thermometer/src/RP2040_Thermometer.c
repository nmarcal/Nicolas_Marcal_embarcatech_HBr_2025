#include <stdio.h>          // Para funções de entrada/saída
#include <string.h>         // Para manipulação de strings
#include "pico/stdlib.h"    // Biblioteca padrão do Pico
#include "hardware/adc.h"   // Para acesso ao conversor analógico-digital
#include "hardware/i2c.h"   // Para comunicação I2C
#include "inc/ssd1306.h"    // Para controle do display OLED SSD1306

// Definições de constantes
#define ADC_TEMPERATURE_CHANNEL 4  // Canal ADC para o sensor de temperatura interno
#define I2C_SDA 14                 // Pino GPIO 14 para dados I2C (SDA)
#define I2C_SCL 15                 // Pino GPIO 15 para clock I2C (SCL)

// Função para converter valor ADC para temperatura em Celsius
float adc_to_temperature_celsius(uint16_t adc_value) {
    const float conversion_factor = 3.3f / (1 << 12);  // Fator de conversão para volts (3.3V, 12 bits)
    float voltage = adc_value * conversion_factor;     // Converte valor ADC para tensão
    
    // Fórmula de conversão específica para o sensor de temperatura do RP2040
    return 27.0f - (voltage - 0.706f) / 0.001721f;
}

// Função para exibir temperatura no display OLED (apenas valor inteiro)
void display_temperature(float temp_celsius) {
    char temp_str[20];  // Buffer para armazenar a string de temperatura
    
    // Converte float para inteiro arredondado
    int temp_int = (int)(temp_celsius + 0.5f);  // Adiciona 0.5 antes do cast para arredondar
    
    // Formata a string com o valor inteiro
    snprintf(temp_str, sizeof(temp_str), "Temp %d C", temp_int);

    // Configura área de renderização (toda a tela)
    struct render_area frame_area = {
        start_column: 0,                     // Começa na primeira coluna
        end_column: ssd1306_width - 1,       // Até a última coluna
        start_page: 0,                       // Primeira página
        end_page: ssd1306_n_pages - 1        // Última página
    };
    
    // Calcula tamanho do buffer necessário
    calculate_render_area_buffer_length(&frame_area);

    // Cria e limpa o buffer de exibição
    uint8_t buffer[ssd1306_buffer_length];
    memset(buffer, 0, ssd1306_buffer_length);
    
    // Desenha a string no buffer (posição X=5, Y=0)
    ssd1306_draw_string(buffer, 5, 0, temp_str);
    
    // Envia o buffer para o display
    render_on_display(buffer, &frame_area);
}

// Função principal
int main() {
    // Inicializa comunicação serial (para debug)
    stdio_init_all();
    
    // Configuração do I2C para o display OLED
    i2c_init(i2c1, 400000);  // Inicializa I2C a 400kHz
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);  // Configura pino SDA
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);  // Configura pino SCL
    gpio_pull_up(I2C_SDA);    // Habilita pull-up no SDA
    gpio_pull_up(I2C_SCL);    // Habilita pull-up no SCL
    
    // Inicializa o display OLED
    ssd1306_init();
   
    // Configuração do ADC para o sensor de temperatura
    adc_init();  // Inicializa o módulo ADC
    adc_set_temp_sensor_enabled(true);  // Habilita sensor de temperatura interno
    adc_select_input(ADC_TEMPERATURE_CHANNEL);  // Seleciona o canal do sensor

    // Loop principal
    while (true) {
        // Lê e converte a temperatura
        float temp_c = adc_to_temperature_celsius(adc_read());
        
        // Exibe no terminal (com uma casa decimal para debug)
        printf("Temperatura: %.1f C\n", temp_c);
        
        // Exibe no display OLED (apenas valor inteiro)
        display_temperature(temp_c);
        
        // Aguarda 1 segundo antes da próxima leitura
        sleep_ms(1000);
    }

    return 0;  // Nunca será alcançado devido ao loop infinito
}