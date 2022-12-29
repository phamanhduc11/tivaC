#include <stdint.h>
#include <string.h>
#include <INC/i2c.h>
#include <INC/spi.h>
#include <INC/device/eeprom.h>

#include <Global/Include.h>

uint32_t deviceMode = InvalidMode;
uint32_t deviceType = EEPROM_TYPE_INVALID;

static uint8_t eepromBuffer[64]; // will be initialized to 0 by startup code.

void setEEPROMProtocol(uint32_t mode) {
    if (mode < Mode_MAX) {
        deviceMode = mode;
    } else {
        deviceMode = InvalidMode;
    }
}

void setEEPROMType(uint32_t eepromType) {
    if (eepromType < EEPROM_TYPE_MAX) {
        deviceType = eepromType;
    }
    else {
        deviceType = EEPROM_TYPE_INVALID;
    }
}

void eepromWrite(uint32_t writeAddr, uint32_t size, uint8_t *wData) {
    if (deviceMode == I2CMode) {
        // I2C eeprom only allow to write 32 bytes in 1 transfer => Missing
        uint8_t startFrame[2] = { (uint8_t) ((writeAddr & 0x1f00) >> 8), (writeAddr & 0xff)};
        memcpy(eepromBuffer, startFrame, 2);
        memcpy(eepromBuffer+2, wData, size);
        I2C_WriteBytes(AT24C64_ADDR, size+2, eepromBuffer);
    }
    else if (deviceMode == SPIMode) {
        // Check ORG pin of SPI EEPROM device: 0 -> 1 byte , 1 -> 2 bytes
        // below code using 1 byte transfer
        uint8_t startFrame[2] = { (uint8_t) (0x28 | ((writeAddr & 0x700) >> 8)) , (uint8_t) (writeAddr & 0xff)};
        memcpy(eepromBuffer, startFrame, 2);
        memcpy(eepromBuffer + 2, wData, size);
        memset(eepromBuffer + size + 2 , 0x00, 2);
        SPI_WriteBytes(size + size + 2, eepromBuffer);
    }
}

void eepromRead(uint32_t readAddr, uint32_t size, uint8_t *rData) {
    if (deviceMode == I2CMode) {
        uint8_t startFrame[2] = {(uint8_t) ((readAddr & 0x1f00) >> 8), (uint8_t) (readAddr & 0xff)};
        I2C_WriteBytes(AT24C64_ADDR, 2, startFrame);
        I2C_ReadBytes(AT24C64_ADDR, size, rData);
    }
    else if (deviceMode == SPIMode) {
        // Check ORG pin of SPI EEPROM device: 0 -> 1 byte , 1 -> 2 bytes
        // below code using 1 byte transfer
        uint8_t startFrame[2] = { (uint8_t) (0x30 | ((readAddr & 0x700) >> 8)) , (uint8_t) (readAddr & 0xff)};
        memcpy(eepromBuffer, startFrame, 2);
        memset(eepromBuffer + 2, 0x00, 2);
        SPI_WriteBytes(4, startFrame);
        SPI_ReadBytes(size, rData);
    }
}

// Current only for SPI 
void eepromErase(uint32_t addr, uint32_t size) {
    if (deviceMode == SPIMode) {
        uint8_t startFrame[2] = { (uint8_t) (0x30 | ((addr & 0x700) >> 8)) , (uint8_t) (addr & 0xff)};
        SPI_WriteBytes(2, startFrame);
    }
    
}

void eepromEraseAll(void) {
    if (deviceMode == SPIMode) {
    }
}
// 10 011X XXXX XXXX
void eepromWriteEnable(void) {
    if (deviceMode == SPIMode) {
        uint8_t enableFrame[3] = {0x26, 0x0, 0x0};
        SPI_WriteBytes(2, enableFrame);
    }
}
