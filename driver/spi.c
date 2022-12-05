#include <stdint.h>
#include <stdbool.h>

#include <Global/Include.h>
#include <INC/spi.h>

// SPO Clock Polarity Bit
// 0 : _____-_-_____ Low steady
// 1 : -----_-_----- High steady

// SPH Phase Control Bit
// 0 : Capture Data on first clock edge transition
// 1 : Capture Data on second clock edge transition


bool SPI_WriteBytes(uint32_t count, uint8_t *data) {
  return false;
}

bool SPI_ReadBytes(uint32_t count, uint8_t *data) {
  return false;
}
