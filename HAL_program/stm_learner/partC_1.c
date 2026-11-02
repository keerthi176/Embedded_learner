/*

Question 1: Bare-Metal GPIO & Clock Enable via Pointer Structs Problem: 
    On an STM32 Nucleo board, the user LED (LD2) is typically connected to PA5 (Port A, Pin 5). Write a bare-metal C program using structure pointers to. 
        1. Enable the clock for GPIOA in the RCC (Reset and Clock Control) register (RCC_AHB1ENR at offset 0x30 from base 0x40023800). 
        2. Configure PA5 as a General Purpose Output mode in GPIOx_MODER. 
        3. Toggle the LED every ~500ms using a simple software delay loop, accessing GPIOx_ODR directly.

*/

#include <stdint.h>

typedef struct {

    volatile uint32_t CR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t APB2RSTR;
    volatile uint32_t APB1RSTR;
    volatile uint32_t AHBENR;

} RCC;

typedef struct {

    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFR[2];

} GPIO;

#define RCC_BASE    ((RCC *)0x40021000U)
#define GPIOA       ((GPIO *)0x48000000U)

void delay(volatile uint32_t count)
{
    while (count--)
    {
        __asm volatile ("nop");
    }
}

int main(void)
{
    // Enable GPIOA clock
    RCC_BASE->AHBENR |= (1U << 17);

    // Configure PA5 as output
    GPIOA->MODER &= ~(3U << 10);
    GPIOA->MODER |= (1U << 10);

    // Push-pull configuration
    GPIOA->OTYPER &= ~(1U << 5);

    // Low speed
    GPIOA->OSPEEDR &= ~(3U << 10);

    // No pull-up or pull-down
    GPIOA->PUPDR &= ~(3U << 10);

    while (1)
    {
        // LED ON
        GPIOA->ODR |= (1U << 5);

        delay(500000);

        // LED OFF
        GPIOA->ODR &= ~(1U << 5);

        delay(500000);
    }

    return 0;
}