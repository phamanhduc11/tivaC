#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "INC/i2c.h"
#include "INC/gpio.h"
#include "INC/sys.h"
#include "Global/Include.h"

static const uint32_t gI2CDelay = 1;

void I2C_Init(void) {
    uint32_t i2c_clk = 100000;
    // Refer 16.4
    // 1.Initialize Module I2C0
    PAD_SysPeripheralClockEnable(SYSCTL_RCGCI2C_ADDR);
    // 2-5
    PAD_GPIOI2CPinConfig();
    // 6.Initialize I2C Master
    I2CMCR_REG = 0x10;
    // 7.Set SCL clock 100KHz
    I2CMTPR_REG = (uint32_t)(SysCtlClockGet()/(i2c_clk*20)) -1;//7 16MHZ - 11 80MHz
    I2CMCLKOCNT_REG = 0xda;
    // 8.Set Slave address?

    // 9.Set data to be transfer?

    // 10. Set START RUN STOP

    // 11. Wait BUSY bit

    // 12. Check ERROR bit
}

// Master Write
bool I2C_WriteBytes(uint8_t slaveAddress, uint32_t count, uint8_t *data) {
    bool ret = true;
    uint32_t index = 0;
    uint32_t delay = gI2CDelay;

    if (slaveAddress >= 128) {
        ret = false;
        __error__(__FILE__, __LINE__);
        goto exit;
    }
    I2CMSA_REG = (slaveAddress << 1) & 0xfe;
    I2CMDR_REG = data[index++];

    while((I2CMCS_REG & BIT6) && delay) {
        // small delay
    }
    if (delay == 0) {
        __error__(__FILE__, __LINE__);
        ret = false;
        goto exit;
    }

    if (count == 1) {
        // I2CMCS_REG = (I2CMCS_REG & ~0x17) | 0x7;
        I2CMCS_REG = (I2CMCS_REG & ~0x17) | 0x3; // test and worked
    }
    else {
        I2CMCS_REG = (I2CMCS_REG & ~0x17) | 0x3;
    }

    if (count != 1) { // Multibytes
        do 
        {
            delay = gI2CDelay;
            while((I2CMCS_REG & BIT0));
            if ((delay == 0) || (I2CMCS_REG & BIT1)) {
                if ((I2CMCS_REG & BIT4) == 0) {
                    I2CMCS_REG = (I2CMCS_REG & ~0x17) | (BIT2);
                }
                __error__(__FILE__, __LINE__);
                ret = false;
                goto exit;
            }
            I2CMDR_REG = data[index++];
            if (index != count) { // Keep sending
                I2CMCS_REG = (I2CMCS_REG & ~0x17) | BIT0;
            }
            else { // Send last byte and STOP
                I2CMCS_REG = (I2CMCS_REG & ~0x17) | (BIT2 | BIT0);

                delay = gI2CDelay;
                while(--delay);
                while((I2CMCS_REG & BIT0));
                if ((I2CMCS_REG & BIT1)) {
                    __error__(__FILE__, __LINE__);
                    ret = false;
                    goto exit;
                }
            }
        } while (index != count);
    }
    else { // Single byte
        delay = gI2CDelay;
        while(--delay);
        while((I2CMCS_REG & BIT0));
        if ((I2CMCS_REG & BIT1)) {
            ret = false;
            __error__(__FILE__, __LINE__);
            goto exit;
        }
        I2CMCS_REG = (I2CMCS_REG & ~0x17) | BIT2; // test and worked
    }

exit:
    I2CMCS_REG = (I2CMCS_REG & ~0x17) | BIT2; // test and worked
    return ret;
}

// Master Read
bool I2C_ReadBytes(uint32_t slaveAddress, uint32_t count, uint8_t *data) {
    bool ret = true;
    uint32_t index = 0;
    uint32_t delay = gI2CDelay;

    if (slaveAddress >= 128) {
        ret = false;
        goto exit;
    }
    I2CMSA_REG = (slaveAddress << 1) | BIT0;

    while((I2CMCS_REG & BIT6) && delay) {
        // small delay
    }
    if (delay == 0) {
        __error__(__FILE__, __LINE__);
        ret = false;
        goto exit;
    }

    if (count == 1) {
        // I2CMCS_REG = (I2CMCS_REG & ~0x1F) | (BIT2 | BIT1 | BIT0);
        I2CMCS_REG = (I2CMCS_REG & ~0x1F) | (BIT1 | BIT0); // test and worked
    }
    else {
        I2CMCS_REG = (I2CMCS_REG & ~0x1F) | (BIT3 | BIT1 | BIT0);
    }

    if (count != 1) {
        do{
            delay = gI2CDelay;
            while(--delay);
            while((I2CMCS_REG & BIT0));
            if ((I2CMCS_REG & BIT1)) {
                if ((I2CMCS_REG & BIT4) == 0) {
                    I2CMCS_REG = (I2CMCS_REG & ~0x17) | (BIT2);
                }
                __error__(__FILE__, __LINE__);
                ret = false;
                goto exit;
            }

            data[index++] = I2CMDR_REG;
            if (index == (count - 1)) {
                I2CMCS_REG = (I2CMCS_REG & ~0x1f) | (BIT2 | BIT0);
                delay = 8192;
                while(--delay);
                while ((I2CMCS_REG & BIT0));
                if ((I2CMCS_REG & BIT1)) {
                    __error__(__FILE__, __LINE__);
                    ret = false;
                    goto exit;
                }

                data[index++] = I2CMDR_REG;
            }
            else {
                I2CMCS_REG = (I2CMCS_REG & ~0x1f) | (BIT3 | BIT0);
            }
        } while (index != count);
    }
    else {
        delay = gI2CDelay;
        while(--delay);
        while((I2CMCS_REG & BIT0));
        if ((I2CMCS_REG & BIT1)) {
            __error__(__FILE__, __LINE__);
            ret = false;
            goto exit;
        }
        I2CMCS_REG = (I2CMCS_REG & ~0x1F) | (BIT2); // test and worked
        data[index++] = I2CMDR_REG;
    }

exit:
    I2CMCS_REG = (I2CMCS_REG & ~0x1F) | (BIT2); // test and worked
    return ret;
}
