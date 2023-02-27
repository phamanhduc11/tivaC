#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

#include "utils/uartstdio.h"

#include "Global/Include.h"
#include "INC/debug.h"


void __error__(char *pcFilename, uint32_t ui32Line)
{
    UARTprintf("[DEBUG] %s:%d\r\n", pcFilename, ui32Line);
//    while(1);
}

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
            return "LOG";
        case DEBUG_WRN:
            return "WRN";
        case DEBUG_ERR:
            return "ERR";
        default:
            return "ERROR LEVEL";
    }
}

DebugInterface::DebugInterface(void) {
    this->debugLevel = DEBUG_WRN;
}

void DebugInterface::setDebugLevel(DebugLevel level) {
    if (level < DEBUG_MAX_OPT) {
        this->debugLevel = level;
    }
}

void DebugInterface::log(DebugLevel level, const char *pcString, ...) {
    va_list vaArgP;
    va_start(vaArgP, pcString);
    if(level >= this->debugLevel) {
        UARTprintf("[%s]", _get_log_level_string(level));

        switch (level)
        {
        case DEBUG_WRN:
            UARTprintf(YELLOW_LOG);
            break;

        case DEBUG_ERR:
            UARTprintf(RED_LOG);
            break;

        default:
            break;
        }

        UARTvprintf(pcString, vaArgP);
        if (level >= DEBUG_WRN) {
            UARTprintf(END_LOG);
        }
        UARTprintf("\r\n");
    }
    va_end(vaArgP);

}

DebugInterface SystemDebug;