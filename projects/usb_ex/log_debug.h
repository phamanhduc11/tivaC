#ifndef _LOG_DEBUG_
#define _LOG_DEBUG_
typedef enum
{
    LOG_LEVEL_ERROR,
    LOG_LEVEL_INFORMATION,
    LOG_LEVEL_DEBUG
} LogLevel;

extern LogLevel system_log_level;
void log_error(char const * const format, ...);
void log_info(char const * const format, ...);
void log_debug(char const * const format, ...);
void log_debug_array(const char * cstring, unsigned char *array, unsigned short bcnt);
#endif
