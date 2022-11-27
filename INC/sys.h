#ifndef __SYS_H__
#define __SYS_H__
#include <stdint.h>
#include "Global/Include.h"
// Register Map//
#define SYSCTL_BASE 0x400FE000

#define SYSCTL_RCC_REG              *(unsigned int*)(SYSCTL_BASE + 0x060)   //Run-Mode Clock Configuration
#define SYSCTL_RCC2_REG             *(unsigned int*)(SYSCTL_BASE + 0x070)   //Run-Mode Clock Configuration 2
#define SYSCTL_RCGCI2C_REG          *(unsigned int*)(SYSCTL_BASE + 0x620)   //Inter-Integrated Circuit Run Mode Clock Gating Control
#define SYSCTL_RCGCI2C_ADDR         (SYSCTL_BASE + 0x620)                   //Inter-Integrated Circuit Run Mode Clock Gating Control
#define SYSCTL_RCGCGPIO_REG         *(unsigned int*)(SYSCTL_BASE + 0x608)   //General-Purpose Input/Output Run Mode Clock Gating Control
#define SYSCTL_RCGCGPIO_ADDR        (SYSCTL_BASE + 0x608)                   //General-Purpose Input/Output Run Mode Clock Gating Control
#define SYSCTL_PLLSTAT_REG          *(unsigned int*)(SYSCTL_BASE + 0x168)   //PLL Status
#define SYSCTL_RIS_REG              *(unsigned int*)(SYSCTL_BASE + 0x050)   //Raw Interrupt Status
#define SYSCTL_MISC_REG             *(unsigned int*)(SYSCTL_BASE + 0x058)   //Masked Interrupt Status and Clear

/*
// Registers Mask //

// SYSCTL_RCC_REG
#define RCC_ACG_MASK        (bitmask(27,27))
#define RCC_SYSDIV_MASK     (bitmask(26,23))
#define RCC_USESYSDIV_MASK  (bitmask(22,22))
#define RCC_USEPWMDIV_MASK  (bitmask(20,20))
#define RCC_PWMDIV_MASK     (bitmask(19,17))
#define RCC_PWRDN_MASK      (bitmask(13,13))
#define RCC_BYPASS_MASK     (bitmask(11,11))
#define RCC_XTAL_MASK       (bitmask(10,6))
#define RCC_OSCSRC_MASK     (bitmask(5,4))
#define RCC_MOSCDIS_MASK    (bitmask(3,1))

// SYSCTL_RCC2_REG
#define RCC2_USERRCC2_MASK   (bitmask(31,31))
#define RCC2_DIV400_MASK     (bitmask(30,30))
#define RCC2_SYSDIV2_MASK    (bitmask(28,23))
#define RCC2_SYSDIV2LSB_MASK (bitmask(22,22))
#define RCC2_USBPWRDN_MASK   (bitmask(14,14))
#define RCC2_PWRDN2_MASK     (bitmask(13,13))
#define RCC2_BYPASS2_MASK    (bitmask(11,11))
#define RCC2_OSCSRC2_MASK    (bitmask(6,4))

// End Registers Mask //
*/

// Output
// SYSCTL_RCC_REG
#define RCC_ACG_MASK        0x08000000
#define RCC_SYSDIV_MASK     0x07800000
#define RCC_USESYSDIV_MASK  0x00400000
#define RCC_USEPWMDIV_MASK  0x00100000
#define RCC_PWMDIV_MASK     0x000E0000
#define RCC_PWRDN_MASK      0x00002000
#define RCC_BYPASS_MASK     0x00000800
#define RCC_XTAL_MASK       0x000007C0
#define RCC_OSCSRC_MASK     0x00000030
#define RCC_MOSCDIS_MASK    0x0000000E

// SYSCTL_RCC2_REG
#define RCC2_USERRCC2_MASK   0x80000000
#define RCC2_DIV400_MASK     0x40000000
#define RCC2_SYSDIV2_MASK    0x1F800000
#define RCC2_SYSDIV2LSB_MASK 0x00400000
#define RCC2_USBPWRDN_MASK   0x00004000
#define RCC2_PWRDN2_MASK     0x00002000
#define RCC2_BYPASS2_MASK    0x00000800
#define RCC2_OSCSRC2_MASK    0x00000070

// Register Value
// SYSCTL_RCC_REG
#define RCC_ACG_EN      0x1
#define RCC_SYSDIV_1    0x0
#define RCC_SYSDIV_2    0x1
#define RCC_SYSDIV_3    0x2
#define RCC_SYSDIV_4    0x3
#define RCC_SYSDIV_5    0x4
#define RCC_SYSDIV_6    0x5
#define RCC_SYSDIV_7    0x6
#define RCC_SYSDIV_8    0x7
#define RCC_SYSDIV_9    0x8
#define RCC_SYSDIV_10   0x9
#define RCC_SYSDIV_11   0xA
#define RCC_SYSDIV_12   0xB
#define RCC_SYSDIV_13   0xC
#define RCC_SYSDIV_14   0xD
#define RCC_SYSDIV_15   0xE
#define RCC_SYSDIV_16   0xF
#define RCC_USESYSDIV_SET 0x1
#define RCC_USESYSDIV_CLR 0x0
#define RCC_PWMDIV_2    0x0
#define RCC_PWMDIV_4    0x1
#define RCC_PWMDIV_8    0x2
#define RCC_PWMDIV_16   0x3
#define RCC_PWMDIV_32   0x4
#define RCC_PWMDIV_64   0x7
#define RCC_PWRDN_SET   0x1
#define RCC_PWRDN_CLR   0x0
#define RCC_BYPASS_SET  0x1
#define RCC_BYPASS_CLR  0x0
#define RCC_XTAL_4_M        0x6
#define RCC_XTAL_4_096_M    0x7
#define RCC_XTAL_4_9152_M   0x8
#define RCC_XTAL_5_M        0x9
#define RCC_XTAL_5_12_M     0xA
#define RCC_XTAL_6_M        0xB
#define RCC_XTAL_6_144_M    0xC
#define RCC_XTAL_7_3728_M   0xD
#define RCC_XTAL_8_M        0xE
#define RCC_XTAL_8_192_M    0xF
#define RCC_XTAL_10_M       0x10
#define RCC_XTAL_12_M       0x11
#define RCC_XTAL_12_288_M   0x12
#define RCC_XTAL_13_56_M    0x13
#define RCC_XTAL_14_31818_M 0x14
#define RCC_XTAL_16_M       0x15
#define RCC_XTAL_16_384_M   0x16
#define RCC_XTAL_18_M       0x17
#define RCC_XTAL_20_M       0x18
#define RCC_XTAL_24_M       0x19
#define RCC_XTAL_25_M       0x1A
#define RCC_OSCSRC_MAIN     0x0
#define RCC_OSCSRC_PRE      0x1
#define RCC_OSCSRC_PRE_DIV_4 0x2
#define RCC_OSCSRC_LOW_FREQ 0x3
#define RCC_MOSCDIS_SET     0x1
#define RCC_MOSCDIS_CLR     0x0




// SYSCTL_RCC2_REG

// APIs
void PAD_SysPeripheralClockEnable(uint32_t regBase);
void PAD_SysClockSet(void);
#endif
