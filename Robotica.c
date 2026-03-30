#include "pico/stdlib.h"

#define STEP_PIN 12   
#define DIR_PIN 13
#define BUTTON_LEFT 16
#define BUTTON_RIGHT 17

int main() {
    stdio_init_all();


    gpio_init(STEP_PIN);
    gpio_set_dir(STEP_PIN, GPIO_OUT);

    gpio_init(DIR_PIN);
    gpio_set_dir(DIR_PIN, GPIO_OUT);

    gpio_init(BUTTON_LEFT);
    gpio_set_dir(BUTTON_LEFT, GPIO_IN);
    gpio_pull_up(BUTTON_LEFT);

    gpio_init(BUTTON_RIGHT);
    gpio_set_dir(BUTTON_RIGHT, GPIO_IN);
    gpio_pull_up(BUTTON_RIGHT);  

    while (true) {


        if (gpio_get(BUTTON_LEFT) == 0) {
            gpio_put(DIR_PIN, 0);  // left direction
        }
        else if (gpio_get(BUTTON_RIGHT) == 0) {
            gpio_put(DIR_PIN, 1);  // right direction
        }
        
        gpio_put(STEP_PIN, 1);
        sleep_us(10);
        gpio_put(STEP_PIN, 0);
        sleep_us(500);
    }
}