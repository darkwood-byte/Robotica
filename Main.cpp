#include "pico/stdlib.h"
#include "Sensor.h"
#include "Motor.h"
#include "error.h"
#include <stdio.h>

int main(void)
{
    stdio_init_all();

    Init_status_leds();

    Push_working();
    sleep_ms(2000);
    Push_finished();
    sleep_ms(1000);
    Push_incomming();

    Sensor S1;
    S1.AOF = 10;
    S1.AON = 11;
    S1.Check();
    Sensor S2;
    S2.AOF = 12;
    S2.AON = 13;
    S2.Check();
    gpio_init(4);
    gpio_init(5);

    gpio_set_dir(4, GPIO_OUT);
    gpio_set_dir(5, GPIO_OUT);

    gpio_put(4, 0);
    gpio_put(5, 0);
    // Blijf draaien zodat serial blijft werken
    while (true) {
        printf("%d ## %d\n", S1.Check(), S2.Check());
        /*gpio_put(4, 0);
        sleep_us(10);
        gpio_put(4, 1);
        sleep_us(10);*/
        if(S1.Check() == 1)gpio_put(5, 1);
         if(S2.Check() == 1)gpio_put(5, 0);
    }

    return 0;
}