#include "pico/stdlib.h"
#include "Robot.h"
#include <stdio.h>
#include "hardware/sync.h"//voor_WFI
#include "SerialIRQHandler.h"

int main() {
    stdio_init_all();
    printf("hello world\n");

    Robot telescope;
    telescope.Init();

    SerialIRQHandler serial;
    serial.Init(&telescope);

    printf("Ready\n");
    while (true) {
        __wfi();
    }
}
