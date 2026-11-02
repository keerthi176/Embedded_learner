/*
    Consider an SPI driver managing a hardware FIFO using a structure-mapped Ring Buffer:
    typedef struct {
    uint8_t *buffer;
    uint16_t head;
    uint16_t tail;
    uint16_t capacity;
    } SPI_RingBuffer_t; Write a function spi_write_bytes(SPI_RingBuffer_t *ring, const uint8_t *src, uint16_t len) 
     that uses pointer arithmetic to copy len bytes from src into the ring buffer, correctly handling wraparound.

*/

#include <stdint.h>
#include <stddef.h>

typedef struct
{
    uint8_t *buffer;
    uint16_t head;
    uint16_t tail;
    uint16_t capacity;
} SPI_RingBuffer_t;

int spi_write_bytes(SPI_RingBuffer_t *ring,
                    const uint8_t *src,
                    uint16_t len)
{
    uint16_t i;
    uint16_t next_head;

    if ((ring == NULL) || (src == NULL) ||
        (ring->buffer == NULL) || (ring->capacity < 2U))
    {
        return -1;
    }

    for (i = 0U; i < len; i++)
    {
        next_head = (uint16_t)(ring->head + 1U);

        if (next_head >= ring->capacity)
        {
            next_head = 0U;
        }

        /* Full: next head would collide with tail */
        if (next_head == ring->tail)
        {
            return (int)i;  /* Number of bytes successfully written */
        }

        /* Copy using pointer arithmetic */
        *(ring->buffer + ring->head) = *(src + i);

        ring->head = next_head;
    }

    return (int)len;
}


