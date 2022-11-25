#include "INC/i2c.h"
#include "INC/gpio.h"
#include "INC/sys.h"

void I2C_Init(void) {
    // Refer 16.4
    // 1.Initialize Module I2C0
    PAD_SysPeripheralClockEnable(SYSCTL_RCGCI2C_ADDR);
    // 2-5
    PAD_GPIOI2CPinConfig();
    
}
