#include <stdint.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>
#include "leakchecker.h"

void *malloc(size_t);
void free(void *);
void track(size_t, void *, char, char);

void *malloc(size_t size)
{
    static void *(*memfunc)(size_t) = NULL;
    if (!memfunc)
    {
        memfunc = dlsym(RTLD_NEXT, "malloc");
    }
    void *p = memfunc(size);
    track(size, p, 0x0, 0x0);
    printMem();
    return p;
}

void free(void *p)
{
    static void *(*memfree)(void *ptr) = NULL;
    if (!memfree)
    {
        memfree = dlsym(RTLD_NEXT, "free");
    }
    track(0, p, 0x1, 0x0);
    memfree(p);
    printMem();
}

struct Block
{
    void *ptr;
    size_t size;
    struct Block *next;
};
typedef struct Block Block;

void track(size_t size, void *ptr, char b, char p)
{
    static void *(*memfunc)(size_t) = NULL;
    if (!memfunc)
    {
        memfunc = dlsym(RTLD_NEXT, "malloc");
    }
    static void *(*memfree)(void *ptr) = NULL;
    if (!memfree)
    {
        memfree = dlsym(RTLD_NEXT, "free");
    }

    static Block *inIt = NULL;
    static size_t total = 0;
    static Block *alloced = NULL;

    if (p == 0x1)
    {
        printf("Allocated memory: %zu \n", total);
        return;
    }

    if (!alloced)
    {
        alloced = (Block *)memfunc(sizeof(Block));
        alloced->ptr = NULL;
        alloced->size = 0;
        alloced->next = NULL;
        inIt = alloced;
    }

    if (b == 0x0)
    {
        Block *b = (Block *)memfunc(sizeof(Block));
        b->ptr = ptr;
        b->size = size;
        b->next = NULL;
        inIt->next = b;
        inIt = b;
        total += size;
    }
    else
    {
        if (ptr == NULL)
        {
            return;
        }
        Block *prev = NULL;
        for (Block *t = alloced; t != inIt; t = t->next)
        {
            if (t->ptr == ptr)
            {
                total -= t->size;
                if (t == alloced)
                {
                    alloced = alloced->next;
                    memfree(t);
                }
                else
                {
                    prev->next = t->next;
                    memfree(t);
                }
                break;
            }
            prev = t;
        }
        if (inIt->ptr == ptr)
        {
            total -= inIt->size;
            prev->next = NULL;
            memfree(inIt);
            inIt = prev;
        }
    }
}

void printMem()
{
    track(0, NULL, 0, 0x1);
}