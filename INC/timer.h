#ifndef __TIMER_H__
#define __TIMER_H__
#include <stdint.h>

#include "Global/Include.h"
typedef enum {
    GPT_Module_0,
    GPT_Module_1,
    GPT_Module_2,
    GPT_Module_3,
    GPT_Module_4,
    GPT_Module_5,
    GPTW_Module_0,
    GPTW_Module_1,
    GPTW_Module_2,
    GPTW_Module_3,
    GPTW_Module_4,
    GPTW_Module_5,
    GPT_MAX,
} eGPTMModule;

typedef enum {
    GPTM_MODE_ONESHOT,
    GPTM_MODE_PERIOD,
    GPTM_MODE_RTC,
    GPTM_MODE_EDGE_COUNT,
    GPTM_MODE_EDGE_TIME,
    GPTM_MODE_MODE_MAX
} eGPTMMode;

typedef enum {
    GPTM_INT_TA_TIMEOUT = BIT0,
    GPTM_INT_TA_CAPTURE_MATCH = BIT1,
    GPTM_INT_TA_CAPTURE_EVENT = BIT2,
    GPTM_INT_RTC = BIT3,
    GPTM_INT_TA_MATCH = BIT4,
    GPTM_INT_TB_TIMEOUT = BIT8,
    GPTM_INT_TB_CAPTURE_MATCH = BIT9,
    GPTM_INT_TB_CAPTURE_EVENT = BIT10,
    GPTM_INT_TB_MATCH = BIT11,
    GPTM_INT_WTIMER_WRITEUPDATE_ERR = BIT16,
    GPTM_INT_MAX
} eGPTMInterrupt;

class TimerModule {
    private:
        const char *moduleName;
        static uint16_t isInitialized;
        bool isUsable;
        uint32_t baseModuleAddr;
        eGPTMMode mode;
        uint32_t tickMs;
        void initialize(void);
    public:
        TimerModule(eGPTMModule module, eGPTMMode mode, uint32_t tickPerSec);
        void setTime(uint32_t ms);
        void enable(void);
        void disable(void);
        uint32_t getIntStatus(void);
        void clearInt(uint32_t clearFlags);
        void setTimerBreakpointDebug(bool isOn);
};

#endif
