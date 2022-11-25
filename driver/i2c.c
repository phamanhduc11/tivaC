#include "INC/i2c.h"
#include "INC/gpio.h"
#include "INC/sys.h"

void I2C_Init(void) {
    PAD_SysPeripheralClockEnable(SYSCTL_RCGCI2C_ADDR);
    
}
