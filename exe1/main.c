#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
const int LED_PIN_R = 4;

volatile int flag_f_r = 0;
volatile int g_timer_0 = 0;
volatile bool timer_running = false;


void btn_callback(uint gpio, uint32_t events) {
    if (events == 0x4) { // fall edge
        flag_f_r = 1;
    } else if (events == 0x8) { // rise edge
    }
}

bool timer_0_callback(repeating_timer_t *rt) {
    g_timer_0 = 1;
    return true;    
}


int main() {
    stdio_init_all();

    repeating_timer_t timer_0;

    gpio_init(LED_PIN_R);
    gpio_set_dir(LED_PIN_R, GPIO_OUT);

    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);

    gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true,
                                       &btn_callback);

    while (true) {

        if (flag_f_r) {
            flag_f_r = 0;


            if (!timer_running){
                if (!add_repeating_timer_ms(500, timer_0_callback, NULL, &timer_0)) {
                    printf("Failed to add timer\n");
                } else {
                    timer_running = true;
                }
            }

            else{
                cancel_repeating_timer(&timer_0);
                gpio_put(LED_PIN_R, 0);
                timer_running = false;
            }    
        }

        if(g_timer_0){
            g_timer_0 = 0;
            gpio_xor_mask(1u << LED_PIN_R);        }
    }


}
