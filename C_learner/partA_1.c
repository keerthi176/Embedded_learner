// Q 1. Analyze the following code. Determine what integer value gets printed, or if it triggers undefined behavior/crash, explain precisely why?

#include "stdio.h"
#include "stdint.h"

typedef void (*action_t)(int*);

void add_five(int *val)
{
    *val +=5;
}

void mul_two(int *val)
{
    *val *=2;
}

int main(void)
{
    action_t ops[]  = {add_five, mul_two};
    action_t *v_ptr = ops;
    int x =10;
    
    uintptr_t base = (uintptr_t)v_ptr;
    action_t *p1 = (action_t *)(base + sizeof(action_t));
    
    (*(p1 - 1))(&x);
    (*p1)(&x);
    
    printf("x = %d\n",x);
    
    return 0;
}

// Answer: The program prints "x = 30".