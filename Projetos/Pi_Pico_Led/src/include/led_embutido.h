// include/led_embutido.h
#ifndef LED_EMBUTIDO_H
#define LED_EMBUTIDO_H
#include "pico/cyw43_arch.h"

void led_embutido_init();
void led_embutido_set(int estado);

#endif // LED_EMBUTIDO_H
