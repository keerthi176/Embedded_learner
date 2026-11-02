/*

    6: SysTick High-Precision Microsecond Delay Problem: 
        The ARM Cortex-M SysTick timer is a system timer built into the core (0xE000E010). 
        Write initialization and delay functions using SysTick pointer registers (CTRL, LOAD, VAL) 
        to achieve accurate microsecond (delay_us) delays.

*/

#include <stdint.h>

/* Base Addresses */
#define RCC_BASE      0x40023800UL
#define GPIOA_BASE    0x40020000UL
#define SYST_BASE     0xE000E010UL  /* SysTick Base Address */

/* Register Structures */
typedef struct {
    volatile uint32_t reserved[12];
    volatile uint32_t AHB1ENR;       /* Offset 0x30 */
} RCC_TypeDef;

typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
} GPIO_TypeDef;

typedef struct {
    volatile uint32_t CTRL;          /* Control and status register (Offset 0x00) */
    volatile uint32_t LOAD;          /* Reload value register (Offset 0x04) */
    volatile uint32_t VAL;           /* Current value register (Offset 0x08) */
    volatile uint32_t CALIB;         /* Calibration value register (Offset 0x0C) */
} SysTick_TypeDef;

/* Pointer Definitions */
#define RCC     ((RCC_TypeDef *)RCC_BASE)
#define GPIOA   ((GPIO_TypeDef *)GPIOA_BASE)
#define SysTick ((SysTick_TypeDef *)SYST_BASE)

/* SysTick Microsecond Delay Function */
void delay_us(uint32_t us) {
    /* Assuming a 16 MHz processor core clock: */
    /* 1 microsecond = 16 CPU cycles */
    SysTick->LOAD = us * 16 - 1;             /* Set reload value for target microseconds */
    SysTick->VAL  = 0;                       /* Clear current value and COUNTFLAG */
    SysTick->CTRL = (1U << 2) | (1U << 0);   /* Enable SysTick with processor clock (CLKSOURCE=1, ENABLE=1) */

    /* Wait until the COUNTFLAG (bit 16) is set, indicating countdown reached zero */
    while (!(SysTick->CTRL & (1U << 16))) {
        /* Polling loop */
    }

    SysTick->CTRL = 0;                       /* Stop SysTick */
}

int main(void) {
    /* 1. Enable GPIOA clock for user LED (LD2 on PA5) */
    RCC->AHB1ENR |= (1U << 0);

    /* 2. Configure PA5 as General Purpose Output */
    GPIOA->MODER &= ~(3U << (5 * 2));
    GPIOA->MODER |=  (1U << (5 * 2));

    /* 3. Toggle LED using high-precision microsecond delays */
    while (1) {
        GPIOA->ODR ^= (1U << 5);             /* Toggle LED */
        
        /* Delay for 500,000 microseconds (500 ms) */
        delay_us(500000);
    }
}