#include "SerialIRQHandler.h"
#include "pico/stdio.h"      
#include "hardware/uart.h"   
#include <cstdio>

Robot* SerialIRQHandler::robot_instance = nullptr;
char SerialIRQHandler::cmd_buffer[BUFFER_SIZE] = {0};
volatile int SerialIRQHandler::cmd_index = 0;

void SerialIRQHandler::Init(Robot* robot) {
    robot_instance = robot;
    cmd_index = 0;
    stdio_set_chars_available_callback(on_uart_rx, nullptr);
}

bool SerialIRQHandler::parse(const char* buf, int& a, int& b) {
    const char* p = buf;

    bool neg = false;
    if (*p == '-') { neg = true; p++; }
    if (*p < '0' || *p > '9') return false;

    int val = 0;
    while (*p >= '0' && *p <= '9') val = val * 10 + (*p++ - '0');
    a = neg ? -val : val;

    if (*p++ != ':') return false;

    neg = false;
    if (*p == '-') { neg = true; p++; }
    if (*p < '0' || *p > '9') return false;

    val = 0;
    while (*p >= '0' && *p <= '9') val = val * 10 + (*p++ - '0');
    b = neg ? -val : val;

    return (*p == '\0');
}

void SerialIRQHandler::on_uart_rx(void* param) {
    while (true) {
        int ch_raw = getchar_timeout_us(0);
        if (ch_raw == PICO_ERROR_TIMEOUT) {
            break;
        }
        
        char ch = static_cast<char>(ch_raw);
        uart_putc_raw(uart0, ch); 
        
        if (ch == '\n' || ch == '\r') {
            if (cmd_index > 0) {
                cmd_buffer[cmd_index] = '\0';
                parse_and_execute(); 
            }
            cmd_index = 0;
        } 
        else if (ch == '\b' || ch == 127) {
            if (cmd_index > 0) {
                cmd_index--;
                uart_putc_raw(uart0, ' ');
                uart_putc_raw(uart0, '\b');
            }
        }
        else if (cmd_index < BUFFER_SIZE - 1) {
            cmd_buffer[cmd_index++] = ch;
        }
        else {
            cmd_index = 0;
            uart_puts(uart0, "\n[Error] Overflow\n");
        }
    }
}

void SerialIRQHandler::parse_and_execute() {
    int pan = 0, tilt = 0;
    if (parse(cmd_buffer, pan, tilt)) {
        if (robot_instance) robot_instance->Move(pan, tilt);
    } else {
        uart_puts(uart0, "\n[Error] Invalid format\n");
    }
}