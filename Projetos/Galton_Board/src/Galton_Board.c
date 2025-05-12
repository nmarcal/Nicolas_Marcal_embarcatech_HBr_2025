#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"

// Definições dos pinos
#define I2C_SDA           14
#define I2C_SCL           15
#define BUTTON_B_PIN      6     // Pino do botão B (GPIO 6)

// Definições do display OLED
#define SCREEN_WIDTH      128
#define SCREEN_HEIGHT     64
#define BALL_SIZE         2
#define NUM_BINS          5     // 5 canaletas
#define BIN_WIDTH        (SCREEN_WIDTH / NUM_BINS)
#define MAX_BALLS         10
#define PIN_ROWS          5
#define PIN_RADIUS        2
#define PIN_SPACING      24
#define MAX_TOTAL_BALLS   100   // Limite de 100 bolas por rodada
#define NUM_CHUTES        5     // Número de canaletas
#define DEBOUNCE_DELAY_MS 200   // Tempo de debounce para o botão
#define CHUTE_HEIGHT      4     // Altura das canaletas

// Estruturas
typedef struct {
    float x, y;
    float vx, vy;
    bool active;
    int bin;
} Ball;

typedef struct {
    float x, y;
} Pin;

// Variáveis globais
Ball balls[MAX_BALLS];
Pin pins[PIN_ROWS * (PIN_ROWS + 1) / 2];
int bin_counts[NUM_BINS] = {0};
int total_balls = 0;
int balls_in_chutes = 0;
bool running = true;
bool game_active = true;
bool show_histogram = false;
bool button_b_pressed = false;
uint64_t last_button_time = 0;

// Protótipos de função
void init_pins();
void draw_pins(uint8_t *buffer);
void draw_chutes(uint8_t *buffer);
void init_balls();
void spawn_ball();
bool check_pin_collision(Ball *ball);
bool check_ball_collision(Ball *ball1, Ball *ball2);
void update_ball(Ball *ball);
void draw_ball(Ball *ball, uint8_t *buffer);
void draw_histogram(uint8_t *buffer);
void render_game_screen(uint8_t *buffer);
void render_histogram_screen(uint8_t *buffer);
void button_b_isr(uint gpio, uint32_t events);

void display_message(char *message, int line) {
    struct render_area frame_area = {
        start_column: 0,
        end_column: ssd1306_width - 1,
        start_page: 0,
        end_page: ssd1306_n_pages - 1
    };
    calculate_render_area_buffer_length(&frame_area);

    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    ssd1306_draw_string(ssd, 5, line * 8, message);
    render_on_display(ssd, &frame_area);
}

void ssd1306_draw_pixel(uint8_t *buffer, int x, int y, bool color) {
    if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) return;

    int byte_index = x + (y / 8) * SCREEN_WIDTH;
    uint8_t bit_mask = 1 << (y % 8);

    if (color)
        buffer[byte_index] |= bit_mask;
    else
        buffer[byte_index] &= ~bit_mask;
}

void init_pins() {
    int pin_index = 0;
    int start_y = 20;  // Pinos mais baixos

    for (int row = 0; row < PIN_ROWS; row++) {
        int pins_in_row = row + 1;
        int row_y = start_y + row * (SCREEN_HEIGHT - start_y - 10) / PIN_ROWS;
        
        int total_width = (pins_in_row - 1) * PIN_SPACING;
        int start_x = (SCREEN_WIDTH - total_width) / 2;
        
        for (int i = 0; i < pins_in_row; i++) {
            pins[pin_index].x = start_x + i * PIN_SPACING;
            pins[pin_index].y = row_y;
            pin_index++;
        }
    }
}

void draw_pins(uint8_t *buffer) {
    for (int i = 0; i < PIN_ROWS * (PIN_ROWS + 1) / 2; i++) {
        for (int dx = -PIN_RADIUS; dx <= PIN_RADIUS; dx++) {
            for (int dy = -PIN_RADIUS; dy <= PIN_RADIUS; dy++) {
                if (dx*dx + dy*dy <= PIN_RADIUS*PIN_RADIUS) {
                    int px = (int)pins[i].x + dx;
                    int py = (int)pins[i].y + dy;
                    ssd1306_draw_pixel(buffer, px, py, 1);
                }
            }
        }
    }
}

void draw_chutes(uint8_t *buffer) {
    int chute_width = SCREEN_WIDTH / NUM_CHUTES;
    int y_start = SCREEN_HEIGHT - CHUTE_HEIGHT;
    
    for (int i = 0; i < NUM_CHUTES; i++) {
        int x_start = i * chute_width;
        int x_end = (i + 1) * chute_width - 1;
        
        for (int y = y_start; y < SCREEN_HEIGHT; y++) {
            ssd1306_draw_pixel(buffer, x_start, y, 1);
            ssd1306_draw_pixel(buffer, x_end, y, 1);
        }
    }
}

void init_balls() {
    for (int i = 0; i < MAX_BALLS; i++) {
        balls[i].active = false;
        balls[i].bin = -1;
    }
}

