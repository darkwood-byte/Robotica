#ifndef ERROR_H
#define ERROR_H

#include <stdbool.h>
#include "pico/stdlib.h"

// Init LEDs only once
#define Init_Status_Leds()              \
do {                                    \
    static bool initialized = false;    \
    if (!initialized) {                 \
        gpio_init(18);                  \
        gpio_init(19);                  \
        gpio_init(20);                  \
        gpio_init(21);                  \
        gpio_init(22);                  \
                                        \
        gpio_set_dir(18, GPIO_OUT);    \
        gpio_set_dir(19, GPIO_OUT);    \
        gpio_set_dir(20, GPIO_OUT);    \
        gpio_set_dir(21, GPIO_OUT);    \
        gpio_set_dir(22, GPIO_OUT);    \
                                        \
        initialized = true;            \
    }                                   \
} while (0)

#define Push_finished()               \
do {                                  \
    Init_Status_Leds();              \
    gpio_put(18, 1);                 \
    gpio_put(19, 0);                 \
    gpio_put(20, 0);                 \
    gpio_put(21, 0);                 \
} while (0)

#define Push_working()               \
do {                                  \
    Init_Status_Leds();              \
    gpio_put(18, 0);                 \
    gpio_put(19, 1);                 \
    gpio_put(20, 0);                 \
    gpio_put(21, 0);                 \
} while (0)

#define Push_error()                 \
do {                                  \
    Init_Status_Leds();              \
    gpio_put(18, 0);                 \
    gpio_put(19, 0);                 \
    gpio_put(20, 1);                 \
    gpio_put(21, 0);                 \
} while (0)

#define Push_incoming()              \
do {                                  \
    Init_Status_Leds();              \
    gpio_put(18, 0);                 \
    gpio_put(19, 0);                 \
    gpio_put(20, 0);                 \
    gpio_put(21, 1);                 \
} while (0)

#endif // ERROR_H