// Q 10. Look at this code carefully. Under aggressive compiler optimizations (-O3), 
// what logical issue exists with pointer aliasing, and what can it print if the compiler reorders accesses?


#include "stdio.h"

void modify_val(float *f, int *i)
{
    *f = 1.0f;
    *i = 0;
}

int main(void)
{
    union {
        
        float f;
        int i;
    }u;

    modify_val(&u.f, &u.i);
    printf("f: %f, i: %d\n", u.f, u.i);

    return 0;
}

// Answer: The program may print "f: 0.000000, i: 0".