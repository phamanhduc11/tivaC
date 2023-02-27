#include "INC/gtimer_driver.h"
/* Systick Timer */
// REGISTER
#define SYSTEM_BASE     0xE000E000
#define STICK_STCTRL    *(unsigned int*) (SYSTEM_BASE + 0x10)
#define STICK_STRELOAD  *(unsigned int*) (SYSTEM_BASE + 0x14)
#define STICK_STCURRENT *(unsigned int*) (SYSTEM_BASE + 0x18)

// REG_BIT_MASK
#define STCTRL_COUNT    0x100
#define STCTRL_CLKSRC   0x4
#define STCTRL_INTEN    0x2
#define STCTRL_ENABLE   0x1

void PAD_SysTickEnable(void) {
    STICK_STCTRL |= STCTRL_ENABLE;
}

void PAD_SysTickDisable(void) {
    STICK_STCTRL &= ~STCTRL_ENABLE;
}

void PAD_SysTickIntEnable(void) {
    STICK_STCTRL |= STCTRL_INTEN;
}

void PAD_SysTickIntDisable(void) {
    STICK_STCTRL &= ~STCTRL_INTEN;
}

void PAD_SysTickClkSrcSet(unsigned int clkSrc) {
    STICK_STCTRL &= ~STCTRL_CLKSRC;
    switch (clkSrc)
    {
    case PIOSC_CLOCK:
        // do nothing
        break;
    case SYS_CLOCK:
        STICK_STCTRL |= STCTRL_CLKSRC;
        break;
    default:
        break;
    }
}

int PAD_SysTickIsCounting(void) {
    int ret = 0;
    ret = (STICK_STCTRL & STCTRL_COUNT) != 0 ? 1 : 0;
    return ret;
}

unsigned int PAD_SysTickCurrentGet(void) {
    return STICK_STCURRENT;
}

void PAD_SysTickPeriodSet(unsigned int period) {
    STICK_STRELOAD = period;
}

unsigned int PAD_SysTickPeriodGet(void) {
    return STICK_STRELOAD;
}
/* Systick End */