#include "pico/stdlib.h"
#include "Sensor.h"
#include "Motor.h"
#include "error.h"
int main(void)
{
    stdio_init_all();
    gpio_init(19);
    gpio_init(20);
    gpio_init(21);
    gpio_init(22);
     gpio_set_dir(19, GPIO_OUT);
    gpio_set_dir(20, GPIO_OUT);
    gpio_set_dir(21, GPIO_OUT);
    gpio_set_dir(22, GPIO_OUT);

    Push_error();
    Sensor S;
    S.AOF = 7;
    S.AON = 6;
    S.Check();
    Motor M;
    M.Init(1, 2, S, S);
    return 0;
}