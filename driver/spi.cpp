#include <stdint.h>
#include <stdbool.h>

#include "driverlib/sysctl.h"

#include <Global/Include.h>
#include <INC/spi.h>
#include <INC/debug.h>
#include <INC/gpio.h>
#include <INC/sys.h>

#define SPI_STR   "SPI"

#define SPI_BASE  SPI0_BASE
#define SPI0_BASE 0x40008000
#define SPI1_BASE 0x40009000
#define SPI2_BASE 0x4000A000
#define SPI3_BASE 0x4000B000


#define SPI_SSICR0         *(volatile unsigned int*) (SPI_BASE + 0x000)        // RW    0x0000.0000 SSI Control 0
#define SPI_SSICR1         *(volatile unsigned int*) (SPI_BASE + 0x004)        // RW    0x0000.0000 SSI Control 1
#define SPI_SSIDR          *(volatile unsigned int*) (SPI_BASE + 0x008)        // RW    0x0000.0000 SSI Data
#define SPI_SSISR          *(volatile unsigned int*) (SPI_BASE + 0x00C)        // RO    0x0000.0003 SSI Status
#define SPI_SSICPSR        *(volatile unsigned int*) (SPI_BASE + 0x010)        // RW    0x0000.0000 SSI Clock Prescale
#define SPI_SSIIM          *(volatile unsigned int*) (SPI_BASE + 0x014)        // RW    0x0000.0000 SSI Interrupt Mask
#define SPI_SSIRIS         *(volatile unsigned int*) (SPI_BASE + 0x018)        // RO    0x0000.0008 SSI Raw Interrupt Status
#define SPI_SSIMIS         *(volatile unsigned int*) (SPI_BASE + 0x01C)        // RO    0x0000.0000 SSI Masked Interrupt Status
#define SPI_SSIICR         *(volatile unsigned int*) (SPI_BASE + 0x020)        // W1C   0x0000.0000 SSI Interrupt Clear
#define SPI_SSIDMACTL      *(volatile unsigned int*) (SPI_BASE + 0x024)        // RW    0x0000.0000 SSI DMA Control
#define SPI_SSICC          *(volatile unsigned int*) (SPI_BASE + 0xFC8)        // RW    0x0000.0000 SSI Clock Configuration
#define SPI_SSIPeriphID4   *(volatile unsigned int*) (SPI_BASE + 0xFD0)        // RO    0x0000.0000 SSI Peripheral Identification 4
#define SPI_SSIPeriphID5   *(volatile unsigned int*) (SPI_BASE + 0xFD4)        // RO    0x0000.0000 SSI Peripheral Identification 5
#define SPI_SSIPeriphID6   *(volatile unsigned int*) (SPI_BASE + 0xFD8)        // RO    0x0000.0000 SSI Peripheral Identification 6
#define SPI_SSIPeriphID7   *(volatile unsigned int*) (SPI_BASE + 0xFDC)        // RO    0x0000.0000 SSI Peripheral Identification 7
#define SPI_SSIPeriphID0   *(volatile unsigned int*) (SPI_BASE + 0xFE0)        // RO    0x0000.0022 SSI Peripheral Identification 0
#define SPI_SSIPeriphID1   *(volatile unsigned int*) (SPI_BASE + 0xFE4)        // RO    0x0000.0000 SSI Peripheral Identification 1
#define SPI_SSIPeriphID2   *(volatile unsigned int*) (SPI_BASE + 0xFE8)        // RO    0x0000.0018 SSI Peripheral Identification 2
#define SPI_SSIPeriphID3   *(volatile unsigned int*) (SPI_BASE + 0xFEC)        // RO    0x0000.0001 SSI Peripheral Identification 3
#define SPI_SSIPCellID0    *(volatile unsigned int*) (SPI_BASE + 0xFF0)        // RO    0x0000.000D SSI PrimeCell Identification 0
#define SPI_SSIPCellID1    *(volatile unsigned int*) (SPI_BASE + 0xFF4)        // RO    0x0000.00F0 SSI PrimeCell Identification 1
#define SPI_SSIPCellID2    *(volatile unsigned int*) (SPI_BASE + 0xFF8)        // RO    0x0000.0005 SSI PrimeCell Identification 2
#define SPI_SSIPCellID3    *(volatile unsigned int*) (SPI_BASE + 0xFFC)        // RO    0x0000.00B1 SSI PrimeCell Identification 3

