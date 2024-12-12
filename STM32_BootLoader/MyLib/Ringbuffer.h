#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>

typedef struct
{
	uint8_t *buffer;
	uint16_t head;
	uint16_t tail;
	uint16_t maxlen;
}RingBuff;
void ring_buff_init(RingBuff *ring_buff, uint8_t *buff, uint16_t len);
uint8_t ring_buff_push(RingBuff *ring_buff, uint8_t data);
uint8_t ring_buff_pop(RingBuff *ring_buff, uint8_t *data);
uint16_t ring_buff_available(RingBuff *ring_buff);

#endif