// Q 8. What does this dynamic memory navigation code print?

#include "stdio.h"
#include "stdlib.h"
#include "stddef.h"

typedef struct 
{

    int id;
    double score;

} Student;

int main(void)
{
    size_t count = 3;

    Student *pool = malloc(count * sizeof(Student));

    for(size_t i = 0; i < count; i++) 
    {
        pool[i].id    = (int)(101 + i);
        pool[i].score = 85.5 + i;
    }

    char *byte_ptr = (char *)pool;
    byte_ptr += sizeof(Student);
    Student *s2 = (Student *)byte_ptr;

    double *score_ptr = (double *)(byte_ptr + offsetof(Student, score));
    printf("ID: %d, Score: %.2f\n", s2->id, *score_ptr);

    free(pool);

    return 0;
}