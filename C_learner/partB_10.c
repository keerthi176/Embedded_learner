/*

    A system features 4 UART channels (UART0 through UART3). Each UART has a base register map defined by UART_TypeDef:

    typedef struct {
    volatile uint32_t DR; // Data Register
    volatile uint32_t SR; // Status Register
    volatile uint32_t CR1; // Control Register 1
    } UART_TypeDef; 
    
    Define a pointer dispatch array mapping base addresses 0x4000C000, 0x4000C400, 0x4000C800, and 0x4000CC00. 
    Write a single line function using this table to transmit byte data on channel uart_ch by waiting until the Transmit Data Register Empty (TXE, bit 7) flag is set in SR.

*/

#include <stdint.h>

typedef struct {
    volatile uint32_t DR;
    volatile uint32_t SR;
    volatile uint32_t CR1;
} UART_TypeDef;

#define UART_TXE (1U << 7)

UART_TypeDef * const uart_table[4] = {
    (UART_TypeDef *)0x4000C000UL,  // UART0
    (UART_TypeDef *)0x4000C400UL,  // UART1
    (UART_TypeDef *)0x4000C800UL,  // UART2
    (UART_TypeDef *)0x4000CC00UL   // UART3
};

void UART_TransmitByte(uint8_t uart_ch, uint8_t data)
{
    while ((uart_table[uart_ch]->SR & UART_TXE) == 0U) {}
    uart_table[uart_ch]->DR = data;
}


int main(void)
{
    // Example usage: Transmit byte 0x55 on UART channel 1
    UART_TransmitByte(1, 0x55);

    return 0;
}

