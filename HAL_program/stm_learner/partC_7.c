/*

    7: UART Direct String Transmission
        Problem: Write a blocking function uart_send_string(const char *str) 
        that transmits a null-terminated string over USART2 
        using pointer traversal and checking the Transmit Data Register Empty (TXE) bit in USART_SR.

*/

#include <stdint.h>

/* USART Register Structure (as defined previously) */
typedef struct {

    volatile uint32_t SR;            /* Status register (Offset: 0x00) */
    volatile uint32_t DR;            /* Data register (Offset: 0x04) */
    volatile uint32_t BRR;           /* Baud rate register (Offset: 0x08) */
    volatile uint32_t CR1;           /* Control register 1 (Offset: 0x0C) */
    volatile uint32_t CR2;           /* Control register 2 (Offset: 0x10) */
    volatile uint32_t CR3;           /* Control register 3 (Offset: 0x14) */
    volatile uint32_t GTPR;          /* Guard time and prescaler register (Offset: 0x18) */
    
} USART_TypeDef;

#define USART2_BASE   0x40004400UL
#define USART2 ((USART_TypeDef *)USART2_BASE)

/**
 * Transmits a single character over USART2 (blocking)
 */
void uart_send_char(char c) {
    /* Wait until TXE (Transmit data register empty) bit is set (Bit 7 in SR) */
    while (!(USART2->SR & (1U << 7))) {
        /* Polling loop */
    }
    
    /* Writing to DR clears the TXE flag and initiates transmission */
    USART2->DR = (uint8_t)c;
}

/**
 * Transmits a null-terminated string using pointer traversal
 */
void uart_send_string(const char *str) {
    /* Traverse the string using the pointer until the null terminator ('\0') is reached */
    while (*str != '\0') {
        uart_send_char(*str);
        str++; /* Advance the pointer to the next character */
    }
}

int main(void) {
    /* Assume USART2_Init() has already been called to configure */
    /* GPIOA (PA2 TX, PA3 RX), Baud Rate, and enable USART2. */
    // USART2_Init();

    while (1) {
        uart_send_string("Hello from STM32 Bare-Metal!\r\n");
        
        /* Simple delay loop between transmissions */
        for (volatile int i = 0; i < 1000000; i++) {
            __asm("nop");
        }
    }
}