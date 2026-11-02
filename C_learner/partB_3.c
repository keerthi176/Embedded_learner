/*

    A system clock register RCC_PLLCFGR at offset 0x0C from base 0x40023800 is structured as: Bits [5:0]: PLLM division factor (6 bits)
    Bits [14:6]: PLLN multiplication factor (9 bits) Bits [17:16]: PLLP division factor (2 bits) Bit [22]: PLLSRC clock source select (1 bit: 0 = HSI, 1 = HSE) 
    Using a union containing a raw 32-bit integer and a bit-field structure, 
    write code to configure the PLL for PLLM = 8, PLLN = 336, PLLP = 2 (encoded as 0b00), and PLLSRC = 1 (HSE) without corrupting reserved bits.

*/

#include <stdint.h>
#include "stdio.h"

typedef union {
    
    uint32_t reg;

    struct {

        uint32_t PLLM : 6;          // Bits 5:0        
        uint32_t PLLN : 9;          // Bits [14:6]
        uint32_t PLLP : 2;          // Bits [17:16]
        uint32_t RESERVED1 : 4;     // Bits [21:18] Reserved
        uint32_t PLLSRC : 1;        // Bit [22]
        uint32_t RESERVED2 : 9;     // Bits [31:23] Reserved

    } bits;

} RCC_PLLCFGR_TypeDef;

#define RCC_BASE_ADDRESS    0x40023800
#define RCC_PLLCFGR_OFFSET  0x0C
#define RCC_PLLCFGR         ((RCC_PLLCFGR_TypeDef *)(RCC_BASE_ADDRESS + RCC_PLLCFGR_OFFSET))

void configure_pll(void) {

    // Read the current register value
    uint32_t current_value = RCC_PLLCFGR->reg;

    // Create a new configuration value
    RCC_PLLCFGR_TypeDef new_config;
    new_config.reg = current_value;     // Start with the current value to preserve reserved bits

    // Set PLLM = 8, PLLN = 336, PLLP = 2 (encoded as 0b00), PLLSRC = 1 (HSE)
    new_config.bits.PLLM = 8;
    new_config.bits.PLLN = 336;
    new_config.bits.PLLP = 0b00;        // PLLP division factor of 2 is encoded as 0b00
    new_config.bits.PLLSRC = 1;         // Select HSE as the clock source

    // Write the new configuration back to the register
    RCC_PLLCFGR->reg = new_config.reg;

}