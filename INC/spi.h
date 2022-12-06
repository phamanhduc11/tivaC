#ifndef __SPI_HW__
#define __SPI_HW__
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define SPI_BASE 0x40008000

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

//APIs
bool SPI_WriteBytes(uint32_t count, uint8_t *data);
bool SPI_ReadBytes(uint32_t count, uint8_t *data);

#ifdef __cplusplus
}
#endif

#endif