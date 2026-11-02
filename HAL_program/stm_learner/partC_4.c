/*

    4: Hardware Timer PWM for LED Breathing Problem: 
        Configure TIM2 Channel 1 (mapped to PA5 on many Nucleo boards) to generate a Hardware PWM signal. 
        Use pointer iteration inside a main loop to dynamically adjust the capture/compare register (CCR1), 
        creating a smooth LED breathing effect.

*/

#include <stdint.h>

/* Base Addresses */
#define RCC_BASE      0x40023800UL
#define GPIOA_BASE    0x40020000UL
#define TIM2_BASE     0x40000000UL

/* Register Structures */
typedef struct {
    volatile uint32_t reserved[12];
    volatile uint32_t AHB1ENR;       /* Offset 0x30: AHB1 peripheral clock enable */
    volatile uint32_t reserved1[3];
    volatile uint32_t APB1ENR;       /* Offset 0x40: APB1 peripheral clock enable */
} RCC_TypeDef;

typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFR[2];         /* [0] = AFRL, [1] = AFRH */
} GPIO_TypeDef;

typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t CCMR2;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
    volatile uint32_t RESERVED0;
    volatile uint32_t CCR1;
    volatile uint32_t CCR2;
    volatile uint32_t CCR3;
    volatile uint32_t CCR4;
} TIM_TypeDef;

/* Pointer Definitions */
#define RCC   ((RCC_TypeDef *)RCC_BASE)
#define GPIOA ((GPIO_TypeDef *)GPIOA_BASE)
#define TIM2  ((TIM_TypeDef *)TIM2_BASE)

/* Simple software delay loop for breathing step transition speed */
void delay_ms(volatile uint32_t count) {
    while (count--) {
        for (volatile int i = 0; i < 1000; i++) {
            __asm("nop");
        }
    }
}

int main(void) {
    /* 1. Enable Clocks for GPIOA and TIM2 */
    RCC->AHB1ENR |= (1U << 0);  /* Enable GPIOA clock */
    RCC->APB1ENR |= (1U << 0);  /* Enable TIM2 clock */

    /* 2. Configure PA5 for Alternate Function mode (AF1 for TIM2_CH1) */
    GPIOA->MODER &= ~(3U << (5 * 2));
    GPIOA->MODER |=  (2U << (5 * 2)); /* 10 = Alternate function mode */

    /* Set AFRL for PA5 to AF1 (0001) */
    GPIOA->AFR[0] &= ~(0xFU << (5 * 4));
    GPIOA->AFR[0] |=  (1U   << (5 * 4));

    /* 3. Configure Timer Timebase */
    /* Assuming a 16 MHz HSI clock: */
    /* Prescaler = 16 - 1 -> Timer clock runs at 1 MHz (1 us per tick) */
    TIM2->PSC = 16 - 1;
    
    /* Auto-Reload Register sets period to 1000 ticks (1 ms PWM period = 1 kHz frequency) */
    TIM2->ARR = 1000 - 1;

    /* 4. Configure TIM2 Channel 1 for PWM Mode 1 */
    /* OC1M = 110 (PWM Mode 1: Active when CNT < CCR1, inactive otherwise) */
    TIM2->CCMR1 &= ~(0xFFU);
    TIM2->CCMR1 |= (6U << 4) | (1U << 3); /* PWM Mode 1 enabled, Output preload enable */

    /* Enable Channel 1 Output with active-high polarity */
    TIM2->CCER |= (1U << 0); /* CC1E: Capture/Compare 1 output enable */

    /* 5. Enable Timer Counter */
    TIM2->CR1 |= (1U << 0);  /* CEN: Counter enable */

    /* 6. Dynamic Breathing Loop via CCR1 Pointer Iteration */
    int16_t brightness = 0;
    int8_t step = 10;

    while (1) {
        /* Update the Capture/Compare Register 1 to change the duty cycle */
        TIM2->CCR1 = brightness;

        /* Adjust brightness level */
        brightness += step;

        /* Reverse direction at peak brightness or completely off */
        if (brightness >= 1000 || brightness <= 0) {
            step = -step;
        }

        /* Control the speed of the fade transition */
        delay_ms(15);
    }
}
