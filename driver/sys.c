#include <stdio.h>
#include <stdint.h>
#include "Global/Include.h"
#include "INC/sys.h"

/*********************************
 * \brief Set system clock to 80Mhz,
 * Refer to 5.3 Document
 * 
 * \return None.
 * 
 ********************************/
void PAD_SysClockSet(void) {
#if defined(gcc)
    uint32_t u32Delay = 0;
    // uint32_t u32RCC = SYSCTL_RCC_REG;
    // uint32_t u32RCC2 = SYSCTL_RCC2_REG;

    // Clear PLL status in Interrupt register
    SYSCTL_MISC_REG |= (BIT6);

    // Bypass PLL and system clock divider
    SET_MASK_VAL(SYSCTL_RCC_REG, RCC_BYPASS_MASK, RCC_BYPASS_SET);
    SET_MASK_VAL(SYSCTL_RCC2_REG, RCC2_BYPASS2_MASK, RCC_BYPASS_SET);
    SET_MASK_VAL(SYSCTL_RCC_REG, RCC_USESYSDIV_MASK, RCC_USESYSDIV_CLR);

    // Select XTAL and OSC source and enable PLL
    SET_MASK_VAL(SYSCTL_RCC2_REG, RCC2_USERRCC2_MASK, 0x1);

    SET_MASK_VAL(SYSCTL_RCC_REG, RCC_XTAL_MASK, RCC_XTAL_16_M);
    SET_MASK_VAL(SYSCTL_RCC_REG, RCC_OSCSRC_MASK, RCC_OSCSRC_MAIN);
    SET_MASK_VAL(SYSCTL_RCC2_REG, RCC2_OSCSRC2_MASK, RCC_OSCSRC_MAIN);
    SET_MASK_VAL(SYSCTL_RCC_REG, RCC_PWRDN_MASK, RCC_PWRDN_CLR);
    SET_MASK_VAL(SYSCTL_RCC2_REG, RCC2_PWRDN2_MASK, RCC_PWRDN_CLR);

    //Set SYSDIV and USESYSDIV
    SET_MASK_VAL(SYSCTL_RCC_REG, RCC_SYSDIV_MASK, RCC_SYSDIV_4);
    SET_MASK_VAL(SYSCTL_RCC2_REG, RCC2_DIV400_MASK, 0x1);
    SET_MASK_VAL(SYSCTL_RCC2_REG, RCC2_SYSDIV2_MASK, 0x2);
    SET_MASK_VAL(SYSCTL_RCC2_REG, RCC2_SYSDIV2LSB_MASK, 0x0);

    SET_MASK_VAL(SYSCTL_RCC_REG, RCC_USESYSDIV_MASK, RCC_USESYSDIV_SET);

    // Wait PLL lock
    for (u32Delay = 0x8000; u32Delay == 0; u32Delay--){
        // if(SYSCTL_PLLSTAT_REG)
        if(SYSCTL_RIS_REG & BIT6)
        {
            break;
        }
    }

    // Enable PLL by clear BYPASS
    SET_MASK_VAL(SYSCTL_RCC_REG, RCC_BYPASS_MASK, RCC_BYPASS_CLR);
    SET_MASK_VAL(SYSCTL_RCC2_REG, RCC2_BYPASS2_MASK, RCC_BYPASS_CLR);
#endif
}



/*********************************
 * \brief Enable Peripheral Clock,
 * \param regBase Enable Module Clock register address
 * \param eModule Choose which module to enable
 * \return None.
 * 
 ********************************/
void PAD_SysPeripheralClockEnable(uint32_t regBase, uint32_t eModule) {
    switch(regBase)
    {
        case SYSCTL_RCGCI2C_ADDR:
            SYSCTL_RCGCI2C_REG |= 1 << eModule;
            break;
        case SYSCTL_RCGCGPIO_ADDR:
            SYSCTL_RCGCGPIO_REG |= 1 << eModule;
            break;
        case SYSCTL_RCGCSSI_ADDR:
            SYSCTL_RCGCSSI_REG |= 1 << eModule;
        default:
            break;
            //Do nothing
    }
}
