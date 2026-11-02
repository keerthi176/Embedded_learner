//! Q 7. Custom embedded protocols encode flags in pointer low-bits (since heap pointers allocated on 8-byte 
//! boundaries have 3 trailing zero bits). Trace this tag-pointer program and state its output.


#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"

#define TAG_MASK 0x07

void *set_tag(void *ptr, uint8_t tag)
{
    return (void *)((uintptr_t)ptr | (tag & TAG_MASK));
}

void *clear_tag(void *ptr)
{
    return (void *)((uintptr_t)ptr & ~(uintptr_t)TAG_MASK);
}

uint8_t get_tag(void *ptr)
{
    return (uint8_t)((uintptr_t)ptr & TAG_MASK);
}

int main(void)
{
    int *data = malloc(sizeof(int));
    *data = 42;

    void *tagged_ptr = set_tag(data,5);
    printf("Tag: %d, ", get_tag(tagged_ptr));

    int *clean_ptr = (int *)clear_tag(tagged_ptr);
    printf("Value: %d\n", *clean_ptr);

    free(clean_ptr);

    return 0;
}