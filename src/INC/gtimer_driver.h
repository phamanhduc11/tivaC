#ifndef _TIMER_DRIVER_
#define _TIMER_DRIVER_

#ifdef __cplusplus
extern "C"
{
#endif

enum {
    PIOSC_CLOCK,
    SYS_CLOCK,
};

void PAD_SysTickEnable(void);
void PAD_SysTickDisable(void);
void PAD_SysTickIntEnable(void);
void PAD_SysTickIntDisable(void);
void PAD_SysTickClkSrcSet(unsigned int clkSrc);
int PAD_SysTickIsCounting(void);
unsigned int PAD_SysTickCurrentGet(void);
void PAD_SysTickPeriodSet(unsigned int period);
unsigned int PAD_SysTickPeriodGet(void);

#ifdef __cplusplus
}
#endif

#endif
