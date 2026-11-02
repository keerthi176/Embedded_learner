/*
    Consider a microcontroller GPIO Peripheral mapped at base address 0x40020000. The peripheral layout consists of:
        1. MODER (32-bit): Mode register (2 bits per pin, Pins 0-15). 
        2. OTYPER (16-bit): Output type register (1 bit per pin). Reserved 16 bits following. 
        3. ODR (16-bit): Output data register (1 bit per pin). Reserved 16 bits following. 
        
        Construct a C struct overlay matching this physical register map using bit-fields for MODER 
        such that pin0_mode occupies the lowest 2 bits. How can you set Pin 0 to Output Mode (0b01) 
        and toggle Pin 0 on ODR using only structure pointers?
*/

#include <stdint.h>
#include "stdio.h"

typedef struct {

    uint32_t pin0_mode : 2;
    uint32_t pin1_mode : 2;
    uint32_t pin2_mode : 2;
    uint32_t pin3_mode : 2;
    uint32_t pin4_mode : 2;
    uint32_t pin5_mode : 2;
    uint32_t pin6_mode : 2;
    uint32_t pin7_mode : 2;
    uint32_t pin8_mode : 2;
    uint32_t pin9_mode : 2;
    uint32_t pin10_mode : 2;
    uint32_t pin11_mode : 2;
    uint32_t pin12_mode : 2;
    uint32_t pin13_mode : 2;
    uint32_t pin14_mode : 2;
    uint32_t pin15_mode : 2;

} MODER_Bits_TypeDef;

typedef struct {

    MODER_Bits_TypeDef MODER;
    uint16_t OTYPER;
    uint16_t RESERVED1; 
    uint16_t ODR;
    uint16_t RESERVED2;

} MODER_TypeDef;

#define GPIO_BASE_ADDRESS 0x40020000
#define GPIO ((volatile MODER_TypeDef *)GPIO_BASE_ADDRESS)

int main(void) 
{

    // Set Pin 0 to Output Mode (0b01)
    GPIO->MODER.pin0_mode = 0b01;

    // Toggle Pin 0 on ODR
    GPIO->ODR ^= (1 << 0); // XOR with 1 at bit position 0 to toggle

    printf("Size of MODER_TypeDef: %zu bytes\n", sizeof(MODER_TypeDef));

    while(1) {
        // Main loop
    }

    return 0;
}