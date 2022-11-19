#ifndef _TIMER_DRIVER_
#define _TIMER_DRIVER_

enum {
    PIOSC_CLOCK,
    SYS_CLOCK,
};

void SysTickEnable(void);
void SysTickDisable(void);
void SysTickIntEnable(void);
void SysTickIntDisable(void);
void SysTickClkSrcSet(unsigned int clkSrc);
int SysTickIsCounting(void);
unsigned int SysTickCurrentGet(void);
void SysTickPeriodSet(unsigned int period);
unsigned int SysTickPeriodGet(void);
#endif
