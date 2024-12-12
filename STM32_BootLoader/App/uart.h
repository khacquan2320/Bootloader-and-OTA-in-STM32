#ifndef UART_H
#define UART_H

#include <stdint.h>


uint16_t uart_available(void);
uint8_t uart_read(void);
void uart_receive_rx(uint8_t data_rx);
void uart_init(void);



#endif