bool check_ball_collision(Ball *ball1, Ball *ball2) {
    if (!ball1->active || !ball2->active) return false;
    if (ball1 == ball2) return false;
    
    float dx = ball1->x - ball2->x;
    float dy = ball1->y - ball2->y;
    float distance_sq = dx*dx + dy*dy;
    float min_distance = BALL_SIZE * 2;
    
    return distance_sq <= min_distance * min_distance;
}

void spawn_ball() {
    if (total_balls >= MAX_TOTAL_BALLS) {
        game_active = false;
        return;
    }

    for (int i = 0; i < MAX_BALLS; i++) {
        if (!balls[i].active) {
            // Verifica posição segura para spawn
            bool safe_position = false;
            int attempts = 0;
            
            while (!safe_position && attempts < 10) {
                balls[i].x = SCREEN_WIDTH / 2 + (rand() % 11 - 5);
                balls[i].y = 0;
                safe_position = true;
                
                // Verifica colisão com outras bolas
                for (int j = 0; j < MAX_BALLS; j++) {
                    if (balls[j].active) {
                        float dx = balls[i].x - balls[j].x;
                        float dy = balls[i].y - balls[j].y;
                        if (dx*dx + dy*dy < (BALL_SIZE*2)*(BALL_SIZE*2)) {
                            safe_position = false;
                            break;
                        }
                    }
                }
                attempts++;
            }
            
            balls[i].vx = (rand() % 100) / 100.0f * 0.5f - 0.25f;
            balls[i].vy = 0.5 + (rand() % 20) / 100.0f;
            balls[i].active = true;
            balls[i].bin = -1;
            total_balls++;
            break;
        }
    }
}

bool check_pin_collision(Ball *ball) {
    for (int i = 0; i < PIN_ROWS * (PIN_ROWS + 1) / 2; i++) {
        float dx = ball->x - pins[i].x;
        float dy = ball->y - pins[i].y;
        float distance_sq = dx*dx + dy*dy;

        if (distance_sq <= (BALL_SIZE + PIN_RADIUS) * (BALL_SIZE + PIN_RADIUS)) {
            float angle = (rand() % 314) / 100.0f;
            float speed = sqrt(ball->vx * ball->vx + ball->vy * ball->vy) * (0.8 + (rand() % 40)/100.0f);
            
            ball->vx = cos(angle) * speed;
            ball->vy = fabs(sin(angle)) * speed * 1.1f;
            
            ball->x += ball->vx * 0.5f;
            ball->y += ball->vy * 0.5f;
            
            return true;
        }
    }
    return false;
}

void update_ball(Ball *ball) {
    if (!ball->active) return;

    // Verifica colisões com outras bolas
    for (int i = 0; i < MAX_BALLS; i++) {
        if (&balls[i] != ball && check_ball_collision(ball, &balls[i])) {
            // Simples física de colisão elástica
            float temp_vx = ball->vx;
            float temp_vy = ball->vy;
            
            ball->vx = balls[i].vx * 0.9f;
            ball->vy = balls[i].vy * 0.9f;
            
            balls[i].vx = temp_vx * 0.9f;
            balls[i].vy = temp_vy * 0.9f;
            
            // Empurra as bolas para evitar que fiquem grudadas
            float dx = ball->x - balls[i].x;
            float dy = ball->y - balls[i].y;
            float distance = sqrt(dx*dx + dy*dy);
            float overlap = (BALL_SIZE * 2 - distance) / 2.0f;
            
            if (distance > 0) {
                ball->x += overlap * dx / distance;
                ball->y += overlap * dy / distance;
                balls[i].x -= overlap * dx / distance;
                balls[i].y -= overlap * dy / distance;
            }
        }
    }

    // Movimento original
    ball->x += ball->vx;
    ball->y += ball->vy;
    ball->vy += 0.02f;

    // Limita a velocidade máxima
    float speed = sqrt(ball->vx * ball->vx + ball->vy * ball->vy);
    if (speed > 2.0f) {
        ball->vx = ball->vx / speed * 2.0f;
        ball->vy = ball->vy / speed * 2.0f;
    }

    // Colisão com as paredes laterais
    if (ball->x <= BALL_SIZE || ball->x >= SCREEN_WIDTH - BALL_SIZE) {
        ball->vx *= -0.7f;
        ball->x = (ball->x <= BALL_SIZE) ? BALL_SIZE : SCREEN_WIDTH - BALL_SIZE;
    }

    // Colisão com pinos
    check_pin_collision(ball);

    // Verifica se a bola entrou em uma canaleta
    if (ball->y >= SCREEN_HEIGHT - BALL_SIZE - CHUTE_HEIGHT) {
        ball->active = false;
        
        int chute_width = SCREEN_WIDTH / NUM_CHUTES;
        int chute_index = (int)(ball->x / chute_width);
        
        if (chute_index >= 0 && chute_index < NUM_BINS) {
            bin_counts[chute_index]++;
            balls_in_chutes++;
        }
    }
}

