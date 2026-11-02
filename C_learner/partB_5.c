/*

    In ARM Cortex-M microcontrollers, vector table relocation is achieved by writing the vector table's base memory address to the Vector Table Offset Register 
    (VTOR, mapped at 0xE000ED08). Write a C function that takes a dynamic RAM array containing new ISR function pointers and
     updates the hardware VTOR register using pointers. What alignment rule must the new base address observe if the table contains 64 vectors?

*/

#include <stdint.h>
#include <stdbool.h>

// VTOR hardware register memory mapping
#define SYSTEM_CONTROL_SPACE_BASE  (0xE000E000UL)
#define VTOR_OFFSET                (0xD08UL)
#define SCB_VTOR                   (*((volatile uint32_t *)(SYSTEM_CONTROL_SPACE_BASE + VTOR_OFFSET)))

#define NUM_VECTORS                (64U)
#define VECTOR_TABLE_SIZE_BYTES    (NUM_VECTORS * sizeof(uint32_t)) // 256 Bytes
#define REQUIRED_ALIGNMENT_BYTES   (256U)

/**
 * @brief Relocates the Cortex-M Vector Table to a dynamic RAM array.
 * @param ram_vector_table Pointer to a 256-byte aligned uint32_t array of 64 entries.
 * @return true if VTOR was updated successfully, false if alignment check failed.
 */
bool relocate_vector_table(const uint32_t *ram_vector_table) {
    uint32_t new_vtor_address = (uint32_t)ram_vector_table;

    // 1. Verify 256-byte alignment rule (lowest 8 bits must be 0)
    if ((new_vtor_address % REQUIRED_ALIGNMENT_BYTES) != 0U) {
        return false; // Misaligned memory base address
    }

    // 2. Write new vector table base address to VTOR register
    SCB_VTOR = new_vtor_address;

    // 3. Execution & Data Memory Synchronization Barriers
    // Ensures all previous memory writes complete and flushes CPU instruction pipeline
    __asm volatile (
        "dsb 0xF \n\t"  // Data Synchronization Barrier
        "isb 0xF \n\t"  // Instruction Synchronization Barrier
        ::: "memory"
    );

    return true;
}
