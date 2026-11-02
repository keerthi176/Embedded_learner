/*

    10: Memory-Mapped SPI Configuration & Transmission Problem: 
        Configure SPI1 in Master mode (0x40013000). 
        Write a pointer-based function spi_transfer_byte(uint8_t data) that sends a byte over MOSI 
        while reading the incoming MISO byte from the data register (DR).

*/

#include <stdint.h>

/* Base Addresses */
#define RCC_BASE      0x40023800UL
#define GPIOA_BASE    0x40020000UL
#define SPI1_BASE     0x40013000UL

/* Register Structures */
typedef struct {
    volatile uint32_t reserved[12];
    volatile uint32_t AHB1ENR;       /* Offset 0x30: AHB1 peripheral clock enable */
    volatile uint32_t reserved2[4];
    volatile uint32_t APB2ENR;       /* Offset 0x44: APB2 peripheral clock enable */
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
    volatile uint32_t CR1;            /* Control register 1 (Offset: 0x00) */
    volatile uint32_t CR2;            /* Control register 2 (Offset: 0x04) */
    volatile uint32_t SR;             /* Status register (Offset: 0x08) */
    volatile uint32_t DR;             /* Data register (Offset: 0x0C) */
    volatile uint32_t CRCPR;          /* CRC polynomial register (Offset: 0x10) */
    volatile uint32_t RXCRCR;         /* RX CRC register (Offset: 0x14) */
    volatile uint32_t TXCRCR;         /* TX CRC register (Offset: 0x18) */
    volatile uint32_t I2SCFGR;        /* I2S configuration register (Offset: 0x1C) */
    volatile uint32_t I2SPR;          /* I2S prescaler register (Offset: 0x20) */
} SPI_TypeDef;

/* Pointer Definitions */
#define RCC   ((RCC_TypeDef *)RCC_BASE)
#define GPIOA ((GPIO_TypeDef *)GPIOA_BASE)
#define SPI1  ((SPI_TypeDef *)SPI1_BASE)

void SPI1_Init(void) {

    /* 1. Enable Clocks for GPIOA and SPI1 */
    RCC->AHB1ENR |= (1U << 0);  /* Enable GPIOA clock */
    RCC->APB2ENR |= (1U << 12); /* Enable SPI1 clock (Bit 12 in APB2ENR) */

    /* 2. Configure PA5 (SCK), PA6 (MISO), and PA7 (MOSI) for Alternate Function 5 (SPI1) */
    /* Clear mode bits for pins 5, 6, 7 */
    GPIOA->MODER &= ~((3U << (5 * 2)) | (3U << (6 * 2)) | (3U << (7 * 2)));
    /* Set mode to Alternate Function (10) */
    GPIOA->MODER |=  ((2U << (5 * 2)) | (2U << (6 * 2)) | (2U << (7 * 2)));

    /* Set AFRL for PA5, PA6, PA7 to AF5 (0101) */
    GPIOA->AFR[0] &= ~((0xFU << (5 * 4)) | (0xFU << (6 * 4)) | (0xFU << (7 * 4)));
    GPIOA->AFR[0] |=  ((5U   << (5 * 4)) | (5U   << (6 * 4)) | (5U   << (7 * 4)));

    /* 3. Configure SPI1 Control Register 1 (CR1) */
    /* - Master Mode: MSTR = 1 (Bit 2) */
    /* - Baud Rate: f_PCLK / 8 -> BR[2:0] = 010 (Bits [5:3]) */
    /* - Software Slave Management: SSM = 1, SSI = 1 (Bits 9 & 8) */
    /* - Clock Polarity & Phase: CPOL = 0, CPHA = 0 (Mode 0, default) */
    SPI1->CR1 = (1U << 2) |  /* MSTR: Master configuration */
                (2U << 3) |  /* BR[2:0]: Baud rate control (f_PCLK/8) */
                (1U << 8) |  /* SSI: Internal slave select */
                (1U << 9);   /* SSM: Software slave management enabled */

    /* 4. Enable SPI1 */
    SPI1->CR1 |= (1U << 6);  /* SPE: Peripheral enabled */
}

/**
 * Transmits a byte over MOSI and simultaneously reads the incoming byte from MISO.
 */
uint8_t spi_transfer_byte(uint8_t data) {

    /* 1. Wait until TXE (Transmit buffer empty) flag is set in SPI_SR */
    while (!(SPI1->SR & (1U << 1))) {
        /* Wait for transmission buffer to be ready */
    }

    /* 2. Write data to the Data Register (DR) to initiate transmission */
    *(volatile uint8_t *)&SPI1->DR = data; /* Byte-access to DR */

    /* 3. Wait until RXNE (Receive buffer not empty) flag is set in SPI_SR */
    while (!(SPI1->SR & (1U << 0))) {
        /* Wait for incoming byte to arrive */
    }

    /* 4. Read and return the received byte from Data Register (DR) */
    return *(volatile uint8_t *)&SPI1->DR;
}

int main(void) {

    SPI1_Init();

    uint8_t response = 0;

    while (1) {
        
        /* Transmit a dummy command or data byte (e.g., 0xFF) and capture response */
        response = spi_transfer_byte(0xFF);

        /* Small delay between transfers */
        for (volatile int i = 0; i < 500000; i++) {
            __asm("nop");
        }
    }
}