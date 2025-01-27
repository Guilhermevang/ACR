#include <stdio.h>
#include <stdlib.h>

#include "helper-functions.h"

FILE *open_file(const char *file_name)
{
    FILE *file = fopen(file_name, "rb");
    if (file == NULL)
    {
        printf("ERRO: Não foi possível abrir o arquivo '%s'\n", file_name);
        exit(EXIT_FAILURE);
    }

    return file;
}