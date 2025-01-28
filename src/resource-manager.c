#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "resource-manager.h"
#include "helper-functions.h"

#define MAX_ALLOCATIONS 128

static void *allocated_pointers[MAX_ALLOCATIONS];
static size_t allocation_count = 0;

void *safe_malloc(size_t size)
{
    if (allocation_count >= MAX_ALLOCATIONS)
    {
        handle_error("Não é possível alocar mais memória do que %s\n", MAX_ALLOCATIONS);
    }

    void *ptr = malloc(size);
    if (!ptr)
    {
        handle_error("Não foi possível alocar memória. Tamanho: %d\n", size);
    }

    allocated_pointers[allocation_count++] = ptr;

    return ptr;
}

void free_all_resources()
{
    for (allocation_count; allocation_count > 0; allocation_count--)
    {
        free(allocated_pointers[allocation_count - 1]);
        allocated_pointers[allocation_count - 1] = NULL;
    }
}

void free_resource(void *ptr)
{
    int found = 0;
    int i = -1;

    while (!found && i < allocation_count)
    {
        i++;
        found = memcmp(ptr, allocated_pointers[i], sizeof(ptr)) == 0;
    }

    if (found)
    {
        // Libera o recurso
        free(allocated_pointers[i]);
        allocated_pointers[i] = NULL;

        // Reorganiza o array
        for (int j = i; j < allocation_count - 1; j++)
        {
            allocated_pointers[j] = allocated_pointers[j - 1];
        }

        allocated_pointers[allocation_count - 1] = NULL; // Por segurança
        allocation_count--;
    }
}