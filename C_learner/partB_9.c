/*
    An embedded device saves its configuration structure to internal Flash memory:

    typedef struct {
    uint32_t baud_rate;
    uint8_t node_id;
    uint8_t tx_power;
    uint16_t crc16;
    } __attribute__((packed)) SystemConfig_t; 

     Write a function that calculates a 16-bit XOR checksum over all fields of the configuration structure excluding the crc16 field itself, 
     using byte-level pointer iteration.

*/

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint32_t baud_rate;
    uint8_t  node_id;
    uint8_t  tx_power;
    uint16_t crc16;
} __attribute__((packed)) SystemConfig_t;

uint16_t CalculateConfigChecksum(const SystemConfig_t *config)
{
    const uint8_t *p;
    uint16_t checksum = 0U;
    size_t i;

    if (config == NULL)
    {
        return 0U;
    }

    p = (const uint8_t *)config;

    /* Exclude the final 2-byte crc16 field */
    for (i = 0U; i < offsetof(SystemConfig_t, crc16); i++)
    {
        checksum ^= (uint16_t)(*(p + i));
    }

    return checksum;
}

