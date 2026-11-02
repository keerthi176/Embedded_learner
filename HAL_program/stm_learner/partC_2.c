/*

    Question2: EXTI Push-Button Interrupt Handler Problem: The Nucleo blue user button (B1) is connected to PC13. 
    Configure PC13 as an external interrupt (EXTI13) triggered on the falling edge (button press connects pin to GND). 
    Write the Interrupt Service Routine (ISR) using function pointers to toggle LD2 (PA5) whenever the button is pressed.

*/

#include <stdint.h>

/* RCC Registers */
#define RCC_AHB1ENR     (*(volatile uint32_t *)0x40023830UL)
#define RCC_APB2ENR     (*(volatile uint32_t *)0x40023844UL)

/* GPIOA Registers */
#define GPIOA_MODER     (*(volatile uint32_t *)0x40020000UL)
#define GPIOA_ODR       (*(volatile uint32_t *)0x40020014UL)

/* GPIOC Registers */
#define GPIOC_MODER     (*(volatile uint32_t *)0x40020800UL)
#define GPIOC_PUPDR     (*(volatile uint32_t *)0x4002080CUL)

/* SYSCFG Registers */
#define SYSCFG_EXTICR4  (*(volatile uint32_t *)0x40013814UL)

/* EXTI Registers */
#define EXTI_IMR        (*(volatile uint32_t *)0x40013C00UL)
#define EXTI_RTSR       (*(volatile uint32_t *)0x40013C08UL)
#define EXTI_FTSR       (*(volatile uint32_t *)0x40013C0CUL)
#define EXTI_PR         (*(volatile uint32_t *)0x40013C14UL)

/* NVIC Registers */
#define NVIC_ISER1      (*(volatile uint32_t *)0xE000E104UL)

/* Pin definitions */
#define LED_PIN         5U
#define BUTTON_PIN      13U

#define LED_MASK        (1UL << LED_PIN)
#define BUTTON_MASK     (1UL << BUTTON_PIN)

/* Function pointer declaration */
typedef void (*ISR_Callback)(void);

/* LED toggle function */
void toggle_led(void)
{
    GPIOA_ODR ^= LED_MASK;
}

/* Function pointer initialized to the LED toggle function */
ISR_Callback button_callback = toggle_led;

/* GPIO and EXTI initialization */
void button_exti_init(void)
{
    /* 1. Enable GPIOA and GPIOC clocks */
    RCC_AHB1ENR |= (1UL << 0) | (1UL << 2);

    /* 2. Enable SYSCFG peripheral clock */
    RCC_APB2ENR |= (1UL << 14);

    /* 3. Configure PA5 as output */
    GPIOA_MODER &= ~(3UL << (LED_PIN * 2U));
    GPIOA_MODER |=  (1UL << (LED_PIN * 2U));

    /* Initially turn LED OFF */
    GPIOA_ODR &= ~LED_MASK;

    /* 4. Configure PC13 as input */
    GPIOC_MODER &= ~(3UL << (BUTTON_PIN * 2U));

    /* Enable pull-up on PC13 */
    GPIOC_PUPDR &= ~(3UL << (BUTTON_PIN * 2U));
    GPIOC_PUPDR |=  (1UL << (BUTTON_PIN * 2U));

    /* 5. Route EXTI13 to GPIOC */
    SYSCFG_EXTICR4 &= ~(0xFUL << 4U);
    SYSCFG_EXTICR4 |=  (0x2UL << 4U);

    /* 6. Disable EXTI13 while configuring it */
    EXTI_IMR &= ~BUTTON_MASK;

    /* 7. Disable rising-edge trigger */
    EXTI_RTSR &= ~BUTTON_MASK;

    /* 8. Enable falling-edge trigger */
    EXTI_FTSR |= BUTTON_MASK;

    /* 9. Clear any previously pending interrupt */
    EXTI_PR = BUTTON_MASK;

    /* 10. Enable EXTI15_10 interrupt in NVIC */
    NVIC_ISER1 = (1UL << 8U);

    /* 11. Unmask EXTI13 */
    EXTI_IMR |= BUTTON_MASK;
}

/* Interrupt Service Routine */
void EXTI15_10_IRQHandler(void)
{
    /* Check whether EXTI13 caused the interrupt */
    if ((EXTI_PR & BUTTON_MASK) != 0U)
    {
        /* Clear the pending flag by writing 1 */
        EXTI_PR = BUTTON_MASK;

        /* Invoke the callback through the function pointer */
        if (button_callback != 0)
        {
            button_callback();
        }
    }
}

int main(void)
{
    button_exti_init();

    while (1)
    {
        /* CPU can perform other tasks here.
           LED toggling is handled by the interrupt. */
    }
}