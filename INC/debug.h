#include <stdint.h>

#define RED_LOG     "\033[31m"
#define GREEN_LOG   "\033[32m"
#define BLUE_LOG    "\033[34m"
#define END_LOG     "\033[0m"


typedef enum {
    DEBUG_LV0,
    DEBUG_LV1,
    DEBUG_LV2,
    DEBUG_LOG,
    DEBUG_WRN,
    DEBUG_ERR,
    DEBUG_MAX_OPT,
} DebugLevel;


class DebugInterface {
    private:
        uint32_t debugLevel;
    public:
        void setDebugLevel(DebugLevel level);
        DebugInterface(void);
        void log(DebugLevel level, const char *pcString, ...);
};


extern DebugInterface SystemDebug;