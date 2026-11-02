// Q 3. What is the exact printed integer output of the following nested pointer expression?

#include "stdio.h"

int main(void)
{
    int grid[3][4] = {{1, 2, 3, 4}, 
                      {5, 6, 7, 8}, 
                      {9, 10, 11, 12}
                    };

    int (*p)[4] = grid;
    int *ptr = (int *)(p + 2);

    printf("%d\n",ptr[-5]);
    return 0;
}

// Answer: The program prints "4".
// Still a doubt because it is causing a undefined behsvior because ptr[-5] is accessing memory outside the bounds of the array.
