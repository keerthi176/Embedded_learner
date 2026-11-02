/*
    In ARM Cortex-M architecture, the bit-band alias region maps a 32-MB memory region 
    so that each individual bit in a 1-MB peripheral region can be written to or read atomically using standard 32-bit pointer dereferencing.
    The bit-band alias formula is: Alis_Addr = Alias_Base + (Byte_Offset * 32 ) + (Bit_Number * 4) 
    Given: Peripheral Bit-Band Base = 0x40000000 Peripheral Bit-Band Alias Base = 0x42000000 
    GPIO Output Data Register Address (GPIO_ODR) = 0x4001080C Write a macro or function SET_GPIO_BIT_ATOMIC(port_odr, bit_num) 
    that returns a volatile pointer to the bit-band alias address for bit_num (e.g., bit 5 of GPIO_ODR) 
    and sets it to 1 without using read-modify-write (|=) operations. What is the generated alias memory address for Bit 5 of GPIO_ODR ?
*/

#include "stdio.h"

#define SET_GPIO_BIT_ATOMIC(port_odr, bit_num) \
    (*(volatile uint32_t *)(0x42000000UL + \
    (((uint32_t)(port_odr) - 0x40000000UL) * 32UL) + \
    ((uint32_t)(bit_num) * 4UL))) = 1U


int main(void) {
    uint32_t gpio_odr_address = 0x4001080C; // GPIO Output Data Register Address
    uint8_t bit_number = 5; // Bit number to set

    // Set the specified bit atomically using the macro
    SET_GPIO_BIT_ATOMIC(gpio_odr_address, bit_number);

    // Calculate the generated alias memory address for Bit 5 of GPIO_ODR
    uint32_t alias_address = 0x42000000UL + ((gpio_odr_address - 0x40000000UL) * 32UL) + (bit_number * 4UL);
    
    printf("Alias memory address for Bit %d of GPIO_ODR: 0x%X\n", bit_number, alias_address);

    return 0;
}
