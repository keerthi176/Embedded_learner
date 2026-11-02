/*

    9: Flash Memory Unlock & Sector Erase Sequence Problem: 
        STM32 flash memory requires a strict key unlock sequence before modification. 
        Write a function using pointers to unlock the Flash Controller (FLASH_KEYR at 0x40023C04) and erase Sector 7 (0x08060000) in flash memory

*/

#include <stdint.h>

/* Flash Interface Base Address */
#define FLASH_BASE    0x40023C00UL

/* Flash Register Structure */
typedef struct {
    volatile uint32_t ACR;           /* Access control register (Offset: 0x00) */
    volatile uint32_t KEYR;          /* Key register (Offset: 0x04) */
    volatile uint32_t OPTKEYR;       /* Option key register (Offset: 0x08) */
    volatile uint32_t SR;            /* Status register (Offset: 0x0C) */
    volatile uint32_t CR;            /* Control register (Offset: 0x10) */
    volatile uint32_t OPTCR;         /* Option control register (Offset: 0x14) */
} FLASH_TypeDef;

/* Pointer Definition */
#define FLASH ((FLASH_TypeDef *)FLASH_BASE)

/**
 * Unlocks the Flash Control Register (FLASH_CR)
 */
void Flash_Unlock(void) {
    /* If the lock bit is set, write the two consecutive unlock keys to KEYR */
    if (FLASH->CR & (1U << 31)) {
        FLASH->KEYR = 0x45670123U; /* KEY1 */
        FLASH->KEYR = 0xCDEF89ABU; /* KEY2 */
    }
}

/**
 * Locks the Flash Control Register (FLASH_CR) back up
 */
void Flash_Lock(void) {
    FLASH->CR |= (1U << 31); /* Set LOCK bit */
}

/**
 * Erases Sector 7 (starts at 0x08060000)
 */
void Flash_Erase_Sector7(void) {
    /* 1. Wait until any ongoing flash operation is completed (BSY bit in SR) */
    while (FLASH->SR & (1U << 16)) {
        /* Polling BSY bit */
    }

    /* 2. Unlock Flash controller */
    Flash_Unlock();

    /* 3. Configure Sector Erase in FLASH_CR */
    /* Clear PSIZE (optional, depends on voltage, default x32/x16 works) */
    /* Set SER bit (Sector Erase, bit 1) */
    /* Select Sector 7 in SNB[3:0] (Bits [6:3] = 0111) */
    FLASH->CR &= ~(0xFU << 3);       /* Clear sector number bits */
    FLASH->CR |=  (7U   << 3);       /* Set sector = 7 */
    FLASH->CR |=  (1U   << 1);       /* Set SER (Sector Erase) bit */

    /* 4. Trigger the erase operation by setting STRT bit (bit 16) */
    FLASH->CR |=  (1U << 16);

    /* 5. Wait for BSY bit to clear, indicating erase completion */
    while (FLASH->SR & (1U << 16)) {
        /* Polling BSY bit */
    }

    /* 6. Clear the SER bit and lock flash memory */
    FLASH->CR &= ~(1U << 1);
    Flash_Lock();
}

int main(void) {
    /* Erase Sector 7 of Flash Memory */
    Flash_Erase_Sector7();

    while (1) {
        /* Main application loop */
    }
}