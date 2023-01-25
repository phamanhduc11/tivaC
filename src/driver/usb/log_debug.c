#include "stdint.h"
#include "stdarg.h"
#include "INC/usb/log_debug.h"
#include "utils/uartstdio.h"


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
            return "ERROR";
        case LOG_LEVEL_INFORMATION:
            return "INFO";
        case LOG_LEVEL_DEBUG:
            return "DEBUG";
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

void log_error(const char * const format, ...) {
    va_list args;
    va_start(args, format);
    UARTprintf("[%s] ", _get_log_level_string(LOG_LEVEL_ERROR));
    UARTvprintf(format, args);
    UARTprintf("\n");
    va_end(args);
}
