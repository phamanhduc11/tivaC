#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <Global/Include.h>
#include "TivaWare\utils\uartstdio.h"

#include "INC/debug.h"

static char const * const _get_log_level_string(DebugLevel const log_level)
{
    switch(log_level)
    {
        case DEBUG_LV0:
            return "LV0";
        case DEBUG_LV1:
            return "LV1";
        case DEBUG_LV2:
            return "LV2";
        case DEBUG_LOG:
            return "DEBUG_LOG";
        case DEBUG_WRN:
            return "DEBUG_WRN";
        case DEBUG_ERR:
            return "DEBUG_ERR";
        default:
            return "ERROR LEVEL";
    }
}

DebugInterface::DebugInterface(void) {
    this->debugLevel = DEBUG_WRN;
}

void DebugInterface::setDebugLevel(DebugLevel level) {
    if (level >= this->debugLevel && level < DEBUG_MAX_OPT) {
        this->debugLevel = level;
    }
}

void DebugInterface::log(DebugLevel level, const char *pcString, ...) {
    va_list vaArgP;
    va_start(vaArgP, pcString);
    if(level >= this->debugLevel) {
        UARTprintf("[%s]", _get_log_level_string(level));
        UARTvprintf(pcString, vaArgP);
        UARTprintf("\r\n");
    }
    va_end(vaArgP);

}

DebugInterface SystemDebug;