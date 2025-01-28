#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "resource-manager.h"
#include "helper-functions.h"
#include "logger.h"

FILE *open_file(const char *file_name)
{
    FILE *file = fopen(file_name, "rb");
    if (file == NULL)
    {
        log_error("ERRO: Não foi possível abrir o arquivo '%s'\n", file_name);
        exit(EXIT_FAILURE);
    }

    return file;
}

// Lida com erros, formatando mensagens com variáveis
int handle_error(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    // Buffer para armazenar a mensagem formatada
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format, args); // Formata a mensagem

    log_error(buffer); // Envia a mensagem formatada para o logger

    va_end(args);

    free_all_resources();

    exit(EXIT_FAILURE);
}