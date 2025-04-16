
#include <stdio.h>          // Para funções de entrada/saída
#include <string.h>         // Para manipulação de strings 
#include "pico/stdlib.h"    // Biblioteca padrão do Raspberry Pi Pico
#include "hardware/adc.h"   // Para acesso ao conversor analógico-digital (ADC)
#include "hardware/i2c.h"   // Para comunicação I2C
#include "inc/ssd1306.h"    // Para controle do display OLED SSD1306

// Definições de pinos
#define JOYSTICK_X_PIN    26    // Pino GPIO 26 (ADC0) para eixo X do joystick
#define JOYSTICK_Y_PIN    27    // Pino GPIO 27 (ADC1) para eixo Y do joystick
#define I2C_SDA           14    // Pino GPIO 14 para dados I2C (SDA)
#define I2C_SCL           15    // Pino GPIO 15 para clock I2C (SCL)

// Configurações do joystick
#define ADC_MAX           4095  // Valor máximo do ADC (12 bits)
#define DEAD_ZONE         100   // Margem em torno do centro considerada como repouso
#define CENTER_X          (ADC_MAX / 2)  // Valor teórico do centro no eixo X
#define CENTER_Y          (ADC_MAX / 2)  // Valor teórico do centro no eixo Y

// Função para exibir duas mensagens no OLED
void display_two_messages(char *message1, int line1, char *message2, int line2) {
    // Define a área de renderização como toda a tela
    struct render_area frame_area = {
        start_column: 0,                     // Começa na coluna 0 (esquerda)
        end_column: ssd1306_width - 1,       // Termina na última coluna
        start_page: 0,                       // Começa na primeira página
        end_page: ssd1306_n_pages - 1        // Termina na última página
    };
    
    // Calcula o tamanho necessário para o buffer de renderização
    calculate_render_area_buffer_length(&frame_area);

    // Cria e limpa o buffer de pixels do display
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);  // Preenche o buffer com zeros

    // Desenha as strings no buffer:
    // Primeira mensagem na posição (5, line1*8)
    ssd1306_draw_string(ssd, 5, line1 * 8, message1);
    // Segunda mensagem na posição (5, line2*8)
    ssd1306_draw_string(ssd, 5, line2 * 8, message2);

    // Envia o buffer para o display OLED
    render_on_display(ssd, &frame_area);
}

// Função para ler o joystick com tratamento de zona morta
void read_joystick(int16_t *x, int16_t *y) {
    // Lê o valor bruto do eixo X
    adc_select_input(0);          // Seleciona canal ADC0 (X)
    uint16_t raw_x = adc_read();  // Faz a leitura do ADC
    
    // Lê o valor bruto do eixo Y
    adc_select_input(1);          // Seleciona canal ADC1 (Y)
    uint16_t raw_y = adc_read();  // Faz a leitura do ADC
    
    // Aplica zona morta para o eixo X:
    // Se o valor estiver dentro da margem do centro, considera como zero
    if (abs(raw_x - CENTER_X) < DEAD_ZONE) {
        *x = 0;  // Valor dentro da zona morta
    } else {
        *x = (int16_t)raw_x - CENTER_X;  // Valor fora da zona morta
    }
    
    // Aplica zona morta para o eixo Y:
    // Se o valor estiver dentro da margem do centro, considera como zero
    if (abs(raw_y - CENTER_Y) < DEAD_ZONE) {
        *y = 0;  // Valor dentro da zona morta
    } else {
        *y = (int16_t)raw_y - CENTER_Y;  // Valor fora da zona morta
    }
}

// Função principal
int main() {
    // Inicializa a comunicação serial (para debug)
    stdio_init_all();
    
    // Inicializa o hardware ADC
    adc_init();
    // Configura os pinos do joystick como entradas ADC
    adc_gpio_init(JOYSTICK_X_PIN);
    adc_gpio_init(JOYSTICK_Y_PIN);

    // Inicializa a interface I2C para o display OLED
    i2c_init(i2c1, 400 * 1000);  // Configura a 400kHz (padrão para SSD1306)
    // Configura os pinos I2C
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    // Habilita resistores de pull-up
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    
    // Inicializa o display OLED
    ssd1306_init();

    // Variáveis para armazenar os valores do joystick
    int16_t x_value, y_value;
    // Buffers para as mensagens a serem exibidas
    char message_x[32];
    char message_y[32];

    // Loop principal
    while (true) {
        // Lê os valores do joystick
        read_joystick(&x_value, &y_value);

        // Formata as mensagens com os valores lidos
        snprintf(message_x, sizeof(message_x), "X: %4d", x_value);
        snprintf(message_y, sizeof(message_y), "Y: %4d", y_value);

        // Exibe as mensagens no OLED (X na linha 1, Y na linha 3)
        display_two_messages(message_x, 1, message_y, 3);

        // Opcional: exibe os valores no terminal serial
        printf("X: %4d, Y: %4d\n", x_value, y_value);

        // Pequena pausa para evitar flicker no display
        sleep_ms(100);
    }

    return 0;  // Nunca será executado devido ao loop infinito
}
