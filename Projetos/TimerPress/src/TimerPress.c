#include <stdio.h>          // Para funções de entrada/saída
#include <string.h>         // Para manipulação de strings 
#include <stdlib.h>         // Para funções gerais
#include "pico/stdlib.h"   // Biblioteca padrão do Raspberry Pi Pico
#include "hardware/gpio.h"  // Para controle de GPIO
#include "hardware/timer.h" // Para funções de temporização
#include "hardware/i2c.h"   // Para comunicação I2C
#include "inc/ssd1306.h"   // Para controle do display OLED SSD1306

// Definições de pinos
#define BUTTON_A_PIN       5    // Pino GPIO 5 para botão A
#define BUTTON_B_PIN       6    // Pino GPIO 6 para botão B
#define I2C_SDA           14    // Pino GPIO 14 para dados I2C (SDA)
#define I2C_SCL           15    // Pino GPIO 15 para clock I2C (SCL)

// Variáveis globais
volatile int counter = 0;              // Contador regressivo
volatile int button_b_presses = 0;     // Contagem de pressionamentos do botão B
volatile bool counting_active = false; // Flag indicando se a contagem está ativa

// Estrutura para controle do botão com debounce
typedef struct {
    uint pin;                       // Número do pino GPIO
    volatile bool pressed;          // Flag indicando pressionamento atual
    volatile bool last_state;       // Estado anterior do botão
    volatile absolute_time_t last_press_time; // Tempo do último pressionamento
} Button;

// Inicialização das estruturas dos botões
Button button_a = {BUTTON_A_PIN, false, true, 0};
Button button_b = {BUTTON_B_PIN, false, true, 0};

// Tempo de debounce (50ms para evitar leituras múltiplas de um único pressionamento)
const uint64_t debounce_time_us = 50000;

// Função para exibir duas mensagens no display OLED
void display_two_messages(char *message1, int line1, char *message2, int line2) {
    // Configura área de renderização (toda a tela)
    struct render_area frame_area = {
        .start_column = 0,               // Começa na primeira coluna
        .end_column = ssd1306_width - 1, // Até a última coluna
        .start_page = 0,                 // Primeira página
        .end_page = ssd1306_n_pages - 1  // Última página
    };
    
    // Calcula tamanho do buffer necessário
    calculate_render_area_buffer_length(&frame_area);

    // Cria e limpa o buffer de exibição
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);

    // Desenha as strings no buffer
    ssd1306_draw_string(ssd, 5, line1 * 8, message1); // Linha 1 (posição Y = line1 * 8)
    ssd1306_draw_string(ssd, 5, line2 * 8, message2); // Linha 2 (posição Y = line2 * 8)
    
    // Envia o buffer para o display
    render_on_display(ssd, &frame_area);
}

// Função para verificar estado do botão com debounce
void check_button(Button *btn) {
    absolute_time_t current_time = get_absolute_time(); // Obtém tempo atual
    bool current_state = gpio_get(btn->pin);            // Lê estado atual do botão
    
    // Detecta borda de descida (transição HIGH->LOW - botão pressionado)
    if (!current_state && btn->last_state) {
        // Verifica se passou tempo suficiente desde o último pressionamento (debounce)
        if (absolute_time_diff_us(btn->last_press_time, current_time) > debounce_time_us) {
            btn->pressed = true;                  // Marca como pressionado
            btn->last_press_time = current_time;  // Atualiza tempo do último pressionamento
        }
    }
    btn->last_state = current_state; // Atualiza estado anterior
}

// Função principal
int main() {
    stdio_init_all(); // Inicializa comunicação serial (para debug)
    
    // Inicialização dos botões
    gpio_init(button_a.pin);           // Inicializa pino do botão A
    gpio_init(button_b.pin);           // Inicializa pino do botão B
    gpio_set_dir(button_a.pin, GPIO_IN);  // Configura como entrada
    gpio_set_dir(button_b.pin, GPIO_IN);  // Configura como entrada
    gpio_pull_up(button_a.pin);        // Habilita resistor de pull-up
    gpio_pull_up(button_b.pin);        // Habilita resistor de pull-up

    // Configuração do I2C para o display OLED
    i2c_init(i2c1, 400000);              // Inicializa I2C a 400kHz
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Configura pino SDA
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Configura pino SCL
    gpio_pull_up(I2C_SDA);               // Habilita pull-up no SDA
    gpio_pull_up(I2C_SCL);               // Habilita pull-up no SCL

    // Inicializa o display OLED
    ssd1306_init();
    // Exibe mensagem inicial
    display_two_messages("Pressione A", 2, "para iniciar", 3);

    // Variáveis para controle do tempo
    absolute_time_t last_time = get_absolute_time();
    const uint64_t one_second = 1000000; // 1 segundo em microssegundos

    // Loop principal
    while (1) {
        // Verifica estado dos botões
        check_button(&button_a);
        check_button(&button_b);

        // Trata pressionamento do botão A (inicia contagem)
        if (button_a.pressed) {
            button_a.pressed = false;    // Reseta flag
            counter = 9;                // Inicia contador em 9
            button_b_presses = 0;       // Zera contagem de pressionamentos B
            counting_active = true;     // Ativa contagem regressiva
            last_time = get_absolute_time(); // Reseta temporizador
        }

        // Trata pressionamento do botão B (durante contagem ativa)
        if (button_b.pressed && counting_active) {
            button_b.pressed = false;    // Reseta flag
            button_b_presses++;          // Incrementa contador de pressionamentos
        }

        // Lógica da contagem regressiva
        if (counting_active) {
            absolute_time_t current_time = get_absolute_time();
            // Verifica se passou 1 segundo
            if (absolute_time_diff_us(last_time, current_time) >= one_second) {
                last_time = current_time; // Atualiza tempo de referência
                if (counter > 0) {
                    counter--;           // Decrementa contador
                } else {
                    counting_active = false; // Desativa contagem ao chegar em 0
                }
            }
        }

        // Prepara strings para exibição
        char counter_str[20];
        char presses_str[20];
        
        // Lógica para exibição de diferentes mensagens conforme estado
        if (!counting_active && counter == 0 && button_b_presses > 0) {
            // Modo: contagem finalizada com pressionamentos registrados
            snprintf(counter_str, sizeof(counter_str), "Cont: 0");
            snprintf(presses_str, sizeof(presses_str), "Press: %d", button_b_presses);
        } else if (!counting_active && counter == 0) {
            // Modo: esperando início (tela inicial)
            snprintf(counter_str, sizeof(counter_str), "Pressione A");
            snprintf(presses_str, sizeof(presses_str), "para iniciar");
        } else {
            // Modo: contagem em andamento
            snprintf(counter_str, sizeof(counter_str), "Cont: %d", counter);
            snprintf(presses_str, sizeof(presses_str), "Press: %d", button_b_presses);
        }
        
        // Atualiza display
        display_two_messages(counter_str, 1, presses_str, 3);

        // Pequena pausa para reduzir carga da CPU
        sleep_ms(10);
    }

    return 0; // Nunca será alcançado devido ao loop infinito
}
