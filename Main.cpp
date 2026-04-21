#include "pico/stdlib.h"
#include "Robot.h"

int main(void)
{
    stdio_init_all();

    Robot Telescope;
    Telescope.Init();

    while (true) {
        __wfi();
    }

    return 0;
}