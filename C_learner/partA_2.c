// Q 2. Assuming a 64-bit Little-Endian platform with 8-byte alignment rules, what does this program print?

#include"stdio.h"
#include"stdint.h"
#include "stddef.h"

struct RawData
{
    uint16_t header;
    uint32_t payload;
    uint8_t flag;

}__attribute__((packed));

struct HeaderView
{
    uint8_t b0;
    uint8_t b1;
};

int main(void)
{
    struct RawData data = {0x1234, 0xAABBCCDD, 0xEF};
    uint8_t *ptr = (uint8_t *)&data;

    uint32_t *payload_ptr = (uint32_t *)(ptr + offsetof(struct RawData, payload));
    struct HeaderView *h_ptr = (struct HeaderView *)(ptr + offsetof(struct RawData, header));

    printf("Payload: 0x%X, Header B0: 0x%X\n", *payload_ptr, h_ptr->b0);
    return 0;
}