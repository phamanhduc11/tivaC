#ifndef __SPI_HW__
#define __SPI_HW__
#include <stdint.h>
#include <stdbool.h>

#include "Global/Include.h"


typedef enum {
    SPI0_DEV,
    SPI1_DEV,
    SPI2_DEV,
    SPI3_DEV,
} eSPI_DEV;

typedef enum {
    SPI_Mode_TI,
    SPI_Mode_0,
    SPI_Mode_1,
    SPI_Mode_2,
    SPI_Mode_3,
    SPI_Mode_Microwire,
    SPI_MAX,
} eSPI_Mode;

typedef enum {
    SSI_RORMIS  = BIT0,
    SSI_RTMIS   = BIT1,
    SSI_RXMIS   = BIT2,
    SSI_TXMIS   = BIT3,
    SSI_INT_MAX,
} eSSI_INT;

typedef enum {
    SSI_HANDLE_OK,
    SSI_WRITE_ERR,
    SSI_READ_ERR,
    SSI_STATUS_MAX,
} eSSI_HANDLE_STATUS;

// Current code only use SSI0 device
class SPIInterface {
    private:
        static uint8_t isInitialized;
        const char *deviceName;
        bool isUsable;
        uint32_t setClock;
        uint32_t bitSize;
        uint8_t const *wData;
        uint8_t *rData;
        volatile uint32_t wSize;
        volatile uint32_t rSize;
        volatile uint32_t wIndex;
        volatile uint32_t rIndex;
        volatile uint32_t handlerStatus;
        uint8_t moduleInitialize(eSPI_DEV eModule);
        void rwHandler(void);
    public:
        SPIInterface(eSPI_DEV device, eSPI_Mode transMode, uint32_t setClock, uint8_t bitSize);
        uint8_t frameSupport(eSPI_Mode transMode, uint8_t bitSize);
        uint8_t write(uint32_t wSize, void *wBuffer);
        uint8_t read(uint32_t rSize, void *rBuffer);
        eSSI_HANDLE_STATUS isWriting(void);
        eSSI_HANDLE_STATUS isReading(void);
        void interruptClear(int flags);
        void interruptMaskSet(int flags);
        void interruptHandler(void);
        void setUseModuleFssPin(bool isUse);
        void setCSPin(void);
        void clearCSPin(void);
};

void SPI_SetDevice(SPIInterface *spidev);

#ifdef __cplusplus
extern "C"
{
#endif

//APIs
void SPI_WriteBytes(uint32_t count, uint8_t *data);
void SPI_ReadBytes(uint32_t count, uint8_t *data);

#ifdef __cplusplus
}
#endif


#endif
