#ifndef _EEPROM_H_
#define _EEPROM_H_
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

enum eepromTransferMode {
  I2CMode,
  SPIMode,
  InvalidMode=0xff,
};

void eepromWrite(uint32_t writeAddr, uint32_t size, uint8_t *wData);
void eepromRead(uint32_t readAddr, uint32_t size, uint8_t *rData);

#ifdef __cplusplus
}
#endif

#endif