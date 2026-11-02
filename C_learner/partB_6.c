/*
    An embedded flash memory is divided into two virtual pages for EEPROM emulation. 
    Each page begins with a 4-byte header: 1. 0x00000000: ERASED page 2. 0x11223344: VALID page 3. 
    0x55667788: RECEIVE_DATA (Active migration page) Given FLASH_PAGE0_BASE = 0x08008000 and FLASH_PAGE1_BASE = 0x0800C000, 
    write a function using pointer casting to return a pointer to the current VALID page base address. If neither or both are valid, return NULL.
*/

#include <stdint.h>
#include <stddef.h>

#define FLASH_PAGE0_BASE  0x08008000UL
#define FLASH_PAGE1_BASE  0x0800C000UL

#define PAGE_ERASED       0x00000000UL
#define PAGE_VALID        0x11223344UL
#define PAGE_RECEIVE_DATA 0x55667788UL

uint32_t *GetValidFlashPage(void)
{
    volatile uint32_t *page0 =
        (volatile uint32_t *)FLASH_PAGE0_BASE;

    volatile uint32_t *page1 =
        (volatile uint32_t *)FLASH_PAGE1_BASE;

    uint32_t header0 = *page0;
    uint32_t header1 = *page1;

    if ((header0 == PAGE_VALID) &&
        (header1 != PAGE_VALID))
    {
        return (uint32_t *)FLASH_PAGE0_BASE;
    }
    else if ((header1 == PAGE_VALID) &&
             (header0 != PAGE_VALID))
    {
        return (uint32_t *)FLASH_PAGE1_BASE;
    }

    /* Neither page or both pages are VALID */
    return NULL;
}


