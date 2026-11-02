// Q 5. What does this type-punning pointer snippet output on a standard system where int is 4 bytes and short is 2 bytes (Little-Endian)?

#include "stdio.h"
#include "stdint.h"

typedef union {
 
    uint32_t word;
    uint16_t half[2];
    uint8_t bytes[4];

}Register;

int main(void)
{
    Register reg = {.word = 0x12345678};
    uint16_t *h_ptr = (uint16_t *)&reg.half[1];

    uint8_t *b_ptr = (uint8_t *)&h_ptr;

    *b_ptr = 0xAA;

    printf("reg.word = 0x%08X\n", reg.word);
    return 0;
}

// Answer: The program prints "reg.word = 0x1234AA78".