#ifndef _LOG_DEBUG_
#define _LOG_DEBUG_

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
    LOG_LEVEL_ERROR,
    LOG_LEVEL_INFORMATION,
    LOG_LEVEL_DEBUG
} LogLevel;

extern LogLevel system_log_level;
void _log_error(char const * const format, ...);
void log_info(char const * const format, ...);
void log_debug(char const * const format, ...);
void log_debug_array(const char * cstring, unsigned char *array, unsigned short bcnt);

#define log_error(fmt, ...) _log_error("[%d]" fmt, __LINE__, ##__VA_ARGS__) 
#ifdef __cplusplus
}
#endif
#endif
