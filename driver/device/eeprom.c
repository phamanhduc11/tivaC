#include <stdint.h>
#include <string.h>
#include <INC/i2c.h>
#include <INC/spi.h>

#include <Global/Include.h>
// AT24C32/64
#define AT24C64_ADDR 0x50

uint8_t deviceMode = InvalidMode;

static uint8_t eepromBuffer[64]; // will be initialized to 0 by startup code.

void eepromWrite(uint32_t writeAddr, uint32_t size, uint8_t *wData) {
    if (deviceMode == I2CMode) {
        // I2C eeprom only allow to write 32 bytes in 1 transfer => Missing
        uint8_t startFrame[2] = {(writeAddr & 0x1f00) >> 8, (writeAddr & 0xff)};
        memcpy(eepromBuffer, startFrame, 2);
        memcpy(eepromBuffer+2, wData, size);
        I2C_WriteBytes(AT24C64_ADDR, size+2, eepromBuffer);
    }
    else if (deviceMode == SPIMode) {
        // Check ORG pin of SPI EEPROM device: 0 -> 1 byte , 1 -> 2 bytes
        // below code using 1 byte transfer
        uint8_t startFrame[2] = { 0x28 | ((writeAddr & 0x700) >> 8) , (writeAddr & 0xff)};
        memcpy(eepromBuffer, startFrame, 2);
        memcpy(eeprombuffer+2, wData, size);
        SPI_WriteBytes(size + 2, eepromBuffer);
    }
}

void eepromRead(uint32_t readAddr, uint32_t size, uint8_t *rData) {
    if (deviceMode == I2CMode) {
        uint8_t startFrame[2] = {(readAddr & 0x1f00) >> 8, (readAddr & 0xff)};
        I2C_WriteBytes(AT24C64_ADDR, 2, startFrame);
        I2C_ReadBytes(AT24C64_ADDR, size, rData);
    }
    else if (deviceMode == SPIMode) {
        // Check ORG pin of SPI EEPROM device: 0 -> 1 byte , 1 -> 2 bytes
        // below code using 1 byte transfer
        uint8_t startFrame[2] = { 0x30 | ((readAddr & 0x700) >> 8) , (readAddr & 0xff)};
        SPI_WriteBytes(2, startFrame);
        SPI_ReadBytes(size, rData);
    }
}
