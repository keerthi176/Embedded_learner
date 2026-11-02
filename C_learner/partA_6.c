// State the printed output of this self-modifying function dispatch system?

#include "stdio.h"

typedef int (*state_fn)(int);

int state_a(int x);
int state_b(int x);

state_fn transition_table[] = {state_a, state_b};

int state_a(int x)
{
    printf("State A: %d\n", x);
    return x>5?1:0;
}

int state_b(int x)
{
    printf("State B: %d\n", x);
    return 0;
}

int main(void)
{
    state_fn *current_state = transition_table;
    int val = 2;

    for(int i = 0; i<3;i++) {

        int next_idx = (*current_state)(val);

        current_state = transition_table + next_idx;
        val = 3;
    }

    printf("\n");

    return 0;
}

// Answer: The program prints:
// State A: 2
// State A: 3
// State A: 3