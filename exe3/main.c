#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
const int LED_PIN_R = 4;

volatile int button_pressed = 0;
volatile int button_released = 0;

volatile int timer_fired = 0;
alarm_id_t alarm_id;


int64_t alarm_callback(alarm_id_t id, void *user_data) {
    timer_fired = 1;

    return 0;
}

void btn_callback(uint gpio, uint32_t events) {
    if (events == 0x4) { // fall edge
        button_pressed = 1;
        timer_fired = 0;
        alarm_id = add_alarm_in_ms(300, alarm_callback, NULL, false);
        
    } else if (events == 0x8) { // rise edge
        button_released = 1;
    }
}


int main() {
    stdio_init_all();

    gpio_init(LED_PIN_R);
    gpio_set_dir(LED_PIN_R, GPIO_OUT);
    gpio_put(LED_PIN_R, 0);

    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);

    gpio_set_irq_enabled_with_callback(
        BTN_PIN_R, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &btn_callback);


    int led_state = 0;
    while (true) {

        if (button_released) {
            button_released = 0;
            
            if(!timer_fired){
                cancel_alarm(alarm_id);
            }

           else {
                timer_fired = 0;
                led_state = !led_state;
                gpio_put(LED_PIN_R, led_state);
                
            }
        }
    }
}
