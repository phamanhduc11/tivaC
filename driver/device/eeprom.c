#include <stdint.h>
#include <string.h>
#include <INC/i2c.h>

#include <Global/Include.h>
// AT24C32/64
#define AT24C64_ADDR 0x50

static uint8_t eepromBuffer[64]; // will be initialized to 0 by startup code.

void eepromWrite(uint32_t writeAddr, uint32_t size, uint8_t *wData) {
    uint8_t startFrame[2] = {(writeAddr & 0x1f00) >> 8, (writeAddr & 0xff)};
    memcpy(eepromBuffer, startFrame, 2);
    memcpy(eepromBuffer+2, wData, size);
    I2C_WriteBytes(AT24C64_ADDR, size+2, eepromBuffer);
}

void eepromRead(uint32_t readAddr, uint32_t size, uint8_t *rData) {
    uint8_t startFrame[2] = {(readAddr & 0x1f00) >> 8, (readAddr & 0xff)};
    I2C_WriteBytes(AT24C64_ADDR, 2, startFrame);
    I2C_ReadBytes(AT24C64_ADDR, size, rData);
}
