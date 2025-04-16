#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"

// Definições de pinos
#define JOYSTICK_X_PIN    26    // ADC0 (GPIO 26)
#define JOYSTICK_Y_PIN    27    // ADC1 (GPIO 27)
#define I2C_SDA           14    // GPIO 14 (SDA)
#define I2C_SCL           15    // GPIO 15 (SCL)

// Configurações do joystick
#define ADC_MAX           4095  // Valor máximo do ADC (12 bits)
#define DEAD_ZONE         100   // Zona morta em torno do centro
#define CENTER_X          (ADC_MAX / 2)  // Valor central teórico X
#define CENTER_Y          (ADC_MAX / 2)  // Valor central teórico Y

// Função para exibir duas mensagens no OLED
void display_two_messages(char *message1, int line1, char *message2, int line2) {
    struct render_area frame_area = {
        start_column: 0,
        end_column: ssd1306_width - 1,
        start_page: 0,
        end_page: ssd1306_n_pages - 1
    };
    calculate_render_area_buffer_length(&frame_area);

    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);  // Limpa o buffer

    // Desenha as mensagens
    ssd1306_draw_string(ssd, 5, line1 * 8, message1);  // Linha 1 (Y = line1 * 8 pixels)
    ssd1306_draw_string(ssd, 5, line2 * 8, message2);  // Linha 2 (Y = line2 * 8 pixels)

    render_on_display(ssd, &frame_area);  // Renderiza no display
}

// Função para ler o joystick com zona morta
void read_joystick(int16_t *x, int16_t *y) {
    // Ler valores brutos
    adc_select_input(0);  // Canal ADC0 (X)
    uint16_t raw_x = adc_read();
    adc_select_input(1);  // Canal ADC1 (Y)
    uint16_t raw_y = adc_read();
    
    // Aplicar zona morta para X
    if (abs(raw_x - CENTER_X) < DEAD_ZONE) {
        *x = 0;
    } else {
        *x = (int16_t)raw_x - CENTER_X;
    }
    
    // Aplicar zona morta para Y
    if (abs(raw_y - CENTER_Y) < DEAD_ZONE) {
        *y = 0;
    } else {
        *y = (int16_t)raw_y - CENTER_Y;
    }
}

int main() {
    stdio_init_all();  // Inicializa UART para o terminal
    adc_init();       // Inicializa o ADC
    adc_gpio_init(JOYSTICK_X_PIN);
    adc_gpio_init(JOYSTICK_Y_PIN);

    // Inicializa o display OLED (I2C)
    i2c_init(i2c1, 400 * 1000);  // Modo padrão do SSD1306
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init();

    int16_t x_value, y_value;
    char message_x[32];  // Buffer para o texto do eixo X
    char message_y[32];  // Buffer para o texto do eixo Y

    while (true) {
        read_joystick(&x_value, &y_value);

        // Formata as mensagens
        snprintf(message_x, sizeof(message_x), "X: %4d", x_value);
        snprintf(message_y, sizeof(message_y), "Y: %4d", y_value);

        // Exibe no OLED (X na linha 1, Y na linha 2)
        display_two_messages(message_x, 1, message_y, 3);  // Linhas 1 e 3 (para espaçamento)

        // Exibe no terminal (opcional)
        printf("X: %4d, Y: %4d\n", x_value, y_value);

        sleep_ms(100);  // Delay para evitar flicker
    }

    return 0;
}