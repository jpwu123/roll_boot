#pragma once

#include <stdint.h>

void uart_rx_init(void);
void uart_rx_byte_process(uint8_t data);
