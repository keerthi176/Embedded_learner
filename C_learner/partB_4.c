/*
    A DMA controller writes 16-bit ADC samples into a circular memory buffer of size 256 samples (uint16_t buffer[256]). 
    The DMA controller updates its hardware register DMA_NDTR (Number of Data to Transfer), which decrements from 256 down to 0 and auto-reloads back to 256. 
    Write a thread-safe function using pointers to calculate how many new samples are available for processing since the last check, 
    returning a pointer to the start of the unread batch.
*/

#include <stdint.h>
#include <stddef.h>

#define BUFFER_SIZE 256

static uint16_t buffer[BUFFER_SIZE];
static volatile uint16_t *dma_ndtr = (volatile uint16_t *)0x40020000; // Example address for DMA_NDTR register

static uint16_t read_index = 0;

uint16_t DMA_GetNewSamples(uint16_t *count) 
{
    uint16_t ndtr;
    uint16_t write_index;
    uint16_t available;
    uint16_t *start;

    ndtr = *dma_ndtr;

    write_index = (BUFFER_SIZE - ndtr) & (BUFFER_SIZE - 1U);


    if (write_index >= read_index) 
    {
        available = write_index - read_index;
        start = &buffer[read_index];
    } 
    else 
    {
        available = (BUFFER_SIZE - read_index) + write_index;
        start = &buffer[read_index];
    }


    read_index = write_index;
    *count = available;

    return start;