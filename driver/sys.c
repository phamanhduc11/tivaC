#include "INC/sys.h"
void SysClockInit(uint32_t clockConfig) {
    SYSCTL_RCC_REG
}



void PAD_SysPeripheralClockEnable(uint32_t regBase) {
    switch(regBase)
    {
        case SYSCTL_RCGCI2C_ADDR:
            // Only using I2C0 module
            SYSCTL_RCGCI2C_REG |= BIT0;
            break;
        case SYSCTL_RCGCGPIO_ADDR:
            // Only using GPIOB module
            SYSCTL_RCGCGPIO_REG |= BIT1;
            break;
        default:
            //Do nothing
    }
}
