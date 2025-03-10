#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
const int BTN_PIN_G = 26;

const int LED_PIN_R = 4;
const int LED_PIN_G = 6;

volatile int flag_r = 0;
volatile int flag_g = 0;

volatile int g_timer_0_g = 0;
volatile int g_timer_0_r = 0;

void btn_callback(uint gpio, uint32_t events) {
    if (events == 0x4) {
        if (gpio == BTN_PIN_R)
            flag_r = 1;
        else if (gpio == BTN_PIN_G)
            flag_g = 1;
    }
}

bool timer_0_callback(repeating_timer_t *rt) {
    g_timer_0_r = 1;
    return true;    
}

bool timer_1_callback(repeating_timer_t *rt) {
    g_timer_0_g = 1;
    return true;    
}

int main() {
    stdio_init_all();

    repeating_timer_t timer_0;
    bool timer_0_running = false;

    repeating_timer_t timer_1;
    bool timer_1_running = false;

    gpio_init(LED_PIN_R);
    gpio_set_dir(LED_PIN_R, GPIO_OUT);

    gpio_init(LED_PIN_G);
    gpio_set_dir(LED_PIN_G, GPIO_OUT);

    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);

    gpio_init(BTN_PIN_G);
    gpio_set_dir(BTN_PIN_G, GPIO_IN);
    gpio_pull_up(BTN_PIN_G);

    gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true,
                                       &btn_callback);
    gpio_set_irq_enabled(BTN_PIN_G, GPIO_IRQ_EDGE_FALL, true);

    while (true) {

        if (flag_r) {
            flag_r = 0;

            if (!timer_0_running){
                if (!add_repeating_timer_ms(500, timer_0_callback, NULL, &timer_0)) {
                    printf("Failed to add timer\n");
                } else {
                    timer_0_running = true;
                }
            }

            else{
                cancel_repeating_timer(&timer_0);
                gpio_put(LED_PIN_R, 0);
                timer_0_running = false;
            }
        }

        if (flag_g) {
            flag_g = 0;

            if (!timer_1_running){
                if (!add_repeating_timer_ms(250, timer_1_callback, NULL, &timer_1)) {
                    printf("Failed to add timer\n");
                } else {
                    timer_1_running = true;
                }
            }

            else{
                cancel_repeating_timer(&timer_1);
                gpio_put(LED_PIN_G, 0);
                timer_0_running = false;
            }
        }

        if (g_timer_0_g){
            g_timer_0_g = 0;
            gpio_xor_mask(1u << LED_PIN_G);
        }
        if (g_timer_0_r){
            g_timer_0_r = 0;
            gpio_xor_mask(1u << LED_PIN_R);
        }
    }
}
