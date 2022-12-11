#ifndef __SPI_HW__
#define __SPI_HW__
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

//APIs
bool SPI_WriteBytes(uint32_t count, uint8_t *data);
bool SPI_ReadBytes(uint32_t count, uint8_t *data);

#ifdef __cplusplus
}
#endif

typedef enum {
    SPI0_DEV,
    SPI1_DEV,
    SPI2_DEV,
    SPI3_DEV,
} SPI_DEV;

typedef enum {
    SPI_Mode_0,
    SPI_Mode_1,
    SPI_Mode_2,
    SPI_Mode_3,
    SPI_MAX,
} SPI_Mode;

// Current code only use SSI0 device
class SPIInterface {
        static uint8_t isInitialized;
        const char *deviceName;
        bool isUsable;
        uint32_t setClock;
        uint8_t moduleInitialize(SPI_DEV eModule);
    public:
        SPIInterface(SPI_DEV device, SPI_Mode transMode, uint32_t setClock, uint8_t bitSize);
        uint8_t frameSupport(SPI_Mode transMode, uint8_t bitSize);
        uint8_t write(uint32_t wSize, uint8_t *wBuffer);
        uint8_t read(uint32_t rSize, uint8_t *rBuffer);
};

#endif
