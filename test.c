#include <stdlib.h>
#include <stdio.h>

int main()
{
    int *i = (int *)malloc(10 * sizeof(int));
    int *j = (int *)malloc(10 * sizeof(int));
    int *k = (int *)malloc(10 * sizeof(int));
    int *l = (int *)malloc(10 * sizeof(int));
    free(i);
    free(j);
    free(k);
    free(l);

    return 0;
}