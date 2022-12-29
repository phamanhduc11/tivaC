#include <stdint.h>
#include <stdbool.h>

#include "driverlib/sysctl.h"
#include "driverlib/debug.h"

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

static uint32_t getSPIAddr(eSPI_DEV device) {
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

extern SPIInterface *spi;
void SPI_WriteBytes(uint32_t count, uint8_t *data) {
  spi->write(count, data);
}

void SPI_ReadBytes(uint32_t count, uint8_t *data) {
  spi->read(count, data);
}

static volatile uint16_t bugBuffer[1024] = {0,};
static volatile uint16_t fakeIndex = 0;
uint8_t SPIInterface::isInitialized = 0;

SPIInterface::SPIInterface(eSPI_DEV device, eSPI_Mode transMode, uint32_t setClock, uint8_t bitSize) {
  uint8_t status = 0xff;
  uint32_t devBaseAddr = 0xfffffff;
  this->deviceName  = SPI_STR;
  this->isUsable    = false;
  this->setClock    = setClock;
  this->bitSize     = bitSize;
  this->wData       = (uint8_t *)0;
  this->wSize       = 0;
  this->rSize       = 0;
  this->wIndex      = 0;
  this->rIndex      = 0;
  this->handlerStatus  = 0;

  if( (1 << device) & this->isInitialized) {
    SystemDebug.log(DEBUG_WRN, "%s %d module is already initialized and used!!", this->deviceName, device);
    return;
  }
  else {
    this->isInitialized = 1 << device;
  }

  //devBaseAddr = getSPIAddr(device);
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

uint8_t SPIInterface::moduleInitialize(eSPI_DEV eModule) {
  // 1 Enable SSI Clock
  PAD_SysPeripheralClockEnable(SYSCTL_RCGCSSI_ADDR, eModule_0);
  // 2 -> 5 Config SSI pin on port A
  // PAD_GPIOSSIPinConfig();
  // 3 Frame support
  return 0;
}

// SPO Clock Polarity Bit
// 0 : _____-_-_____ Low steady
// 1 : -----_-_----- High steady

// SPH Phase Control Bit
// 0 : Capture Data on first clock edge transition
// 1 : Capture Data on second clock edge transition

static uint8_t getSPHSPOValue(eSPI_Mode mode) {
  switch(mode) {
    case SPI_Mode_0:
      return 0x0;
    case SPI_Mode_1:
      return 0x1;
    case SPI_Mode_2:
      return 0x2; 
    case SPI_Mode_3:
      return 0x3;
    default:
      return 0x0;
  }
} 

static uint8_t getFrameModeValue(eSPI_Mode mode) {
  switch(mode) {
    case SPI_Mode_TI:
      return 0x1;
    case SPI_Mode_Microwire:
      return 0x2;
    case SPI_Mode_0:
    case SPI_Mode_1:
    case SPI_Mode_2:
    case SPI_Mode_3:
      return 0x0;
    default:
      return 0x0;
  }
}

uint8_t SPIInterface::frameSupport(eSPI_Mode transMode, uint8_t bitSize) {
  uint32_t tempVal = 0;
  // Refer 15.4 Frame formats
  // 1 - Ensure SSE Bit is clear
  SPI_SSICR1 &= (SPI_SSICR1 & (~BIT1));
  // 2 - Set SSI Master
  SPI_SSICR1 = 0;
  // 3 - Config SSI clock source
  SPI_SSICC = 0;
  // 4 - Configure clock prescaler 
  SPI_SSICPSR = 4;
  // 5 - Configure SSICR0 -> clock, frame mode, data size
  tempVal = (SysCtlClockGet()/(this->setClock*SPI_SSICPSR)) << 8;
  tempVal |= getSPHSPOValue(transMode) << 6;
  tempVal |= getFrameModeValue(transMode) << 4; // force to use Freescale SPI Frame Format
  tempVal |= bitSize - 1;
  SPI_SSICR0 = tempVal;
  // 6 - Optional uDMA
  // 7 - Enable SSI
  SPI_SSICR1 |= BIT1;
  PAD_GPIOSSIPinConfig();
  return 0;
}


uint8_t SPIInterface::write(uint32_t wSize, void *wBuffer) {
  uint8_t status = 0;
  if (wSize != 0) {
    this->wSize = wSize;
    this->wIndex = 0;
    this->wData = (uint8_t *)wBuffer;

    SystemDebug.log(DEBUG_LV0, "[%s] Write request with %d size!!!", this->deviceName, wSize);
    // Write handling
    while ((this->wSize > this->wIndex) && (0 == (SPI_SSISR & BIT3))) {
        SPI_SSIDR = this->wData[this->wIndex++];
    }
    if (this->wSize > this->wIndex) {
      // set transmission interrupt for keep transmit in ISR
      SystemDebug.log(DEBUG_LV0, "[%s] Write request with 0 size!!!", this->deviceName);
      SPI_SSIIM |= SSI_TXMIS;
    }
  } 
  else {
    SystemDebug.log(DEBUG_WRN, "[%s] Write request with 0 size!!!", this->deviceName);
    status = 1;
  }
  return status;
}

uint8_t SPIInterface::read(uint32_t rSize, void *rBuffer) {
  uint8_t status = 0;
  if (rSize != 0) {
    this->rIndex = 0;
    this->rSize = wSize;
    this->rData = (uint8_t *) rBuffer;

    SystemDebug.log(DEBUG_LV0, "[%s] Read request with %d size!!!", this->deviceName, wSize);
    // Read handling
    while ((this->rSize != this->rIndex) && (SPI_SSISR & BIT2)) {
        this->rData[rIndex++] = SPI_SSIDR;
    }
  }
  else {
    SystemDebug.log(DEBUG_WRN, "[%s] Read request with 0 size", this->deviceName);
    status = 1;
  }
  return status;
}

void SPIInterface::interruptMaskSet(int flags) {
  ASSERT(flags & SSI_RORMIS
      || flags & SSI_RTMIS
      || flags & SSI_RXMIS
      || flags & SSI_TXMIS
         );
  SPI_SSIIM |= flags;
}

void SPIInterface::interruptClear(int flags) {
  ASSERT(flags & SSI_RORMIS
      || flags & SSI_RTMIS
      || flags & SSI_RXMIS
      || flags & SSI_TXMIS
         );
  SPI_SSIICR = flags;
}

void SPIInterface::rwHandler(void) {
  // Write handling
  while ((this->wSize != this->wIndex) && (0 == (SPI_SSISR & BIT3))) {
      SPI_SSIDR = this->wData[this->wIndex++];
  }
  if (this->wSize == this->wIndex) {
    SPI_SSIMIS &= ~SSI_TXMIS;
  }

  // Read handling
  while ((this->rSize != this->rIndex) && (SPI_SSISR & BIT2)) {
      this->rData[rIndex++] = SPI_SSIDR;
  }

  // Dummy read if requested buffer received enough data but there are still more data
  if ((this->rSize == this->rIndex) && (SPI_SSISR & BIT2)) {
    while ((this->rSize == this->rIndex) && SPI_SSISR & BIT2) {
      bugBuffer[fakeIndex++] = SPI_SSIDR; 
      if (fakeIndex >= 4096) {
        fakeIndex = 0;
      }
    }

    if (fakeIndex >= 4096) {
      fakeIndex = 0;
    }
    bugBuffer[fakeIndex++] = 0xAA; // magic space
  }
}

eSSI_HANDLE_STATUS SPIInterface::isWriting(void) {
  eSSI_HANDLE_STATUS status = SSI_HANDLE_OK;
  if (this->wSize != this->wIndex)
    status = SSI_WRITE_ERR;
  
  return status;
}
eSSI_HANDLE_STATUS SPIInterface::isReading(void) {
  eSSI_HANDLE_STATUS status = SSI_HANDLE_OK;
  if (this->rSize != this->rIndex)
    status = SSI_READ_ERR;
  
  return status;
}

void SPIInterface::interruptHandler(void) {
  uint32_t status = SPI_SSIMIS;
  this->handlerStatus = SPI_SSISR;
  SystemDebug.log(DEBUG_LV0,"SPI status %08x\r\n", this->handlerStatus);
  SystemDebug.log(DEBUG_LV0,"SPI interrupt status %08x\r\n", status);

  if (status & SSI_RORMIS) {
    // Handle receive over run
    SPI_SSIICR |= SSI_RORMIS;
  }

  if (status & SSI_RTMIS) {
    // Handle receive time-out
    rwHandler();
    SPI_SSIICR |= SSI_RTMIS;
  }

  if (status & SSI_RXMIS) {
    // Handle receive status int
    rwHandler();
    SPI_SSIICR |= SSI_RTMIS;
  }

  if (status & SSI_TXMIS) {
    // Handle transmit status int
    rwHandler();
    SPI_SSIICR |= SSI_TXMIS;
  }
}

void SPIInterface::setUseModuleFssPin(bool isUse) {
  PAD_GPIOSSICSPinConfig(isUse);
}

void SPIInterface::setCSPin(void) {
  PAD_GPIOPinSet(GPIOPA_APB_BASE, BIT3);
}

void SPIInterface::clearCSPin(void) {
  PAD_GPIOPinClear(GPIOPA_APB_BASE, BIT3);
}
