#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <stdio.h>
#include <stdlib.h>

void *safe_malloc(size_t size);
void free_all_resources();
void free_resource(void *ptr);

#endif