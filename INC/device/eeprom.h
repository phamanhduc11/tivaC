#ifndef _EEPROM_H_
#define _EEPROM_H_
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

// AT24C32/64
#define AT24C64_ADDR 0x50

enum eepromTransferMode {
  I2CMode,
  SPIMode,
  Mode_MAX,
  InvalidMode=0xff,
};

void eepromWrite(uint32_t writeAddr, uint32_t size, uint8_t *wData);
void eepromRead(uint32_t readAddr, uint32_t size, uint8_t *rData);
void setEEPROMProtocol(uint32_t mode);

#ifdef __cplusplus
}
#endif

#endif
