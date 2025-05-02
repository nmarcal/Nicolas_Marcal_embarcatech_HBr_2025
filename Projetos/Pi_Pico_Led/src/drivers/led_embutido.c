// drivers/led_embutido.c
#include "../include/led_embutido.h"


void led_embutido_init() {
    cyw43_arch_init();
}

void led_embutido_set(int estado) {
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, estado);
}
