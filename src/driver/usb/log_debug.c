#include "stdint.h"
#include "stdarg.h"
#include "INC/usb/log_debug.h"
#include "utils/uartstdio.h"

#define RED_LOG     "\033[31m"
#define GREEN_LOG   "\033[32m"
#define BLUE_LOG    "\033[34m"
#define YELLOW_LOG  "\033[33m"
#define END_LOG     "\033[0m"

void log_debug_array(const char * cstring, unsigned char *array, unsigned short bcnt) {
    unsigned int i = 0;
    UARTprintf(cstring);
    for (i=0; i < bcnt; i++) {
        UARTprintf("%02x ", array[i]);
    }
    UARTprintf("\n");
}



char const * const _get_log_level_string(LogLevel const log_level)
{
    switch(log_level)
    {
        case LOG_LEVEL_ERROR:
            return RED_LOG "ERROR" END_LOG;
        case LOG_LEVEL_INFORMATION:
            return YELLOW_LOG "INFO" END_LOG;
        case LOG_LEVEL_DEBUG:
            return BLUE_LOG "DEBUG" END_LOG;
    }
    return "DEFAULT";
}


void log_info(const char * const format, ...) {
    va_list args;
    va_start(args, format);
    UARTprintf("[%s] ", _get_log_level_string(LOG_LEVEL_INFORMATION));
    UARTvprintf(format, args);
    UARTprintf("\n");
    va_end(args);
}

void log_debug(const char * const format, ...) {
    va_list args;
    va_start(args, format);
    UARTprintf("[%s] ", _get_log_level_string(LOG_LEVEL_DEBUG));
    UARTvprintf(format, args);
    UARTprintf("\n");
    va_end(args);
}

void _log_error(const char * const format, ...) {
    va_list args;
    va_start(args, format);
    UARTprintf("[%s] ", _get_log_level_string(LOG_LEVEL_ERROR));
    UARTvprintf(format, args);
    UARTprintf("\n");
    va_end(args);
}
