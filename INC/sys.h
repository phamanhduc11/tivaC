#ifndef __SYS_H__
#define __SYS_H__
#include "Global/Include.h"
// Registers //
#define SYSCTL_BASE 0x400FE000

#define SYSCTL_RCGCI2C_REG          *(unsigned int*)(SYSCTL_BASE + 0x620)   //Inter-Integrated Circuit Run Mode Clock Gating Control
#define SYSCTL_RCGCI2C_ADDR                              (SYSCTL_BASE + 0x620)   //Inter-Integrated Circuit Run Mode Clock Gating Control
#define SYSCTL_RCC_REG              *(unsigned int*)(SYSCTL_BASE + 0x060)     //Run-Mode Clock Configuration
#define SYSCTL_RCC2_REG             *(unsigned int*)(SYSCTL_BASE + 0x070)     //Run-Mode Clock Configuration 2

/*
// Registers Mask //
// SYSCTL_RCC_REG
#define RCC_USERRCC2_MASK   (bitmask(31,31))
#define RCC_DIV400_MASK     (bitmask(30,30))
#define RCC_SYSDIV2_MASK    (bitmask(28,23))
#define RCC_SYSDIV2LSB_MASK (bitmask(22,22))
#define RCC_USBPWRDN_MASK   (bitmask(14,14))
#define RCC_PWRDN2_MASK     (bitmask(13,13))
#define RCC_BYPASS2_MASK    (bitmask(11,11))
#define RCC_OSCSRC2_MASK    (bitmask(6,4))
// End Registers Mask //
*/

// Output
#define RCC_USERRCC2_MASK   0x80000000
#define RCC_DIV400_MASK     0x40000000
#define RCC_SYSDIV2_MASK    0x1F800000
#define RCC_SYSDIV2LSB_MASK 0x00400000
#define RCC_USBPWRDN_MASK   0x00004000
#define RCC_PWRDN2_MASK     0x00002000
#define RCC_BYPASS2_MASK    0x00000800
#define RCC_OSCSRC2_MASK    0x00000070

// APIs
void PAD_SysPeripheralClockEnable(uint32_t regBase);

#endif