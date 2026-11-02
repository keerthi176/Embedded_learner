/*
    A microcontroller receives an unaligned telemetry frame over UART into a raw byte buffer: 
    uint8_t rx_buf[16] = {0xAA, 0x01, 0x12, 0x34, 0x56, 0x78, 0x00, 0x00, ...};
    Bytes [1..4] represent a 32-bit timestamp (0x78563412 Little-Endian). 
    Dereferencing *(uint32_t *)&rx_buf[1] triggers a HardFault exception on strict architectures (e.g., ARM Cortex-M0). 
    Write a safe driver function using union and byte-pointer arithmetic to extract the 32-bit timestamp safely across all MCU architectures.
*/

#include <stdint.h>
#include <stddef.h>

typedef union
{
    uint32_t timestamp;
    uint8_t  bytes[4];
} TimestampData;

uint32_t UART_GetTimestamp(const uint8_t *rx_buf)
{
    TimestampData data;
    const uint8_t *p = rx_buf + 1;

    data.bytes[0] = *(p + 0);
    data.bytes[1] = *(p + 1);
    data.bytes[2] = *(p + 2);
    data.bytes[3] = *(p + 3);

    return ((uint32_t)data.bytes[0])       |
           ((uint32_t)data.bytes[1] << 8)  |
           ((uint32_t)data.bytes[2] << 16) |
           ((uint32_t)data.bytes[3] << 24);
}

int main(void)
{
    uint8_t rx_buf[16] = {0xAA, 0x01, 0x12, 0x34, 0x56, 0x78, 0x00, 0x00};

    uint32_t timestamp = UART_GetTimestamp(rx_buf);

    // The timestamp should be 0x78563412
    return (timestamp == 0x78563412) ? 0 : -1;
}   


