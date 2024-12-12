#include "Ringbuffer.h"
uint8_t ring_buff_push(RingBuff *ring_buff, uint8_t data)  // day data vao dau buffer 
{
    uint16_t next;

    next = ring_buff->head + 1;  // next is where head will point to after this write.
    if (next >= ring_buff->maxlen)
        next = 0;

    if (next == ring_buff->tail)  // if the head + 1 == tail, circular buffer is full
        return -1;

		ring_buff->buffer[ring_buff->head] = data;  // Load data and then move
    ring_buff->head = next;             // head to next data offset.
    return 0;  // return success to indicate successful push.
}

uint8_t ring_buff_pop(RingBuff *c, uint8_t *data) // lay data tu cuoi buffer
{
    uint16_t next;

    if (c->head == c->tail)  // if the head == tail, we don't have any data
        return -1;

    next = c->tail + 1;  // next is where tail will point to after this read.
    if(next >= c->maxlen)
        next = 0;

    *data = c->buffer[c->tail];  // Read data and then move
    c->tail = next;              // tail to next offset.
    return 0;  // return success to indicate successful push.
}

uint16_t ring_buff_available(RingBuff *ring_buff) // check xem co data trong buffer ko 
{
//	if(ring_buff->head < ring_buff->tail)
//	{
//		return ring_buff->maxlen -(ring_buff->tail -ring_buff->head);
//	}
	return (ring_buff->head == ring_buff->tail)? 0: 1;
}

void ring_buff_init(RingBuff *ring_buff, uint8_t *buff, uint16_t len) // khoi tao RingBuff 
{
	ring_buff->buffer = buff;
	ring_buff->head =0;
	ring_buff->tail =0;
	ring_buff->maxlen = len;
}