void draw_ball(Ball *ball, uint8_t *buffer) {
    if (!ball->active) return;

    int x = (int)ball->x;
    int y = (int)ball->y;

    for (int i = -BALL_SIZE; i <= BALL_SIZE; i++) {
        for (int j = -BALL_SIZE; j <= BALL_SIZE; j++) {
            int px = x + i;
            int py = y + j;
            if (px >= 0 && px < SCREEN_WIDTH && py >= 0 && py < SCREEN_HEIGHT) {
                ssd1306_draw_pixel(buffer, px, py, 1);
            }
        }
    }
}

void draw_histogram(uint8_t *buffer) {
    int max_count = 0;
    for (int i = 0; i < NUM_BINS; i++) {
        if (bin_counts[i] > max_count) max_count = bin_counts[i];
    }

    if (max_count == 0) return;

    for (int i = 0; i < NUM_BINS; i++) {
        int bar_height = (int)((float)bin_counts[i] / max_count * (SCREEN_HEIGHT - 20));
        if (bar_height == 0 && bin_counts[i] > 0) bar_height = 1;
        
        int x_start = i * BIN_WIDTH + 2;
        int x_end = (i + 1) * BIN_WIDTH - 3;
        int y_start = SCREEN_HEIGHT - 10;
        int y_end = y_start - bar_height;
        
        for (int x = x_start; x <= x_end; x++) {
            for (int y = y_start; y >= y_end; y--) {
                ssd1306_draw_pixel(buffer, x, y, 1);
            }
        }
        
        char count_str[10];
        snprintf(count_str, sizeof(count_str), "%d", bin_counts[i]);
        ssd1306_draw_string(buffer, x_start + (BIN_WIDTH - strlen(count_str) * 6) / 2, SCREEN_HEIGHT - 8, count_str);
    }

    char title[20];
    snprintf(title, sizeof(title), "Resultados");
    ssd1306_draw_string(buffer, (SCREEN_WIDTH - strlen(title) * 6) / 2, 0, title);
}

void render_game_screen(uint8_t *buffer) {
    memset(buffer, 0, ssd1306_buffer_length);

    draw_pins(buffer);
    draw_chutes(buffer);

    for (int i = 0; i < MAX_BALLS; i++) {
        draw_ball(&balls[i], buffer);
    }

    char ball_count_str[30];
    snprintf(ball_count_str, sizeof(ball_count_str), "Bolas: %d", balls_in_chutes);
    ssd1306_draw_string(buffer, 5, 0, ball_count_str);

    // Mostra mensagem quando todas as bolas terminaram
    if (total_balls >= MAX_TOTAL_BALLS && balls_in_chutes == MAX_TOTAL_BALLS) {
        char msg[20] = "Completo!";
        ssd1306_draw_string(buffer, (SCREEN_WIDTH - strlen(msg) * 6) / 2, SCREEN_HEIGHT/2, msg);
    }
}

void render_histogram_screen(uint8_t *buffer) {
    memset(buffer, 0, ssd1306_buffer_length);
    draw_histogram(buffer);

    char total_str[30];
    snprintf(total_str, sizeof(total_str), "Total: %d bolas", balls_in_chutes);
    ssd1306_draw_string(buffer, (SCREEN_WIDTH - strlen(total_str) * 6) / 2, SCREEN_HEIGHT - 2, total_str);
}

void button_b_isr(uint gpio, uint32_t events) {
    uint64_t now = time_us_64() / 1000;
    if (gpio == BUTTON_B_PIN && (now - last_button_time > DEBOUNCE_DELAY_MS)) {
        button_b_pressed = true;
        last_button_time = now;
    }
}

int main() {
    stdio_init_all();

    // Inicializa o display
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init();

    // Configura o botão B com debounce
    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_B_PIN);
    gpio_set_irq_enabled_with_callback(BUTTON_B_PIN, GPIO_IRQ_EDGE_FALL, true, &button_b_isr);

    display_message("Galton Board", 3);
    sleep_ms(2000);

    srand(time_us_64());
    init_pins();
    init_balls();

    uint64_t last_ball_time = time_us_64();
    uint64_t ball_interval = 800000;

    while (running) {
        // Verifica botão B com debounce adicional
        if (button_b_pressed) {
            button_b_pressed = false;
            show_histogram = !show_histogram;
            sleep_ms(50);
        }
        
        if (!show_histogram) {
            uint64_t current_time = time_us_64();
            
            if (game_active) {
                for (int i = 0; i < MAX_BALLS; i++) {
                    update_ball(&balls[i]);
                }
                
                if (current_time - last_ball_time > ball_interval && total_balls < MAX_TOTAL_BALLS) {
                    spawn_ball();
                    last_ball_time = current_time;
                }
            }
        }
        
        struct render_area frame_area = {
            start_column: 0,
            end_column: ssd1306_width - 1,
            start_page: 0,
            end_page: ssd1306_n_pages - 1
        };
        calculate_render_area_buffer_length(&frame_area);
        
        uint8_t buffer[ssd1306_buffer_length];
        
        if (show_histogram) {
            render_histogram_screen(buffer);
        } else {
            render_game_screen(buffer);
        }
        
        render_on_display(buffer, &frame_area);
        sleep_ms(16);
    }

    return 0;
}