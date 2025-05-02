// hal/hal_led.c
#include "../include/hal_led.h"
#include "../include/led_embutido.h"


void hal_led_toggle() {
    static bool ligado = false;
    ligado = !ligado;
    led_embutido_set(ligado ? 1 : 0);
}
