#include "uart.h"
#include "Ringbuffer.h"

#define RING_UART_LEN 128 
uint8_t buff_uart[RING_UART_LEN];
RingBuff ring_buff;

void uart_receive_rx(uint8_t data_rx)
{
	ring_buff_push(&ring_buff, data_rx);
}

uint16_t uart_available()
{
	return ring_buff_available(&ring_buff);
}
uint8_t uart_read()
{
	uint8_t data;
	ring_buff_pop(&ring_buff,&data);
	return data;
}
void uart_init(void)
{
	ring_buff_init(&ring_buff, buff_uart, RING_UART_LEN);
}
