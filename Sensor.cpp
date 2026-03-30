#include "pico/stdlib.h"
#include "Sensor.h"

int Sensor::Check() {
    int aon = gpio_get(AON);
    int aof = gpio_get(AOF);
    if (aon == aof) return -1;
    if (aon == 0) return 1;
    return 0;
}

void Sensor::init() {
    gpio_init(AON);
    gpio_set_dir(AON, GPIO_IN);

    gpio_init(AOF);
    gpio_set_dir(AOF, GPIO_IN);
}