static uint32_t getSPIAddr(SPI_DEV device) {
  switch (device) {
    case SPI0_DEV:
      return SPI0_BASE;
    case SPI1_DEV:
      return SPI1_BASE;
    case SPI2_DEV:
      return SPI2_BASE;
    case SPI3_DEV:
      return SPI3_BASE;
    default:
      return 0xffffffff;
  }
}

bool SPI_WriteBytes(uint32_t count, uint8_t *data) {
  return false;
}

bool SPI_ReadBytes(uint32_t count, uint8_t *data) {
  return false;
}

// class SPIInterface {
//         static uint8_t isInitialized;
//         const uint8_t *deviceName;
//         bool isUsable;
//         uint8_t GPIOInit(SPI_DEV eModule);
//         uint8_t moduleInitialize(SPI_DEV eModule);
//     public:
//         SPIInterface(SPI_DEV device);
//         uint8_t write(uint32_t wSize, uint8_t *wBuffer);
//         uint8_t read(uint32_t rSize, uint8_t *rBuffer);
// };

uint8_t SPIInterface::isInitialized = 0;

SPIInterface::SPIInterface(SPI_DEV device, SPI_Mode transMode, uint32_t setClock, uint8_t bitSize) {
  uint8_t status = 0xff;
  uint32_t devBaseAddr = 0xfffffff;
  this->deviceName  = SPI_STR;
  this->isUsable    = false;
  this->setClock    = setClock;

  if(device & this->isInitialized) {
    SystemDebug.log(DEBUG_WRN, "%s %d module is already initialized and used!!", this->deviceName, device);
    return;
  }

  devBaseAddr = getSPIAddr(device);
  status = this->moduleInitialize(device);
  if(status) {
    SystemDebug.log(DEBUG_WRN, "%s %d module initialize failed: errno %d", this->deviceName, device, status);
    return;
  }

  status = this->frameSupport(transMode, bitSize);
  if(status) {
    SystemDebug.log(DEBUG_WRN, "%s %d frame init: errno %d", this->deviceName, device, status);
    return;
  }

}

uint8_t SPIInterface::moduleInitialize(SPI_DEV eModule) {
  uint8_t status = 0xff;
  // 1 Enable SSI Clock
  PAD_SysPeripheralClockEnable(SYSCTL_RCGCSSI_ADDR, eModule_0);
  // 2 -> 5 Config SSI pin on port A
  PAD_GPIOSSIPinConfig();
  // 3 Frame support
  
  return 0;
}

// SPO Clock Polarity Bit
// 0 : _____-_-_____ Low steady
// 1 : -----_-_----- High steady

// SPH Phase Control Bit
// 0 : Capture Data on first clock edge transition
// 1 : Capture Data on second clock edge transition

static uint8_t getFrameModeValue(SPI_Mode mode) {
  switch(mode) {
    case SPI_Mode_0:
      return 0b00;
    case SPI_Mode_1:
      return 0b01;
    case SPI_Mode_2:
      return 0b10;
    case SPI_Mode_3:
      return 0b11;
    default:
      return 0x0;
  }
} 

uint8_t SPIInterface::frameSupport(SPI_Mode transMode, uint8_t bitSize) {
  uint32_t tempVal = 0;
  // Refer 15.4 Frame formats
  // 1 - Ensure SSE Bit is clear
  SPI_SSICR1 &= (SPI_SSICR1 & (~BIT1));
  // 2 - Set SSI Master
  SPI_SSICR1 = 0;
  // 3 - Config SSI clock source
  SPI_SSICC = 4;
  // 4 - Configure clock prescaler 
  SPI_SSICPSR = 4;
  // 5 - Configure SSICR0 -> clock, frame mode, data size
  tempVal = (SysCtlClockGet()/(this->setClock*SPI_SSICPSR)) << 8;
  tempVal |= getFrameModeValue(transMode) << 6;
  tempVal |= 0 << 4; // force to use Freescale SPI Frame Format
  tempVal |= bitSize - 1;
  SPI_SSICR0 = tempVal;
  // 6 - Optional uDMA
  // 7 - Enable SSI
  SPI_SSICR1 |= BIT1;
  //
  return 0;
}


uint8_t SPIInterface::write(uint32_t wSize, uint8_t *wBuffer) {
  return 0;
}

uint8_t SPIInterface::read(uint32_t rSize, uint8_t *rBuffer) {
  return 0;
}
