/*

        Question 3: UART Ring Buffer via Pointer Arithmetic Problem: Nucleo ST-LINK virtual COM port connects to USART2 (PA2 = TX, PA3 = RX). 
                    Implement a circular RX ring buffer and an interrupt handler that stores incoming serial bytes 
                    using pointer indexing without dropping data during rapid typing.

*/

#include <stdint.h>
#include <stdbool.h>

/* =========================================================
 * 1. RCC REGISTERS
 * ========================================================= */

#define RCC_AHB1ENR     (*(volatile uint32_t *)0x40023830UL)
#define RCC_APB1ENR     (*(volatile uint32_t *)0x40023840UL)

/* =========================================================
 * 2. GPIOA REGISTERS
 * ========================================================= */

#define GPIOA_MODER     (*(volatile uint32_t *)0x40020000UL)
#define GPIOA_AFRL      (*(volatile uint32_t *)0x40020020UL)

/* =========================================================
 * 3. USART2 REGISTERS
 * STM32F401RE: USART2 base = 0x40004400
 * ========================================================= */

#define USART2_SR       (*(volatile uint32_t *)0x40004400UL)
#define USART2_DR       (*(volatile uint32_t *)0x40004404UL)
#define USART2_BRR      (*(volatile uint32_t *)0x40004408UL)
#define USART2_CR1      (*(volatile uint32_t *)0x4000440CUL)

/* =========================================================
 * 4. NVIC REGISTERS
 * ========================================================= */

#define NVIC_ISER1      (*(volatile uint32_t *)0xE000E104UL)

/* =========================================================
 * 5. UART BIT DEFINITIONS
 * ========================================================= */

#define USART_SR_RXNE   (1UL << 5)
#define USART_CR1_RE     (1UL << 2)
#define USART_CR1_TE     (1UL << 3)
#define USART_CR1_RXNEIE (1UL << 5)
#define USART_CR1_UE     (1UL << 13)

/* =========================================================
 * 6. RING BUFFER CONFIGURATION
 * ========================================================= */

#define RX_BUFFER_SIZE  256U
#define RX_BUFFER_MASK  (RX_BUFFER_SIZE - 1U)

/* Buffer size must be a power of two */
#if (RX_BUFFER_SIZE & (RX_BUFFER_SIZE - 1U)) != 0
#error "RX_BUFFER_SIZE must be a power of two"
#endif

static uint8_t rx_buffer[RX_BUFFER_SIZE];

/* Head: next position to write */
/* Tail: next position to read */

static volatile uint16_t rx_head = 0U;
static volatile uint16_t rx_tail = 0U;

/* Counts bytes discarded because the software buffer was full */
static volatile uint32_t rx_overflow_count = 0U;

/* =========================================================
 * 7. RING BUFFER FUNCTIONS
 * ========================================================= */

/* Returns true if the buffer is empty */
bool uart_rx_empty(void)
{
    return (rx_head == rx_tail);
}

/* Returns true if the buffer is full */
bool uart_rx_full(void)
{
    uint16_t next_head =
        (uint16_t)((rx_head + 1U) & RX_BUFFER_MASK);

    return (next_head == rx_tail);
}

/* Get one byte from the ring buffer */
bool uart_rx_get_byte(uint8_t *data)
{
    if (data == 0)
    {
        return false;
    }

    /* No data available */
    if (rx_head == rx_tail)
    {
        return false;
    }

    /* Read using pointer indexing */
    *data = *(rx_buffer + rx_tail);

    /* Advance tail and wrap around */
    rx_tail = (uint16_t)((rx_tail + 1U) & RX_BUFFER_MASK);

    return true;
}

/* =========================================================
 * 8. USART2 INITIALIZATION
 * ========================================================= */

void uart2_init(void)
{
    /* Enable GPIOA clock */
    RCC_AHB1ENR |= (1UL << 0);

    /* Enable USART2 clock */
    RCC_APB1ENR |= (1UL << 17);

    /* Configure PA2 and PA3 as alternate-function pins */

    GPIOA_MODER &= ~((3UL << 4U) | (3UL << 6U));

    GPIOA_MODER |=  ((2UL << 4U) | (2UL << 6U));

    /* Select AF7 for PA2 and PA3 */

    GPIOA_AFRL &= ~((0xFUL << 8U) | (0xFUL << 12U));

    GPIOA_AFRL |=  ((7UL << 8U) | (7UL << 12U));

    /*
     * Assuming USART2 clock = 16 MHz:
     *
     * Baud rate = 115200
     * USARTDIV  = 16,000,000 / (16 * 115200)
     *           = 8.6806
     *
     * BRR       = 0x008B
     */

    USART2_CR1 = 0U;

    USART2_BRR = 0x008BUL;

    /* Enable receiver, transmitter and RXNE interrupt */

    USART2_CR1 |= USART_CR1_RE
                | USART_CR1_TE
                | USART_CR1_RXNEIE;

    /* Enable USART2 interrupt in the NVIC.
     * USART2_IRQn = 38
     * ISER1 bit = 38 - 32 = 6
     */

    NVIC_ISER1 = (1UL << 6U);

    /* Enable USART2 */
    USART2_CR1 |= USART_CR1_UE;
}

/* =========================================================
 * 9. USART2 INTERRUPT HANDLER
 * ========================================================= */

void USART2_IRQHandler(void)
{
    /* Check whether received data is available */
    if ((USART2_SR & USART_SR_RXNE) != 0U)
    {
        /* Read received byte from USART data register */
        uint8_t data = (uint8_t)(USART2_DR & 0xFFU);

        /* Calculate next head position */
        uint16_t next_head =
            (uint16_t)((rx_head + 1U) & RX_BUFFER_MASK);

        /* Check whether the ring buffer is full */
        if (next_head != rx_tail)
        {
            /* Store byte using pointer arithmetic */
            *(rx_buffer + rx_head) = data;

            /* Publish the new head position */
            rx_head = next_head;
        }
        else
        {
            /*
             * Buffer is full.
             * Preserve existing unread data and count
             * the byte that could not be stored.
             */
            rx_overflow_count++;
        }
    }
}

/* =========================================================
 * 10. MAIN APPLICATION
 * ========================================================= */

int main(void)
{
    uint8_t received_byte;

    uart2_init();

    while (1)
    {
        /* Process received bytes outside the ISR */

        if (uart_rx_get_byte(&received_byte))
        {
            /*
             * received_byte contains one UART byte.
             *
             * Add application processing here:
             * - Command parsing
             * - Protocol handling
             * - Data logging
             * - Echo transmission
             */
        }
    }
}