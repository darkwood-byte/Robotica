#ifndef SERIAL_IRQ_HANDLER_H
#define SERIAL_IRQ_HANDLER_H

#include "pico/stdlib.h"
#include "Robot.h"

class SerialIRQHandler {
public:
    static void Init(Robot* robot);

private:
    static constexpr int BUFFER_SIZE = 32;

    static Robot* robot_instance;
    static char cmd_buffer[BUFFER_SIZE];
    static volatile int cmd_index;

    static void on_uart_rx(void* param);
    static void parse_and_execute();
    static bool parse(const char* buf, int& a, int& b);
};

#endif // SERIAL_IRQ_HANDLER_H