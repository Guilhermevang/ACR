#ifndef LOGGER_H
#define LOGGER_H

enum log_type
{
    INFO_LOG = 1,
    ERROR_LOG = 2
};

void custom_log(const char *format, enum log_type level, ...);
void log_info(const char *format, ...);
void log_error(const char *format, ...);

#endif