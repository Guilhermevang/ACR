#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "logger.h"

void custom_log(const char *format, enum log_type level, ...)
{
    va_list args;
    va_start(args, level);

    const char *prefix;
    switch (level)
    {
    case INFO_LOG:
        prefix = "[INFO] ";
        break;
    case ERROR_LOG:
        prefix = "[ERROR] ";
        break;
    default:
        prefix = "[INFO] ";
        break;
    }

    // Buffer para armazenar a mensagem formatada
    char message[512];
    vsnprintf(message, sizeof(message), format, args);

    // Envia para o stdout ou stderr
    if (level == ERROR_LOG)
    {
        fprintf(stderr, "%s%s\n", prefix, message);
    }
    else
    {
        fprintf(stdout, "%s%s\n", prefix, message);
    }

    va_end(args);
}

void log_info(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    custom_log(format, INFO_LOG, args);
    va_end(args);
}

void log_error(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    custom_log(format, ERROR_LOG, args);
    va_end(args);
}