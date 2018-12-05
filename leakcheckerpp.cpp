#include <cstdlib>
#include "leakchecker.h"

void *operator new(std::size_t size)
{
    return malloc(size);
}

void *operator new[](std::size_t size)
{
    return malloc(size);
}
void operator delete(void *ptr)
{
    free(ptr);
    ptr = NULL;
}
void operator delete[](void *ptr)
{
    free(ptr);
    ptr = NULL;
}
