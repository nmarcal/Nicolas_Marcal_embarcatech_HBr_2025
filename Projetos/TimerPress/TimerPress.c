#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"

#define BUTTON_A_PIN       5    // Pino do botão A (GPIO 5)
#define BUTTON_B_PIN       6    // Pino do botão B (GPIO 6)
#define I2C_SDA           14    // Pino SDA do I2C (GPIO 14)
#define I2C_SCL           15    // Pino SCL do I2C (GPIO 15)

// Variáveis globais
volatile int counter = 0;
volatile int button_b_presses = 0;
volatile bool counting_active = false;

// Estrutura para controle do botão
typedef struct {
    uint pin;
    volatile bool pressed;
    volatile bool last_state;
    volatile absolute_time_t last_press_time;
} Button;

Button button_a = {BUTTON_A_PIN, false, true, 0};
Button button_b = {BUTTON_B_PIN, false, true, 0};
const uint64_t debounce_time_us = 50000; // 50ms para debounce

void display_two_messages(char *message1, int line1, char *message2, int line2) {
    struct render_area frame_area = {
        .start_column = 0,
        .end_column = ssd1306_width - 1,
        .start_page = 0,
        .end_page = ssd1306_n_pages - 1
    };
    calculate_render_area_buffer_length(&frame_area);

    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);

    ssd1306_draw_string(ssd, 5, line1 * 8, message1);
    ssd1306_draw_string(ssd, 5, line2 * 8, message2);
    render_on_display(ssd, &frame_area);
}

void check_button(Button *btn) {
    absolute_time_t current_time = get_absolute_time();
    bool current_state = gpio_get(btn->pin);
    
    // Detecta borda de descida (botão pressionado)
    if (!current_state && btn->last_state) {
        if (absolute_time_diff_us(btn->last_press_time, current_time) > debounce_time_us) {
            btn->pressed = true;
            btn->last_press_time = current_time;
        }
    }
    btn->last_state = current_state;
}

int main() {
    stdio_init_all();
    
    // Inicialização dos botões
    gpio_init(button_a.pin);
    gpio_init(button_b.pin);
    gpio_set_dir(button_a.pin, GPIO_IN);
    gpio_set_dir(button_b.pin, GPIO_IN);
    gpio_pull_up(button_a.pin);
    gpio_pull_up(button_b.pin);

    // Configura o I2C para o display OLED
    i2c_init(i2c1, 400000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Inicializa o display OLED
    ssd1306_init();
    display_two_messages("Pressione A", 2, "para iniciar", 3);

    absolute_time_t last_time = get_absolute_time();
    const uint64_t one_second = 1000000;

    while (1) {
        // Verifica estado dos botões
        check_button(&button_a);
        check_button(&button_b);

        // Trata pressionamento do botão A
        if (button_a.pressed) {
            button_a.pressed = false;
            counter = 9;
            button_b_presses = 0;
            counting_active = true;
            last_time = get_absolute_time();
        }

        // Trata pressionamento do botão B
        if (button_b.pressed && counting_active) {
            button_b.pressed = false;
            button_b_presses++;
        }

        // Contagem regressiva
        if (counting_active) {
            absolute_time_t current_time = get_absolute_time();
            if (absolute_time_diff_us(last_time, current_time) >= one_second) {
                last_time = current_time;
                if (counter > 0) {
                    counter--;
                } else {
                    counting_active = false;
                }
            }
        }

        // Atualiza display
        char counter_str[20];
        char presses_str[20];
        
        if (!counting_active && counter == 0 && button_b_presses > 0) {
            snprintf(counter_str, sizeof(counter_str), "Cont: 0");
            snprintf(presses_str, sizeof(presses_str), "Press: %d", button_b_presses);
        } else if (!counting_active && counter == 0) {
            snprintf(counter_str, sizeof(counter_str), "Pressione A");
            snprintf(presses_str, sizeof(presses_str), "para iniciar");
        } else {
            snprintf(counter_str, sizeof(counter_str), "Cont: %d", counter);
            snprintf(presses_str, sizeof(presses_str), "Press: %d", button_b_presses);
        }
        
        display_two_messages(counter_str, 1, presses_str, 3);

        sleep_ms(10);
    }

    return 0;
}