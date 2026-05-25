#include "SerialIRQHandler.h"
#include "pico/stdio.h"      
#include "hardware/uart.h"   
#include <cstdio>

Robot* SerialIRQHandler::robot_instance = nullptr;
char SerialIRQHandler::cmd_buffer[BUFFER_SIZE] = {0};
int SerialIRQHandler::cmd_index = 0;

void SerialIRQHandler::Init(Robot* robot) {
    robot_instance = robot;
    cmd_index = 0;
    stdio_set_chars_available_callback(on_uart_rx, nullptr);
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
    int pan = 0;
    int tilt = 0;

    // sscanf is sneller, veiliger en korter binnen een ISR dan strchr + atoi
    if (sscanf(cmd_buffer, "%d:%d", &pan, &tilt) == 2) {
        if (robot_instance) {
            robot_instance->Move(pan, tilt);
        }
    } else {
        uart_puts(uart0, "\n[Error] Invalid format\n");
    }
}