#include "Global/Include.h"
#include "INC/sys.h"
#include "INC/gpio.h"

void PAD_GPIOI2CPinConfig(void) {
    uint32_t regTemp = 0;
    // Set Pin SDA - SCL => PB3 - PB2
    
    // Enable GPIOB Clock
    PAD_SysPeripheralClockEnable(SYSCTL_RCGCGPIO_ADDR, eModule_1);
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

void PAD_GPIOSSIPinConfig(void) {
    uint32_t regTemp = 0;
    // Set Pin CS - CLK - MISO - MOSI => PA3 - PA2 - PA4 - PA5
    
    // Enable GPIOA Clock
    PAD_SysPeripheralClockEnable(SYSCTL_RCGCGPIO_ADDR, eModule_0);
    // Enable alternate function, value 2
    ///- Set pin to use alternate function
    REGW(GPIOPA_APB_BASE + GPIOAFSEL_REG_OFF, REGR(GPIOPA_APB_BASE + GPIOAFSEL_REG_OFF) | BIT2 | BIT3 | BIT4 | BIT5);
    ///- Set digital enable to all coresponding pin
    REGW(GPIOPA_APB_BASE + GPIODEN_REG_OFF,REGR(GPIOPA_APB_BASE + GPIODEN_REG_OFF) | BIT2 | BIT3 | BIT4 | BIT5);
    //? Missing GPIO Pad
    REGW(GPIOPA_APB_BASE + GPIODR2R_REG_OFF, REGR(GPIOPA_APB_BASE + GPIODR2R_REG_OFF) | BIT2 | BIT3 | BIT | BIT5);
    ///- Set alter function to I2C
    regTemp = REGR(GPIOPA_APB_BASE + GPIOPCTL_REG_OFF);
    SET_MASK_VAL(regTemp, 0xf00, 2);
    SET_MASK_VAL(regTemp, 0xf000, 2);
    SET_MASK_VAL(regTemp, 0xf0000, 2);
    SET_MASK_VAL(regTemp, 0xf00000, 2);
    REGW(GPIOPA_APB_BASE + GPIOPCTL_REG_OFF, regTemp);
}
