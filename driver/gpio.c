#include "Global/Include.h"
#include "INC/sys.h"
#include "INC/gpio.h"

void PAD_GPIOI2CPinConfig(void) {
    uint32_t regTemp = 0;
    // Set Pin SDA - SCL => PB3 - PB2
    
    // Enable GPIOB Clock
    PAD_SysPeripheralClockEnable(SYSCTL_RCGCGPIO_ADDR);
    // Enable alternate function, value 3
    ///- Set pin to use alternate function
    REGW(GPIOPB_APB_BASE + GPIOAFSEL_REG_OFF, REGR(GPIOPB_APB_BASE + GPIOAFSEL_REG_OFF) | BIT2 | BIT3);
    ///- Set open-drain to SDA
    REGW(GPIOPB_APB_BASE + GPIOODR_REG_OFF,REGR(GPIOPB_APB_BASE + GPIOODR_REG_OFF) | BIT3);
    ///- Set digital enable to SDA and SCL
    REGW(GPIOPB_APB_BASE + GPIODEN_REG_OFF,REGR(GPIOPB_APB_BASE + GPIODEN_REG_OFF) | BIT2 | BIT3);
    //? Missing GPIO Pad
    REGW(GPIOPB_APB_BASE + GPIODR2R_REG_OFF, REGR(GPIOPB_APB_BASE + GPIODR2R_REG_OFF) | BIT2 | BIT3);
    ///- Set alter function to I2C
    regTemp = REGR(GPIOPB_APB_BASE + GPIOPCTL_REG_OFF);
    SET_MASK_VAL(regTemp, 0xf00, 3);
    SET_MASK_VAL(regTemp, 0x0f0, 3);
    REGW(GPIOPB_APB_BASE + GPIOPCTL_REG_OFF, regTemp);
}
